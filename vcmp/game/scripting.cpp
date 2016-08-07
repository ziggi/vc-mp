// PUBLIC DOMAIN
//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author: spookie
//
//----------------------------------------------------------

//
//	Taken from spookie's speedo mod.
//


/*
	file:
		scripting.cpp
	desc:
		This file contains the Vice City scripting engine stuff.
*/

#include "../main.h"

// Globals
const FARPROC ProcessOneCommand	= (FARPROC)0x44FBE0;	// tnx for the name CyQ.

/*	The four offsets below need modifying for vice city 1.1.
	This is done dynamically in Custom.cpp inside CustomInitialize()	*/
DWORD SCRIPT_BASE = 0x821280;	// Script memory location in v1.0.
GAME_SCRIPT_THREAD gst;				// Structure needed to call ProcessOneCommand. [mov ecx, offset gst]
BYTE ScriptBuf[MAX_SCRIPT_SIZE];	// Buffer for generating our script stub.
DWORD ScriptVars[MAX_SCRIPT_VARS];	// Array for saving current script variables.

// Functions

//--------------------------------------------------------------------------------
//	func:
//		ExecuteScriptBuf()
//
//	desc:
//		This sends our generated script stub through the game's interpreter.
//
//		Called only by ScriptCommand(). Don't call directly.
//
//	params:
//		n/a
//
int __declspec(naked) ExecuteScriptBuf()
{
	__asm
	{
		//int 4						// SoftICE Debugging - bpint4.
		mov ecx, OFFSET ScriptBuf	// Move our script buffer base into ecx.
		sub ecx, SCRIPT_BASE		// Subtract usual script base.
		mov gst.dwScriptIP, ecx		// Move ecx into the GAME_SCRIPT_THREAD structure.

		mov ecx, OFFSET gst			// Move offset of our GAME_SCRIPT_THREAD structure into ecx.
		call ProcessOneCommand		// Call the game's script opcode processor.
		xor eax, eax				// eax = 0.
		mov al, gst.bJumpFlag		// Move the jumpflag into al (return value).
		ret							// return.
	}
}

