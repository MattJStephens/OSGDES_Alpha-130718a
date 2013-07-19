//////////////////////////////////////////////////////////////////////
//	MJim - Alpha
//	Stephens Atomics 
//	Copyright (C) 2013  Matthew James Stephens, B.Eng- Robotics(Hons)
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	<http://www.gnu.org/licenses/>   
//////////////////////////////////////////////////////////////////////

#define NOMINMAX


#include "MJS_Sockets.h"

//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iphlpapi.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string>
//#include <iostream>
//#include <sys/types.h>

#include "OSG_DES.h"
#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"
#include "OSG_XML_Save_environment_tree.h"


extern CEventHandler g_event_handler;

//Server
//
//
//1.Initialize Winsock.
//2.Create a socket.
//3.Bind the socket.
//4.Listen on the socket for a client.
//5.Accept a connection from a client.
//6.Receive and send data.
//7.Disconnect.
//
//Client
//
//
//1.Initialize Winsock.
//2.Create a socket.
//3.Connect to the server.
//4.Send and receive data.
//5.Disconnect.

bool CEvent_Socket::Create(CEventDesign* p_design, CEnvironment* p_env)
{	 
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{
		if(iter->m_identifier == "Type")
		{
			m_type = iter->m_str_value;
		}
		else if(iter->m_identifier == "Socket")
		{
			m_socket_index = p_env->index_from_name(iter->m_str_value, p_env->mv_socket);
		}

		
	}
	return true;
}
bool CEvent_Socket::Update(CEnvironment* p_env)
{	
	if(m_socket_index!= ULONG_MAX)
	{
		if("ServerEchoString" == m_type)
		{
			std::string str;
			receive_socket(p_env->mv_socket[m_socket_index].m_pSocket, str);
			std::ostringstream oss;
			oss << "Server recieves: " << str << "\n";			
			p_env->mv_socket[m_socket_index].m_str_data = oss.str();
		}
		if("ClientSendElapsedTime" == m_type)
		{
			double f = p_env->get_elapsed_real_time();
			std::ostringstream oss;
			oss << f;
			std::string str= oss.str();
			send_socket(p_env->mv_socket[m_socket_index].m_pSocket, str);			
			std::ostringstream oss2;
			oss2 << "Client sends: " << str << "\n";
			p_env->mv_socket[m_socket_index].m_str_data = oss2.str();
		}
	}
    return 0;
}



