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
//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author: kyeman
//
//-----------------------------------------------------------

#pragma once

#include <windows.h>

//-----------------------------------------------------------

#define MAX_PLAYERS		50
#define MAX_VEHICLES	200

//-----------------------------------------------------------

typedef struct _RGBA {
	unsigned char r,g,b,a;
} RGBA, *PRGBA;

typedef struct _VECTOR {
	float X,Y,Z;
} VECTOR, *PVECTOR;

typedef struct _MATRIX4X4 {
	VECTOR vLookRight;
	float  pad_r;
	VECTOR vLookUp;
	float  pad_u;
	VECTOR vLookAt;
	float  pad_a;
	VECTOR vPos;
	float  pad_p;
} MATRIX4X4, *PMATRIX4X4;

//-----------------------------------------------------------

#define _pad(x,y) BYTE x[y]

#pragma pack(1)
typedef struct _ENTITY_TYPE {

	DWORD	func_table; // 0
	MATRIX4X4 mat; // 4-68
	_pad(__pad0,8); // 68-76
	PDWORD	pModel; // 76-80
	BYTE	nControlFlags; // 80-81
	BYTE	nControlFlags2; // 81-82
	_pad(__pad1,10); // 82-92
	WORD	nModelIndex; // 92-94
	_pad(__pad2,18); // 94-112
	VECTOR  vecMoveSpeed; // 112-124
	VECTOR  vecTurnSpeed; // 124-136
	_pad(__pad3,146); // 136-282
	BYTE	byteSunkFlags; // 282-284
	BYTE	byteLockedFlags; // 283-284

} ENTITY_TYPE;

#pragma pack(1)
typedef struct _PED_TYPE {
	ENTITY_TYPE entity; // 0-284

	_pad(__pad0a,48); // 284-332
	BYTE	byteShootFlags; // 332-333
	BYTE	byteJumpFlags; // 333-334
	_pad(__pad1a,246); // 334-580
	BYTE	byteAction; // 580-581
	_pad(__pad2a,271); // 581-852
	float	fHealth; // 852-856
	float	fArmour; // 856-860
	_pad(__pad3a,24); // 860-884
	float	fRotation1; // 884-888
	float	fRotation2; // 888-892
	_pad(__pad4a,44); // 892-936
	PDWORD	pVehicle; // 936-940
	BYTE	byteIsInVehicle; // 940-941
	_pad(__pad5a,39); // 941-980
	BYTE	bytePedType; // 980-981
	_pad(__pad6a,451); // 981-1432
	DWORD  dwWeaponUsed; // 1432-1436
	PDWORD pdwDamageEntity; // 1436
	/// ............
} PED_TYPE;

#pragma pack(1)
typedef struct _VEHICLE_TYPE {
	ENTITY_TYPE entity; // 0-284

	_pad(__pad0a,132); // 284-416
	BYTE	byteColor1; // 416-417
	BYTE	byteColor2; // 417-418
	_pad(__pad1a,6); // 418-424
	PED_TYPE * pDriver; // 424-428
	PED_TYPE * pPassengers[7]; // 428-456 (probably 8)
	_pad(__pad2a,4); // 456-460
	BYTE	bytePassengersCount; // 460-461
	_pad(__pad2b,3); // 461-464
	BYTE	byteMaxPassengers; // 464-465
	_pad(__pad3a,23); // 465-488
	float	fSteerAngle1; // 488-492
	float	fSteerAngle2; // 492-496
	float	fAcceleratorPedal; // 496-500
	float	fBrakePedal; // 500-504
	_pad(__pad4a,12); // 504-516
	float	fHealth; // 516-520
	_pad(__pad5a,40); // 520-560
	DWORD	dwDoorsLocked; // 560-564
	_pad(__pad6a,4); // 564-568
	PDWORD  pdwDamageEntity; // 568-572
	DWORD	nRadio; // 572-576
	BYTE	byteHorn; // 576-577
	DWORD   dwUnk1; // 577-581
	BYTE	byteSiren; // 581-582
	_pad(__pad7a,874); // 582-1456
	float	fSpecialWeaponRotation1; // 1456 (following 2 are rhino turret and firetruck spray)
	float	fSpecialWeaponRotation2; // 1460	
	/// ............
} VEHICLE_TYPE;

//-----------------------------------------------------------

#define WEATHER_SUNNY		0
#define WEATHER_CLOUDY		1
#define WEATHER_RAINING		2
#define WEATHER_FOGGY		3
#define WEATHER_EXTRASUNNY	4
#define WEATHER_STORM		5
#define WEATHER_INTERIOR	6

//-----------------------------------------------------------