//--------------------------------------------------------------------------------
//	func:
//		ScriptCommand()
//
//	desc:
//		This generates a stript stub in ScriptBuf, then calls ExecuteScriptBuf()
//		to send it through the game's interpreter.
//
//		The function returns the result of the a conditional opcode (1/0) or
//		0 if it wasn't conditional.
//
//	params:
//		ScriptCommand
//			This is a pointer to a SCRIPT_COMMAND structure containing information
//			on a Vice City opcode.
//
//		varargs
//			You must also pass parameters matching the parameter string in the
//			"Params" member of the passed SCRIPT_COMMAND structure. See ReadMe.
//
int ScriptCommand(const SCRIPT_COMMAND* ScriptCommand, ...)
{
	va_list ap;
	const char* p = ScriptCommand->Params;			// Get parameter string.
	va_start(ap, ScriptCommand);					// Start varargs at ScriptCommand.
	memcpy(&ScriptBuf, &ScriptCommand->OpCode, 2);	// Copy opcode to script buf.
	int buf_pos = 2;	// Position in buffer after opcode.
	int var_pos = 0;	// Init var_pos.

	while(*p)			// While we've not ran out of parameters...
	{
		switch(*p)		// Switch current parameter.
		{
			case 'i':	// If integer...
			{
				long i = va_arg(ap, long);		// Grab an int off the stack.
				if ((i >= -128) && (i <= 127))	// If it's in range of a char...
				{
					char ch = (char)i;			// Typecast to char.
					ScriptBuf[buf_pos] = 0x04;	// Variable data type = 0x04 (1b int).
					buf_pos++;					// Increment buffer position.
					ScriptBuf[buf_pos] = ch;	// Insert the char.
					buf_pos++;					// Increment buffer position.
				}
				else if ((i >= -32768) && (i <= 32767))	// If it's in range of a short...
				{
					short sh = (short)i;				// Typecast to short.
					ScriptBuf[buf_pos] = 0x05;			// Variable data type = 0x05 (2b int).
					buf_pos++;							// Increment buffer position.
					memcpy(&ScriptBuf[buf_pos], &sh, 2);// Insert the short.
					buf_pos += 2;						// Increment buffer by 2b.
				}
				else									// Else it must be a 4b int.
				{
					ScriptBuf[buf_pos] = 0x01;			// Variable data type = 0x01 (4b int).
					buf_pos++;;							// Increment buffer position.
					memcpy(&ScriptBuf[buf_pos], &i, 4);	// Insert the int.
					buf_pos += 4;						// Increment buffer by 4b.
				}
				break;
			}
			case 'f':	// If float...
			{
				/*	Take note, MSVC++ puts a double on the stack
					even if you typecase as a float. <3 Microsoft.	*/

				float f = (float)va_arg(ap, double);	// Get float off the stack.
				ScriptBuf[buf_pos] = 0x06;				// Variable data type = 0x06 (float).
				buf_pos++;								// Increment buffer position.
				memcpy(&ScriptBuf[buf_pos], &f, 4);		// Copy float into script buf.
				buf_pos += 4;							// Increment buffer by 4b.
				break;
			}
			case 'v':	// If variable...
			{
				DWORD* v = va_arg(ap, DWORD*);							// Get the pointer to the passed variable.
				ScriptBuf[buf_pos] = 0x02;								// Variable data type = 0x02 (Global Var).
				buf_pos++;												// Increment buffer position.
				WORD var_offs = var_pos * 4;							// Variable offset.
				DWORD* ScriptVar = (DWORD*)(SCRIPT_BASE + var_offs);	// Get pointer to variable.
				ScriptVars[var_pos] = *ScriptVar;						// Read out current variable there.
				*ScriptVar = *v;										// Insert our variable.
				memcpy(&ScriptBuf[buf_pos], &var_offs, 2);				// Copy the offset into the script.
				buf_pos += 2;											// Increment buffer by 2b.
				var_pos++;												// Increment the variable position.
				break;
			}
			case 's':	// If string...
			{
				char* s1 = va_arg(ap, char*);			// Get the pointer to the passed string.
				char s2[8];								// Define a new 8 char string.
				ZeroMemory(s2, 8);						// Zero-out the memory in new string (this needed?).
				strncpy(s2, s1, 8);						// Copy passed string to new string, max 8 chars.
				memcpy(&ScriptBuf[buf_pos], s2, 8);		// Copy new string into script buf.
				buf_pos += 8;							// Increment buffer by 8b.
				break;
			}
		}
		++p;		// Next parameter
	}
	va_end(ap);		// End varargs.

	if (var_pos)	// if we've used a variable...
	{
		int result = ExecuteScriptBuf();	// Execute the script stub now.

		p = ScriptCommand->Params;		// Get the parameter string, again.
		va_start(ap, ScriptCommand);	// Start varargs at ScriptCommand, again.
		var_pos = 0;					// Init var_pos.
		while(*p)						// While we've not ran out of parameters...
		{
			switch(*p)					// Switch current parameter.
			{
				case 'i':				// If integer...
				{
					va_arg(ap, long);	// Skip it.
					break;
				}
				case 'f':				// If float...
				{
					va_arg(ap, double);	// Skip it.
					break;
				}
				case 'v':				// If variable...
				{
					DWORD* v = va_arg(ap, DWORD*);							// Get passed variable pointer.
					DWORD* ScriptVar = (DWORD*)(SCRIPT_BASE + (var_pos*4));	// Get pointer to script variable.
					*v = *ScriptVar;										// Copy passed variable out of the script.
					*ScriptVar = ScriptVars[var_pos];						// Copy saved script variable back.
					var_pos++;												// Increment variable position.
					break;
				}
				case 's':				// If string...
				{
					va_arg(ap, char*);	// Skip it.
					break;
				}
			}
			++p;		// Next parameter.
		}
		va_end(ap);		// End varargs.

		return result;	// Now we've fixed the script vars, return.
	} else {
		return ExecuteScriptBuf();	// If we didn't have any variables, just return.
	}
}


//--------------------------------------------------------------------------------