// PUBLIC DOMAIN
//
//	Taken from spookie's speedo mod.
//

#pragma once

/*
	file:
		scripting.h
	desc:
		This is the header file for GameScripting.cpp.
*/

#define MAX_SCRIPT_VARS	16	// Size of our variable saving array
#define MAX_SCRIPT_SIZE	255	// Size of ScriptBuf - Max is really (2+(13*5))

struct GAME_SCRIPT_THREAD	// 0x88 bytes total.
{							// - Credit to CyQ & PatrickW
	void* pNext;			// 0x00
	void* pPrev;			// 0x04
	char strName[8];		// 0x08
	DWORD dwScriptIP;		// 0x10
	DWORD dwReturnStack[6];	// 0x14
	DWORD dwStackPointer;	// 0x2C
	DWORD dwLocalVar[18];	// 0x30
	BYTE bStartNewScript;	// 0x78
	BYTE bJumpFlag;			// 0x79
	BYTE bWorky;			// 0x7A
	BYTE bAwake;			// 0x7B
	DWORD dwWakeTime;		// 0x7C
	WORD wIfParam;			// 0x80
	BYTE bNotFlag;			// 0x82
	BYTE bWastedBustedCheck;// 0x83
	BYTE bWastedBustedFlag;	// 0x84
	BYTE bMissionThread;	// 0x85
	BYTE pad[2];			// 0x86
};

struct SCRIPT_COMMAND		//	Params
{							//		i = integer
	WORD OpCode;			//		f = float
	char Params[13];		//		v = variable
};							//		s = string

int ScriptCommand(const SCRIPT_COMMAND* ScriptCommand, ...);	// The main scripting function. See notes.

/*	Below are some opcodes i've defined, but it's still missing
	about 95% of them ;) See http://vc-db.webtools4you.net/		*/