/* 
   EchoServer.cpp

   Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/


//#include "Socket.h"
//#include <process.h>
//#include <string>
//
//unsigned __stdcall Answer(void* a) {
//  Socket* s = (Socket*) a;
//
//  while (1) {
//    std::string r = s->ReceiveLine();
//    if (r.empty()) break;
//    s->SendLine(r);
//  }
//
//  delete s;
//
//  return 0;
//}
//
//int main(int argc, char* argv[]) {
//  SocketServer in(2000,5);
//
//  while (1) {
//    Socket* s=in.Accept();
//
//    unsigned ret;
//    _beginthreadex(0,0,Answer,(void*) s,0,&ret);
//  }
// 
//  return 0;
//}


 //WSADATA wsd;

    //struct addrinfo *result = NULL;
    //struct addrinfo hints;
    //WSAOVERLAPPED SendOverlapped;

    //SOCKET ListenSocket = INVALID_SOCKET;
    //SOCKET AcceptSocket = INVALID_SOCKET;

    //WSABUF DataBuf;
    //DWORD SendBytes;
    //DWORD Flags;

    //char buffer[DATA_BUFSIZE];

    //int err = 0;
    //int rc, i;

    //// Load Winsock
    //rc = WSAStartup(MAKEWORD(2, 2), &wsd);
    //if (rc != 0) {
    //    printf("Unable to load Winsock: %d\n", rc);
    //    return 1;
    //}

    //// Make sure the hints struct is zeroed out
    //SecureZeroMemory((PVOID) & hints, sizeof(struct addrinfo));

    //// Initialize the hints to obtain the 
    //// wildcard bind address for IPv4
    //hints.ai_family = AF_INET;
    //hints.ai_socktype = SOCK_STREAM;
    //hints.ai_protocol = IPPROTO_TCP;
    //hints.ai_flags = AI_PASSIVE;

    //rc = getaddrinfo(NULL, "27015", &hints, &result);
    //if (rc != 0) {
    //    printf("getaddrinfo failed with error: %d\n", rc);
    //    return 1;
    //}

    //ListenSocket = socket(result->ai_family,
    //                      result->ai_socktype, result->ai_protocol);
    //if (ListenSocket == INVALID_SOCKET) {
    //    printf("socket failed with error: %d\n", WSAGetLastError());
    //    freeaddrinfo(result);
    //    return 1;
    //}

    //rc = bind(ListenSocket, result->ai_addr, (int) result->ai_addrlen);
    //if (rc == SOCKET_ERROR) {
    //    printf("bind failed with error: %d\n", WSAGetLastError());
    //    freeaddrinfo(result);
    //    closesocket(ListenSocket);
    //    return 1;
    //}

    //rc = listen(ListenSocket, 1);
    //if (rc == SOCKET_ERROR) {
    //    printf("listen failed with error: %d\n", WSAGetLastError());
    //    freeaddrinfo(result);
    //    closesocket(ListenSocket);
    //    return 1;
    //}
    //// Accept an incoming connection request
    //AcceptSocket = accept(ListenSocket, NULL, NULL);
    //if (AcceptSocket == INVALID_SOCKET) {
    //    printf("accept failed with error: %d\n", WSAGetLastError());
    //    freeaddrinfo(result);
    //    closesocket(ListenSocket);
    //    return 1;
    //}

    //printf("Client Accepted...\n");

    //// Make sure the SendOverlapped struct is zeroed out
    //SecureZeroMemory((PVOID) & SendOverlapped, sizeof (WSAOVERLAPPED));

    //// Create an event handle and setup the overlapped structure.
    //SendOverlapped.hEvent = WSACreateEvent();
    //if (SendOverlapped.hEvent == NULL) {
    //    printf("WSACreateEvent failed with error: %d\n", WSAGetLastError());
    //    freeaddrinfo(result);
    //    closesocket(ListenSocket);
    //    closesocket(AcceptSocket);
    //    return 1;
    //}

    //DataBuf.len = DATA_BUFSIZE;
    //DataBuf.buf = buffer;

    //for (i = 0; i < SEND_COUNT; i++) {

    //    rc = WSASend(AcceptSocket, &DataBuf, 1,
    //                 &SendBytes, 0, &SendOverlapped, NULL);
    //    if ((rc == SOCKET_ERROR) &&
    //        (WSA_IO_PENDING != (err = WSAGetLastError()))) {
    //        printf("WSASend failed with error: %d\n", err);
    //        break;
    //    }

    //    rc = WSAWaitForMultipleEvents(1, &SendOverlapped.hEvent, TRUE, INFINITE,
    //                                  TRUE);
    //    if (rc == WSA_WAIT_FAILED) {
    //        printf("WSAWaitForMultipleEvents failed with error: %d\n",
    //               WSAGetLastError());
    //        break;
    //    }

    //    rc = WSAGetOverlappedResult(AcceptSocket, &SendOverlapped, &SendBytes,
    //                                FALSE, &Flags);
    //    if (rc == FALSE) {
    //        printf("WSASend failed with error: %d\n", WSAGetLastError());
    //        break;
    //    }

    //    printf("Wrote %d bytes\n", SendBytes);

    //    WSAResetEvent(SendOverlapped.hEvent);

    //}

    //WSACloseEvent(SendOverlapped.hEvent);
    //closesocket(AcceptSocket);
    //closesocket(ListenSocket);
    //freeaddrinfo(result);

    //WSACleanup();