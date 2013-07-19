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

#pragma once

//#define HELO "HELO 192.168.1.1\r\n"
//#define DATA "DATA\r\n"
//#define QUIT "QUIT\r\n"
//#define DEFAULT_PORT "27015"

#include <string>
#include <iostream>
#include <sys/types.h>
#include <stdio.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#define SOCKET_DATA_BUFSIZE 4096
//#define SEND_COUNT   10


std::string connect_socket_error_to_string(int error_code);
std::string service_error_to_string(int error_code);
std::string create_socket_error_to_string(int error_code);
std::string getaddrinfo_error_to_string(int error_code);

int string_to_socket_flags(std::string str);
int string_to_socket_address_family(std::string str);
int string_to_socket_type(std::string str);
int string_to_socket_protocol(std::string str);
bool initialize_Winsock(WSADATA* p_wsaData);
bool get_remote_host(const char * hname, LPHOSTENT& lpHostEntry);
bool get_service(const char * name, const char * protocol, LPSERVENT& lpServEntry);

bool create_socket(SOCKET& pSocket, int af, int type, int protocol);
bool create_socket(SOCKET& pSocket, ADDRINFOA *p_addrinfo);
bool create_socket(SOCKET& pSocket, struct addrinfo *p_addrinfo, int socket_flags, int socket_address_family, int socket_type, int socket_protocol, std::string node_name, std::string service_name);

bool get_host_address_information(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA *pHints, PADDRINFOA *ppResult);

//NOTE: struct sockaddr_in == SOCKADDR_IN
bool connect_socket(SOCKET& pSocket, SOCKADDR_IN& SockAddr);
bool connect_socket(SOCKET& pSocket, int iProtocolPort, LPHOSTENT& lpHostEntry);


bool bind_socket(SOCKET& pSocket, sockaddr * p_sockaddr, int namelen);

bool send_socket_psz(SOCKET& pSocket, char *s);
bool recieve_socket_psz(SOCKET& pSocket, char *s);

bool send_socket(SOCKET& pSocket, std::string s);
bool receive_socket(SOCKET& pSocket, std::string& s);

void initialize_host_address_information(ADDRINFOA *p_HAI, int family, int socktype, int protocol);
void update_HAI_with_string_values(ADDRINFOA *p_HAI, std::string str_socket_address_family, std::string str_socket_type, std::string str_socket_protocol);

char* get_local_IP(void);
unsigned long string_to_IPv4(std::string str);
u_short u_short_to_TCP_IP(u_short port);