const SCRIPT_COMMAND request_model				= { 0x0247, "i" };		// (CAR_*|BIKE_*|BOAT_*|WEAPON_*|OBJECT_*)
const SCRIPT_COMMAND load_requested_models		= { 0x038B, "" };		// -/-
const SCRIPT_COMMAND create_car					= { 0x00A5, "ifffv" };	// (CAR_*|BIKE_*|BOAT_*), x, y, z, var_car
const SCRIPT_COMMAND load_special_actor			= { 0x023C, "is" };		// SPECIAL_*, MODEL_*
const SCRIPT_COMMAND create_actor				= { 0x009A, "iifffv" };	// PEDTYPE_*, #MODEL, x, y, z, var_actor
const SCRIPT_COMMAND destroy_actor_fading		= { 0x034F, "v" };		// var_actor
const SCRIPT_COMMAND set_weather				= { 0x01B6, "i" };		// WEATHER_*
const SCRIPT_COMMAND set_fade_color				= { 0x0169, "iii" };	// Red(0-255), Green(0-255), Blue(0-255)
const SCRIPT_COMMAND fade						= { 0x016A, "ii" };		// (time in ms), FADE_*
const SCRIPT_COMMAND is_fading					= { 0x016B, "" };		// -/-
const SCRIPT_COMMAND freeze_player				= { 0x01B4, "vi" };		// PLAYER_CHAR, Freeze(1/0)
const SCRIPT_COMMAND name_thread				= { 0x03A4, "s" };		// "MAIN"
const SCRIPT_COMMAND set_max_wanted_level		= { 0x01F0, "i" };		// MaxLevel
const SCRIPT_COMMAND set_wasted_busted_check	= { 0x0111, "i" };		// Check(1/0)
const SCRIPT_COMMAND set_current_time			= { 0x00C0, "ii" };		// Hours, Minutes
const SCRIPT_COMMAND refresh_screen				= { 0x04E4, "ff" };		// x, y
const SCRIPT_COMMAND set_camera					= { 0x03CB, "fff" };	// x, y, z
const SCRIPT_COMMAND create_player				= { 0x0053, "vfffv" };	// 0, x, y, z, PLAYER_CHAR
const SCRIPT_COMMAND create_actor_from_player	= { 0x01F5, "vv" };		// PLAYER_CHAR, PLAYER_ACTOR
const SCRIPT_COMMAND set_actor_skin				= { 0x0352, "vs" };		// var_actor, MODEL_*
const SCRIPT_COMMAND refresh_actor_skin			= { 0x0353, "v" };		// var_actor
const SCRIPT_COMMAND select_interior			= { 0x04BB, "i" };		// INTERIOR_*
const SCRIPT_COMMAND make_actor_leave_vehicle	= { 0x03E2, "v" };		// var_actor
const SCRIPT_COMMAND clear_actor_objective		= { 0x011C, "v" };		// var_actor
const SCRIPT_COMMAND send_actor_to_car			= { 0x01D4, "vv" };		// var_actor, var_car
const SCRIPT_COMMAND send_actor_to_car_driver	= { 0x01D5, "vv" };		// var_actor, var_car
const SCRIPT_COMMAND set_car_color				= { 0x0229, "vii" };	// var_car col1 col2
const SCRIPT_COMMAND wait						= { 0x0001, "i" };		// time_in_ms
const SCRIPT_COMMAND create_thread				= { 0x00D7, "iz" };		// StartIP (4F)
const SCRIPT_COMMAND end_thread					= { 0x004E, "" };		// -/-
const SCRIPT_COMMAND destroy_car				= { 0x00A6, "i" };		// var_car
const SCRIPT_COMMAND set_car_z_angle			= { 0x0175, "if" };		// var_car, angle
const SCRIPT_COMMAND car_relative_coordinates	= { 0x0407, "vfffvvv" };// var_car, x, y, z, var_x, var_y, var_z
const SCRIPT_COMMAND set_car_max_speed			= { 0x00AD, "vf" };		// var_car, speed
const SCRIPT_COMMAND is_model_available			= { 0x0248, "i" };		// #MODEL
const SCRIPT_COMMAND create_actor_in_driverseat	= { 0x0129, "viiv" };	// var_car, pedtype, #MODEL, var_actor
const SCRIPT_COMMAND create_actor_in_passenger	= { 0x01c8, "viiiv" };	// var_car, pedtype, #MODEL, seat, var_actor
const SCRIPT_COMMAND drive_car_to_point1		= { 0x02c2, "vfff" };	// var_car, x, y, z
const SCRIPT_COMMAND car_ignore_traffic			= { 0x00AE, "vi" };		// var_car, flag
const SCRIPT_COMMAND set_car_immune_to_nonplayer= { 0x02aa, "vi" };		// var_car, immune(1/0)
const SCRIPT_COMMAND give_actor_weapon			= { 0x01B2, "vii" };	// var_actor, weapon, ammo
const SCRIPT_COMMAND reset_actor_flags			= { 0x01ED, "v" };		// var_actor
const SCRIPT_COMMAND set_actor_ped_stats		= { 0x0243, "vi" };		// var_actor, pedstats
const SCRIPT_COMMAND toggle_actor_wander		= { 0x0319, "vi" };		// var_actor, wander(1/0)
const SCRIPT_COMMAND play_music					= { 0x0394, "i" };		// music
const SCRIPT_COMMAND clear_player_wanted_level	= { 0x0110, "v" };		// PLAYER_CHAR
const SCRIPT_COMMAND set_car_door_status		= { 0x020A, "vi" };		// var_car, status
const SCRIPT_COMMAND remove_references_to_actor	= { 0x01C2, "v" };		// var_actor
const SCRIPT_COMMAND remove_references_to_car	= { 0x01C3, "v" };		// var_car
const SCRIPT_COMMAND set_player_wanted_level	= { 0x010D, "vi" };		// PLAYER_CHAR, level
const SCRIPT_COMMAND set_player_health			= { 0x0222, "vi" };		// PLAYER_CHAR, health
const SCRIPT_COMMAND get_car_health				= { 0x0227, "vv" };		// var_car, var_health
const SCRIPT_COMMAND text_styled				= { 0x00BA, "sii" };	// TEXT, time, style
const SCRIPT_COMMAND text_now					= { 0x00BC, "sii" };	// TEXT, time, style
const SCRIPT_COMMAND text_1number_styled		= { 0x01E3, "siii" };	// TEXT, number, time, style
const SCRIPT_COMMAND toggle_car_siren			= { 0x0397, "vi" };		// var_car, siren(1/0)
const SCRIPT_COMMAND toggle_widescreen			= { 0x02A3, "i" };		// widescreen(1/0)
const SCRIPT_COMMAND set_camera_behind_player	= { 0x0373, "" };		// -/-
const SCRIPT_COMMAND camera_on_vehicle			= { 0x0158, "vii" };	// var_car, ukn, ukn
const SCRIPT_COMMAND restore_camera				= { 0x015A, "" };		// -/-
const SCRIPT_COMMAND point_camera				= { 0x0160, "fffi" };	// x, y, z, type
const SCRIPT_COMMAND restore_camera_jumpcut		= { 0x02EB, "" };		// -/-
const SCRIPT_COMMAND set_camera_position		= { 0x015F, "ffffff" }; // x, y, z, vx, vy, vz
const SCRIPT_COMMAND tie_actor_to_player		= { 0x01DF, "vv" };		// var_actor, PLAYER_CHAR
const SCRIPT_COMMAND tie_marker_to_car			= { 0x0161, "iiiv" };	// var_car, ukn, ukn, var_marker
const SCRIPT_COMMAND tie_marker_to_actor		= { 0x0162, "viiv" };	// var_actor, ukn, ukn, var_marker
const SCRIPT_COMMAND disable_marker				= { 0x0164, "v" };		// var_marker
const SCRIPT_COMMAND set_marker_color			= { 0x0165, "vi" };		// var_marker, color
const SCRIPT_COMMAND set_marker_brightness		= { 0x0166, "ii" };		// var_marker, brightness
const SCRIPT_COMMAND create_marker				= { 0x0167, "fffiiv" };	// x, y, z, ukn, ukn, var_marker
const SCRIPT_COMMAND show_on_radar				= { 0x0168, "vi" };		// var_marker, size
const SCRIPT_COMMAND set_car_driver_behaviour	= { 0x00AF, "vi" };		// var_car, behaviour
const SCRIPT_COMMAND set_actor_to_kill_actor	= { 0x01C9, "vv" };		// var_actor, var_actor
const SCRIPT_COMMAND set_actor_to_kill_player	= { 0x01CA, "vv" };		// var_actor, PLAYER_CHAR
const SCRIPT_COMMAND is_actor_dead				= { 0x0118, "v" };		// var_actor
const SCRIPT_COMMAND player_near_point_on_foot	= { 0x00F6, "vffffffi" };//	PLAYER_CHAR, x, y, z, rx, ry, rz, b
const SCRIPT_COMMAND create_icon_marker_sphere	= { 0x02A7, "fffiv" };
const SCRIPT_COMMAND is_player_near_point_3d	= { 0x00F5, "vffffffi" };//	PLAYER_CHAR, x, y, z, rx, ry, rz, b
const SCRIPT_COMMAND is_actor_near_point_3d		= { 0x00FE, "vffffffi" };//	var_actor, x, y, z, rx, ry, rz, b
const SCRIPT_COMMAND is_car_near_point_3d		= { 0x01AF, "vffffffi" };//	var_car, x, y, z, rx, ry, rz, b
const SCRIPT_COMMAND set_player_z_angle			= { 0x0171, "if" };
const SCRIPT_COMMAND get_player_z_angle			= { 0x0170, "iv" };
const SCRIPT_COMMAND give_player_weapon			= { 0x01B1, "iii" };	// PLAYER_CHAR, weapon, ammo
const SCRIPT_COMMAND add_to_player_money		= { 0x0109, "vi" };		// PLAYER_CHAR, money
const SCRIPT_COMMAND restart_if_wasted_at		= { 0x016C, "ffff" };	// x, y, z, a
const SCRIPT_COMMAND restart_if_busted_at		= { 0x016D, "ffff" };	// x, y, z, a
const SCRIPT_COMMAND is_key_pressed				= { 0x0e1, "ii" };

