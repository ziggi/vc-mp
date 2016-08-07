//----------------------------------------------------
//
// GPL code license:
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author: kyeman
//
//----------------------------------------------------

#pragma once

#define ADDR_HWND								0x7897A4
#define ADDR_ID3D8DEVICE						0x7897A8
#define ADDR_ID3D8								0x7897B4
#define ADDR_KEYSTATES							0x7DBCB0
#define ADDR_CPED_SAY							0x5226B0 // CPed_Say __thiscall (uint)
#define ADDR_ENTITY_ONSCREEN					0x4885D0 // CEntity_GetIsOnScreen __thiscall (void)
#define ADDR_PRE_GAME_PROCESS					0x4A6190 // Poor naming, is actually Render2DStuff
#define ADDR_PRE_GAME_PROCESS_STORAGE			0x4A5D77
#define ADDR_CFONT_SETDROPSHADOWPOSITION		0x54ff20
#define ADDR_CFONT_SETDROPCOLOR					0x54ff30
#define ADDR_CFONT_SETALPHAFADE					0x54ffc0
#define ADDR_CFONT_SETRIGHTJUSTIFYWRAP			0x54ffd0
#define ADDR_CFONT_SETFONTSTYLE					0x54ffe0
#define ADDR_CFONT_SETPROPON					0x550020
#define ADDR_CFONT_SETPROPOFF					0x550030
#define ADDR_CFONT_SETRIGHTJUSTIFYOFF			0x550040
#define ADDR_CFONT_SETRIGHTJUSTIFYON			0x550060
#define ADDR_CFONT_SETBACKGROUNDONLYTEXTOFF		0x550080
#define ADDR_CFONT_SETBACKGROUNDONLYTEXTON		0x550090
#define ADDR_CFONT_SETBACKGROUNDCOLOR			0x5500a0
#define ADDR_CFONT_SETBACKGROUNDOFF				0x5500d0
#define ADDR_CFONT_SETBACKGROUNDON				0x5500e0
#define ADDR_CFONT_SETCENTRESIZE				0x5500f0
#define ADDR_CFONT_SETWRAPX						0x550100
#define ADDR_CFONT_SETCENTREOFF					0x550110
#define ADDR_CFONT_SETCENTREON					0x550120
#define ADDR_CFONT_SETJUSTIFYOFF				0x550140
#define ADDR_CFONT_SETJUSTIFYON					0x550150
#define ADDR_CFONT_SETCOLOR						0x550170
#define ADDR_CFONT_SETSCALE						0x550230
#define ADDR_CFONT_PRINTSTRING					0x551040
#define ADDR_ASCIITOUNICODE						0x552500
#define ADDR_SPAWN_OBJECT						0x4DB3F0
#define ADDR_GIVE_ACTOR_WEAPON					0x4FFA30
#define ADDR_SET_ACTOR_ARMED_WEAPON				0x4FF900
#define ADDR_GAME_RUNNING						0x86969C
#define ADDR_ID_FROM_ACTOR						0x451CF0
#define ADDR_ACTOR_FROM_ID						0x451CB0
#define ADDR_PED_TABLE							0x97F2AC
#define ADDR_SET_CURRENT_WEAPON					0x4FF90D
#define ADDR_SET_OBJECTIVE						0x521F10
#define ADDR_VEHICLE_FROM_ID					0x451C70
#define ADDR_ID_FROM_VEHICLE					0x42C4B0
#define ADDR_VEHICLE_TABLE						0xA0FDE4
#define ADDR_OBJECT_TABLE						0x94DBE0
#define ADDR_OBJECT_FROM_ID						0x451C30
#define ADDR_ACTOR_PUT_IN_VEHICLE				0x4C6AC0
#define ADDR_VEHICLE_SET_DRIVER					0x512520
#define ADDR_FIND_GROUND_Z_FROM					0x4D51D0
#define ADDR_SET_ACTOR_SKIN						0x50D900
#define ADDR_REQUEST_MODEL_REQ_VAR				0x7D1DE0
#define ADDR_REQUEST_MODEL						0x40E310
#define ADDR_LOAD_REQUESTED_MODELS1				0x4D0DB0
#define ADDR_LOAD_REQUESTED_MODELS2				0x40B5F0
#define ADDR_LOAD_REQUESTED_MODELS3				0x4D0F30
#define ADDR_ACTOR_REMOVE_WEAPONS				0x4FF740
#define ADDR_MENU								0x869668
#define ADDR_STARTGAME							0x869641
#define ADDR_CAMERA_BASE_PTR					0x7E4688
#define ADDR_CAMERA_ON_ENTITY					0x46A4F8
#define ADDR_TIE_MARKER_TO_ACTOR				0x4C3B40
#define ADDR_SET_MARKER_COLOR					0x4C3930
#define ADDR_SHOW_ON_RADAR1						0x4C3840
#define ADDR_RESTART_IF_WASTED_AT				0x443010
#define ADDR_FRAME_LIMITER						0x869655
#define ADDR_HUD_SWITCH							0xA10B45
#define ADDR_IS_MODEL_LOADED_VAR				0x94DDD8
#define ADDR_SET_OBJECTIVE_STORAGE				0x401175
#define ADDR_IS_KEY_PRESSED						0x460C00
#define ADDR_GET_PLAYER_CONTROLS				0x4AB060
#define ADDR_SET_SKIN_MODELNAME					0x4EF030
#define ADDR_REFRESH_ACTOR_SKIN					0x4EEFD0

//-----------------------------------------------------------
