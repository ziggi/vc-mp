# Microsoft Developer Studio Project File - Name="vcmp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=vcmp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vcmp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vcmp.mak" CFG="vcmp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vcmp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vcmp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cwcl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vcmp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=cwlink.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib wsock32.lib ../jpeg/libjpeg.lib d3d8/d3dx8.lib advapi32.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"c:\gtavc\mss\vc-mp.flt"

!ELSEIF  "$(CFG)" == "vcmp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_DEBUG1" /FAs /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=cwlink.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib wsock32.lib ../jpeg/libjpeg.lib d3d8/d3dx8.lib advapi32.lib winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"c:\gtavc\mss\vc-mp.flt" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "vcmp - Win32 Release"
# Name "vcmp - Win32 Debug"
# Begin Group "game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\address.h
# End Source File
# Begin Source File

SOURCE=.\game\aimstuff.cpp
# End Source File
# Begin Source File

SOURCE=.\game\aimstuff.h
# End Source File
# Begin Source File

SOURCE=.\game\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\game\camera.h
# End Source File
# Begin Source File

SOURCE=.\game\common.h
# End Source File
# Begin Source File

SOURCE=.\game\game.cpp
# End Source File
# Begin Source File

SOURCE=.\game\game.h
# End Source File
# Begin Source File

SOURCE=.\game\hooks.cpp
# End Source File
# Begin Source File

SOURCE=.\game\keystuff.cpp
# End Source File
# Begin Source File

SOURCE=.\game\keystuff.h
# End Source File
# Begin Source File

SOURCE=.\game\playerped.cpp
# End Source File
# Begin Source File

SOURCE=.\game\playerped.h
# End Source File
# Begin Source File

SOURCE=.\game\scripting.cpp
# End Source File
# Begin Source File

SOURCE=.\game\scripting.h
# End Source File
# Begin Source File

SOURCE=.\game\util.cpp
# End Source File
# Begin Source File

SOURCE=.\game\util.h
# End Source File
# Begin Source File

SOURCE=.\game\vehicle.cpp
# End Source File
# Begin Source File

SOURCE=.\game\vehicle.h
# End Source File
# End Group
# Begin Group "general"

# PROP Default_Filter ""
# Begin Group "d3d"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\d3d8\include\d3d8.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3d8caps.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3d8types.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\d3dfont.cpp
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3dfont.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3dx8.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3dx8core.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3dx8effect.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3dx8math.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3dx8mesh.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3dx8shape.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\d3dx8tex.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\dinput.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\dxfile.h
# End Source File
# Begin Source File

SOURCE=.\d3d8\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\d3d8\include\dxutil.h
# End Source File
# Begin Source File

SOURCE=.\d3dhook\IDirect3D8Hook.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dhook\IDirect3D8Hook.h
# End Source File
# Begin Source File

SOURCE=.\d3dhook\IDirect3DDevice8Hook.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dhook\IDirect3DDevice8Hook.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\anticheat.cpp
# End Source File
# Begin Source File

SOURCE=.\anticheat.h
# End Source File
# Begin Source File

SOURCE=.\bbfont.cpp
# End Source File
# Begin Source File

SOURCE=.\bbfont.h
# End Source File
# Begin Source File

SOURCE=.\chatwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\chatwindow.h
# End Source File
# Begin Source File

SOURCE=.\cmdprocs.cpp
# End Source File
# Begin Source File

SOURCE=.\cmdwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\cmdwindow.h
# End Source File
# Begin Source File

SOURCE=.\exceptbox.rc
# End Source File
# Begin Source File

SOURCE=.\exceptions.cpp
# End Source File
# Begin Source File

SOURCE=.\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\netstats.cpp
# End Source File
# Begin Source File

SOURCE=.\netstats.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\scoreboard.cpp
# End Source File
# Begin Source File

SOURCE=.\scoreboard.h
# End Source File
# Begin Source File

SOURCE=.\screenshot.cpp
# End Source File
# Begin Source File

SOURCE=.\screenshot.h
# End Source File
# Begin Source File

SOURCE=.\subclass.cpp
# End Source File
# End Group
# Begin Group "net"

# PROP Default_Filter ""
# Begin Group "gamemode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gamemode\generic.cpp
# End Source File
# Begin Source File

SOURCE=.\gamemode\generic.h
# End Source File
# End Group
# Begin Group "raknet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\raknet\AES128.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\AsynchronousFileIO.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\AsynchronousFileIO.h
# End Source File
# Begin Source File

SOURCE=..\raknet\BitStream.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\CheckSum.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\CheckSum.h
# End Source File
# Begin Source File

SOURCE=..\raknet\DataBlockEncryptor.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObject.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObject.h
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectHeader.h
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectManager.h
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectStub.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\DistributedNetworkObjectStub.h
# End Source File
# Begin Source File

SOURCE=..\raknet\EncodeClassName.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\EncodeClassName.h
# End Source File
# Begin Source File

SOURCE=..\raknet\ExtendedOverlappedPool.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\ExtendedOverlappedPool.h
# End Source File
# Begin Source File

SOURCE=..\raknet\GetTime.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\GetTime.h
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTree.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTree.h
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTreeFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTreeFactory.h
# End Source File
# Begin Source File

SOURCE=..\raknet\HuffmanEncodingTreeNode.h
# End Source File
# Begin Source File

SOURCE=..\raknet\InternalPacketPool.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\Multiplayer.h
# End Source File
# Begin Source File

SOURCE=..\raknet\NetworkObject.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\NetworkObject.h
# End Source File
# Begin Source File

SOURCE=..\raknet\NetworkTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\PacketPool.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\PacketPool.h
# End Source File
# Begin Source File

SOURCE=..\raknet\Queue.h
# End Source File
# Begin Source File

SOURCE=..\raknet\RakClient.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakNetStatistics.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakNetworkFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakPeer.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakServer.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RakServer.h
# End Source File
# Begin Source File

SOURCE=..\raknet\RakServerInterface.h
# End Source File
# Begin Source File

SOURCE=..\raknet\Rand.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\ReliabilityLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\RPCNode.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\SHA1.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\SimpleMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\SocketLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\StringCompressor.cpp
# End Source File
# Begin Source File

SOURCE=..\raknet\StringCompressor.h
# End Source File
# Begin Source File

SOURCE=..\raknet\Types.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\net\localplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\net\localplayer.h
# End Source File
# Begin Source File

SOURCE=.\net\netgame.cpp
# End Source File
# Begin Source File

SOURCE=.\net\netgame.h
# End Source File
# Begin Source File

SOURCE=.\net\netrpc.cpp
# End Source File
# Begin Source File

SOURCE=.\net\netrpc.h
# End Source File
# Begin Source File

SOURCE=.\net\playerpool.cpp
# End Source File
# Begin Source File

SOURCE=.\net\playerpool.h
# End Source File
# Begin Source File

SOURCE=.\net\remoteplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\net\remoteplayer.h
# End Source File
# Begin Source File

SOURCE=.\net\vehiclepool.cpp
# End Source File
# Begin Source File

SOURCE=.\net\vehiclepool.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# End Target
# End Project
