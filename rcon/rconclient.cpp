/*

	VC:MP Multiplayer Modification
	Copyright 2004-2005 SA:MP Team

	file:
		rconclient.cpp
	desc:
		Remote Console client.

*/

#include "main.h"

CRconClient::CRconClient(char* szHostOrIp, int iPort, char* szPass)
{
	m_pRak = RakNetworkFactory::GetRakClientInterface();
	m_pRak->SetPassword(szPass);
	if (!m_pRak->Connect(szHostOrIp, (WORD)iPort, (WORD)(iPort+1), 0, 5))
	{
		logprintf("Connection failed.");
	} else {
		logprintf("Connecting to %s:%d (%s)...", szHostOrIp, iPort, szPass);
	}
}

CRconClient::~CRconClient()
{
	m_pRak->Disconnect(100);
	RakNetworkFactory::DestroyRakClientInterface(m_pRak);
}

void CRconClient::Command(char* szCommand)
{
	if (!szCommand) return;

	BYTE bytePacketId = ID_RCON_COMMAND;
	RakNet::BitStream bsCommand;
	bsCommand.Write(bytePacketId);
	DWORD dwCmdLen = (DWORD)strlen(szCommand);
	bsCommand.Write(dwCmdLen);
	bsCommand.Write(szCommand, dwCmdLen);
	m_pRak->Send(&bsCommand, HIGH_PRIORITY, RELIABLE, 0);
}

void CRconClient::Process()
{
	if (!m_pRak)
		return;

	Packet* pPacket;
	while(pPacket = m_pRak->Receive())
	{
		switch (pPacket->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			Packet_ConnectionRequestAccepted(pPacket);
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			Packet_DisconnectionNotification(pPacket);
			break;
		case ID_CONNECTION_LOST:
			Packet_ConnectionLost(pPacket);
			break;
		case ID_CONNECTION_BANNED:
			Packet_ConnectionBanned(pPacket);
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			Packet_NoFreeIncomingConnections(pPacket);
			break;
		case ID_INVALID_PASSWORD:
			Packet_InvalidPassword(pPacket);
			break;
		case ID_RCON_RESPONSE:
			Packet_RconResponce(pPacket);
			break;
		}
		m_pRak->DeallocatePacket(pPacket);		
	}
}

void CRconClient::Packet_ConnectionRequestAccepted(Packet* pPacket)
{
	logprintf("*** Remote Console connected.\n");
}

void CRconClient::Packet_DisconnectionNotification(Packet* pPacket)
{
	logprintf("*** Remote Console disconnected. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_ConnectionLost(Packet* pPacket)
{
	logprintf("*** Remote Console connection lost. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_ConnectionBanned(Packet* pPacket)
{
	logprintf("*** Remote Console unable to connect. You are banned. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_NoFreeIncomingConnections(Packet* pPacket)
{
	logprintf("*** Unable to connect to Remote Console. Max admin slots filled. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_InvalidPassword(Packet* pPacket)
{
	logprintf("*** Invalid Remote Console password. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_RconResponce(Packet* pPacket)
{
	DWORD dwResponceLen;
	memcpy(&dwResponceLen, &pPacket->data[1], 4);
	char* szResponce = (char*)malloc(dwResponceLen+1);
	memcpy(szResponce, &pPacket->data[5], dwResponceLen);
	szResponce[dwResponceLen] = 0;

	logprintf(szResponce);

	free(szResponce);
}