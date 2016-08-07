# VC:MP 0.1c source code

Licensed for use under the GNU General Public Licence.
Note: Some portions are derived from public domain
source code. Please see relevant files for futher details.


# Projects

- launch3: Basic MFC application to launch gta-vc.exe with the command line arguments required to start VC:MP.
- rcon: The remote console client.
- vcmp: A dll renamed as .flt and placed within the GTA:VC /mss folder. Files named as *.flt will be automatically loaded inprocess by the Miles Sound System which GTA:VC uses.
- server: The dedicated server.

# Build

Compilation instructions Microsoft Visual C++ 6.0 or higher:

- VC:MP 0.1 requires the RakNet networking library (http://www.rakkarsoft.com/).
- Copy all files from the \Source and \Include trees in the RakNet distribution to '{location of vcmp source}\raknet'. Alternatively, compile RakNet as a static lib and readjust the linker settings to use it.
- You will now need to add the following enum IDs to /raknet/PacketEnumerations.h
 - ID_PLAYER_SYNC
 - ID_VEHICLE_SYNC
 - ID_PASSENGER_SYNC
 - ID_RCON_COMMAND
 - ID_RCON_RESPONSE
- Add them to the end in the user packets section.

# IMPORTANT

If you're using VC++6.0 will need to open `raknet/NetworkTypes.h` and add `#pragma pack(1)` above all struct definitions.

ie
```C++
#pragma pack(1)

struct PlayerID
{
```

- Open the vcmp.dsw workspace in Microsoft Visual C++.
- You may need to adjust the compile paths in all projects. The default build path is set to compile to c:\gtavc\. e.g. Project Settings > vcmp > Link. The build path is c:\gtavc\mss\vcmp.flt.
- After you have set your compile paths you should be able to compile all projects.