// added by kyeman. (I don't use correct conventions so don't copy elsewhere
// without double checking they're how you want them, yalls.

const SCRIPT_COMMAND toggle_player_infinite_run = { 0x0330, "ii" };
const SCRIPT_COMMAND toggle_player_controllable = { 0x01B4, "ii" };
const SCRIPT_COMMAND toggle_player_fast_reload  = { 0x0331, "ii" };
const SCRIPT_COMMAND set_player_armed_weapon	= { 0x01b8, "ii" };
const SCRIPT_COMMAND get_player_armed_weapon	= { 0x046f, "iv" };
const SCRIPT_COMMAND remove_player_weapons		= { 0x03b8, "i"  };
const SCRIPT_COMMAND put_player_in_car			= { 0x0369, "ii" };
const SCRIPT_COMMAND put_player_at_and_remove_from_car = { 0x012a, "ifff" };
const SCRIPT_COMMAND set_actor_immunities		= { 0x02ab, "iiiiii" };
const SCRIPT_COMMAND set_car_immunities			= { 0x02ac, "iiiiii" };
const SCRIPT_COMMAND create_object				= { 0x0107, "ifffv" };
const SCRIPT_COMMAND set_object_z_angle			= { 0x0177, "if" };
const SCRIPT_COMMAND set_object_collision		= { 0x0382, "ii" }; 
const SCRIPT_COMMAND play_sound					= { 0x018c, "fffi" };
const SCRIPT_COMMAND create_sound				= { 0x018d,	"fffiv" };
const SCRIPT_COMMAND get_player_weapon_ammo		= { 0x0419, "iiv" };
const SCRIPT_COMMAND get_car_z_angle			= { 0x0174, "iv" };