#define INTERIOR_OUTSIDE	0
#define INTERIOR_HOTEL		1
#define INTERIOR_MANSION	2
#define INTERIOR_BANK		3
#define INTERIOR_MALL		4
#define INTERIOR_STRIPCLUB	5
#define INTERIOR_LAWYERS	6
#define INTERIOR_CAFEROBINA	7
#define INTERIOR_CONCERT	8
#define INTERIOR_STUDIO		9
#define INTERIOR_AMMUNATION	10
#define INTERIOR_APPARTMENT	11
#define INTERIOR_POLICEHQ	12
#define INTERIOR_UNKNOWN	12
#define INTERIOR_STADIUM1	14
#define INTERIOR_STADIUM2	15
#define INTERIOR_STADIUM3	16
#define INTERIOR_CLUB		17
#define INTERIOR_PRINTWORKS	18

//-----------------------------------------------------------

#define FADE_OUT			0
#define FADE_IN				1

//-----------------------------------------------------------

#define	VEHICLE_SUBTYPE_CAR				1
#define	VEHICLE_SUBTYPE_BIKE			2
#define	VEHICLE_SUBTYPE_HELI			3
#define	VEHICLE_SUBTYPE_BOAT			4
#define	VEHICLE_SUBTYPE_PLANE			5

//-----------------------------------------------------------

#define ACTION_NORMAL 1
#define ACTION_DRIVING_VEHICLE 50
#define ACTION_WASTED 55
#define ACTION_GETTING_IN_VEHICLE 58
#define ACTION_EXITING_VEHICLE 60

//-----------------------------------------------------------

#define WEAPON_MODEL_CELLPHONE			258
#define WEAPON_MODEL_BRASSKNUCKLE		259
#define WEAPON_MODEL_SCREWDRIVER		260
#define WEAPON_MODEL_GOLFCLUB			261
#define WEAPON_MODEL_NITESTICK			262
#define WEAPON_MODEL_KNIFECUR			263
#define WEAPON_MODEL_BASEBALL_BAT		264
#define WEAPON_MODEL_HAMMER				265
#define WEAPON_MODEL_CLEAVER			266
#define WEAPON_MODEL_MACHETE			267
#define WEAPON_MODEL_KATANA				268
#define WEAPON_MODEL_CHAINSAW			269
#define WEAPON_MODEL_GRENADE			270
#define WEAPON_MODEL_TEARGAS			271
#define WEAPON_MODEL_MOLOTOV			272
#define WEAPON_MODEL_MISSILE			273
#define WEAPON_MODEL_COLT45				274
#define WEAPON_MODEL_PYTHON				275
#define WEAPON_MODEL_RUGER				276
#define WEAPON_MODEL_CHROMEGUN			277
#define WEAPON_MODEL_SHOTGSPA			278
#define WEAPON_MODEL_BUDDYSHOT			279
#define WEAPON_MODEL_M4					280
#define WEAPON_MODEL_TEC9				281
#define WEAPON_MODEL_UZI				282
#define WEAPON_MODEL_INGRAMSL			283
#define WEAPON_MODEL_MP5LNG				284
#define WEAPON_MODEL_SNIPER				285
#define WEAPON_MODEL_LASER				286
#define WEAPON_MODEL_ROCKETLA			287
#define WEAPON_MODEL_FLAME				288
#define WEAPON_MODEL_M60				289
#define WEAPON_MODEL_MINIGUN			290
#define WEAPON_MODEL_BOMB				291
#define WEAPON_MODEL_CAMERA				292
#define WEAPON_MODEL_FINGERS			293
#define WEAPON_MODEL_MINIGUN2			294
#define WEAPON_UNARMED					0
#define WEAPON_BRASSKNUCKLE				1
#define WEAPON_SCREWDRIVER				2
#define WEAPON_GOLFCLUB					3
#define WEAPON_NITESTICK				4
#define WEAPON_KNIFECUR					5
#define WEAPON_BASEBALL_BAT				6
#define WEAPON_HAMMER					7
#define WEAPON_CLEAVER					8
#define WEAPON_MACHETE					9
#define WEAPON_KATANA					10
#define WEAPON_CHAINSAW					11
#define WEAPON_GRENADE					12
#define WEAPON_TEARGAS					14
#define WEAPON_MOLOTOV					15
#define WEAPON_MISSILE					16
#define WEAPON_COLT45					17
#define WEAPON_PYTHON					18
#define WEAPON_CHROMEGUN				19
#define WEAPON_SHOTGSPA					20
#define WEAPON_BUDDYSHOT				21
#define WEAPON_TEC9						22
#define WEAPON_UZI						23
#define WEAPON_INGRAMSL					24
#define WEAPON_MP5LNG					25
#define WEAPON_M4						26
#define WEAPON_RUGER					27
#define WEAPON_SNIPER					28
#define WEAPON_LASER					29
#define WEAPON_ROCKETLA					30
#define WEAPON_FLAME					31
#define WEAPON_M60						32
#define WEAPON_MINIGUN					33
#define WEAPON_BOMB						34
#define WEAPON_HELICANNON				35
#define WEAPON_CAMERA					36
#define WEAPON_COLLISION				39
#define WEAPON_FALL						41
#define WEAPON_DRIVEBY					42
#define WEAPON_DROWN					43
#define WEAPON_WATER					50
#define WEAPON_EXPLOSION				51

//-----------------------------------------------------------
