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

#include "MJS_Sockets.h"

std::string connect_socket_error_to_string(int error_code)
{
	switch (error_code)
	{
		case WSANOTINITIALISED:
		{
			return "WSANOTINITIALISED - A successful WSAStartup call must occur before using this function";
			break;
		}
		case WSAENETDOWN:
		{
			return "WSAENETDOWN - The network subsystem has failed";
			break;
		}
		case WSAEADDRINUSE:
		{
			return "WSAEADDRINUSE - The socket's local address is already in use and the socket was not marked to allow address reuse ";
			break;
		}
		case WSAEINTR:
		{
			return "WSAEINTR - The blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall";
			break;
		}
		case WSAEINPROGRESS:
		{
			return "WSAEINPROGRESS - A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function";
			break;
		}
		case WSAEALREADY:
		{
			return "WSAEALREADY - A nonblocking connect call is in progress on the specified socket";
			break;
		}
		case WSAEADDRNOTAVAIL:
		{
			return "WSAEADDRNOTAVAIL - The remote address is not a valid address (such as INADDR_ANY or in6addr_any)";
			break;
		}	
		case WSAEAFNOSUPPORT:
		{
			return "WSAEAFNOSUPPORT - Addresses in the specified family cannot be used with this socket";
			break;
		}		
		case WSAECONNREFUSED:
		{
			return "WSAECONNREFUSED - The attempt to connect was forcefully rejected";
			break;
		}			
		case WSAEFAULT:
		{
			return "WSAEFAULT - The sockaddr structure pointed to by the name contains incorrect address format for the associated address family or the namelen parameter is too small";
			break;
		}
		case WSAEINVAL:
		{
			return "WSAEINVAL -The parameter s is a listening socket ";
			break;
		}			
		case WSAEISCONN:
		{
			return "WSAEISCONN - The socket is already connected (connection-oriented sockets only).";
			break;
		}
		case WSAENETUNREACH:
		{
			return "WSAENETUNREACH - The network cannot be reached from this host at this time.";
			break;
		}
		case WSAEHOSTUNREACH:
		{
			return "WSAEHOSTUNREACH - A socket operation was attempted to an unreachable host.";
			break;
		}
		case WSAENOBUFS:
		{
			return "WSAENOBUFS - No buffer space is available. The socket cannot be connected.";
			break;
		}
		case WSAENOTSOCK:
		{
			return "WSAENOTSOCK - The descriptor specified in the s parameter is not a socket.";
			break;
		}
		case WSAETIMEDOUT:
		{
			return "WSAETIMEDOUT - An attempt to connect timed out without establishing a connection.";
			break;
		}
		case WSAEWOULDBLOCK:
		{
			return "WSAEWOULDBLOCK -The socket is marked as nonblocking and the connection cannot be completed immediately. ";
			break;
		}
		case WSAEACCES:
		{
			return "WSAEACCES - An attempt to connect a datagram socket to broadcast address failed because setsockopt option SO_BROADCAST is not enabled.";
			break;
		}
	}
	return "unknown";
}

std::string service_error_to_string(int error_code)
{
	switch (error_code)
	{
		case WSANOTINITIALISED:
		{
			return "WSANOTINITIALISED - A successful WSAStartup call must occur before using this function";
			break;
		}
		case WSAENETDOWN:
		{
			return "WSAENETDOWN - The network subsystem has failed.";
			break;
		}
		case WSAHOST_NOT_FOUND:
		{
			return "WSAHOST_NOT_FOUND - Authoritative Answer Service not found.";
			break;
		}
		case WSATRY_AGAIN:
		{
			return "WSATRY_AGAIN - A nonauthoritative Service not found, or server failure.";
			break;
		}
		case WSANO_RECOVERY:
		{
			return "WSANO_RECOVERY - Nonrecoverable errors, the services database is not accessible.";
			break;
		}
		case WSANO_DATA:
		{
			return "WSANO_DATA - Valid name, no data record of requested type.";
			break;
		}
		case WSAEINPROGRESS:
		{
			return "WSAEINPROGRESS - A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";
			break;
		}
		case WSAEINTR:
		{
			return "WSAEINTR - A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall.";
			break;
		}
	}
	return "undefined";
}

std::string create_socket_error_to_string(int error_code)
{
	switch (error_code)
	{
		case WSANOTINITIALISED:
		{
			return "WSANOTINITIALISED - A successful WSAStartup call must occur before using this function";
			break;
		}
		case WSAENETDOWN:
		{
			return "WSAENETDOWN - The network subsystem has failed.";
			break;
		}
		case WSAEAFNOSUPPORT:
		{
			return "WSAEAFNOSUPPORT - The specified address family is not supported";
			break;
		}
		case WSAEINPROGRESS:
		{
			return "WSAEINPROGRESS - A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";
			break;
		}
		case WSAEMFILE:
		{
			return "WSAEMFILE - No more socket descriptors are available.";
			break;
		}		
		case WSAEINVAL:
		{
			return "WSAEINVAL - An invalid argument was supplied. This error is returned if the af parameter is set to AF_UNSPEC and the type and protocol parameter are unspecified.";
			break;
		}
		case WSAENOBUFS:
		{
			return "WSAENOBUFS - No buffer space is available. The socket cannot be created.";
			break;
		}		
		case WSAEPROTONOSUPPORT:
		{
			return "WSAEPROTONOSUPPORT - The specified protocol is not supported.";
			break;
		}		
		case WSAEPROTOTYPE:
		{
			return "WSAEPROTOTYPE - The specified protocol is the wrong type for this socket. ";
			break;
		}
		case WSAEPROVIDERFAILEDINIT:
		{
			return "WSAEPROVIDERFAILEDINIT - The service provider failed to initialize. ";
			break;
		}
		case WSAESOCKTNOSUPPORT:
		{
			return "WSAESOCKTNOSUPPORT - The specified socket type is not supported in this address family.";
			break;
		}
		/*case WSAINVALIDPROVIDER:
		{
			return "WSAINVALIDPROVIDER - ";
			break;
		}
		case WSAINVALIDPROCTABLE:
		{
			return "WSAINVALIDPROCTABLE - ";
			break;
		}*/
	}
	return "undefined";
}

std::string getaddrinfo_error_to_string(int error_code)
{
	switch (error_code)
	{
		case EAI_AGAIN:
		{
			return "EAI_AGAIN - A temporary failure in name resolution occurred.";
			break;
		}
		case EAI_BADFLAGS:
		{
			return "EAI_BADFLAGS - An invalid value was provided for the ai_flags member of the pHints parameter.";
			break;
		}
		case EAI_FAIL:
		{
			return "EAI_FAIL - A nonrecoverable failure in name resolution occurred.";
			break;
		}
		case EAI_FAMILY:
		{
			return "EAI_FAMILY - The ai_family member of the pHints parameter is not supported.";
			break;
		}
		case EAI_MEMORY:
		{
			return "EAI_MEMORY - A memory allocation failure occurred.";
			break;
		}
		case EAI_NONAME:
		{
			return "EAI_NONAME - The name does not resolve for the supplied parameters or the pNodeName and pServiceName parameters were not provided.";
			break;
		}
		case EAI_SERVICE:
		{
			return "EAI_SERVICE - he pServiceName parameter is not supported for the specified ai_socktype member of the pHints parameter.";
			break;
		}
		case EAI_SOCKTYPE:
		{
			return "EAI_SOCKTYPE - The ai_socktype member of the pHints parameter is not supported.";
			break;
		}		
	}
	return "undefined";
}

int string_to_socket_flags(std::string str)
{
	if(str == "0") { return 0; }
	if(str == "AI_PASSIVE") { return AI_PASSIVE; }
	else if(str == "AI_CANONNAME") { return AI_CANONNAME; }
	else if(str == "AI_NUMERICHOST") { return AI_NUMERICHOST; }
	else if(str == "AI_ADDRCONFIG") { return AI_ADDRCONFIG; }
	else if(str == "AI_NON_AUTHORITATIVE") { return AI_NON_AUTHORITATIVE; }
	else if(str == "AI_SECURE") { return AI_SECURE; }
	else if(str == "AI_RETURN_PREFERRED_NAMES") { return AI_RETURN_PREFERRED_NAMES; }
	return 0;
}

int string_to_socket_address_family(std::string str)
{	
	if(str == "0") { return 0; }
	else if(str == "AF_UNSPEC") { return AF_UNSPEC; }
	else if(str == "AF_INET") { return AF_INET; }
	else if(str == "AF_IPX") { return AF_IPX; }
	else if(str == "AF_APPLETALK") { return AF_APPLETALK; }
	else if(str == "AF_NETBIOS") { return AF_NETBIOS; }
	else if(str == "AF_INET6") { return AF_INET6; }
	else if(str == "AF_IRDA") { return AF_IRDA; }//Infrared Data Association 
	else if(str == "AF_BTH") { return AF_BTH; }
	else if(str == "unspecified") { return AF_UNSPEC; }
	else if(str == "IPv4") { return AF_INET; }
	else if(str == "IPX/SPX") { return AF_IPX; }
	else if(str == "AppleTalk") { return AF_APPLETALK; }
	else if(str == "NetBIOS") { return AF_NETBIOS; }
	else if(str == "IPv6") { return AF_INET6; }
	else if(str == "IrDA") { return AF_IRDA; }//Infrared Data Association 
	else if(str == "Bluetooth") { return AF_BTH; }
	return 0;
}

int string_to_socket_type(std::string str)
{
	if(str == "0") { return 0; } 
	else if(str == "SOCK_STREAM") { return SOCK_STREAM; } //TCP
	else if(str == "SOCK_DGRAM") { return SOCK_DGRAM; } //User Datagram Protocol 
	else if(str == "SOCK_RAW") { return SOCK_RAW; } // Must set  IP_HDRINCL 
	else if(str == "SOCK_RDM") { return SOCK_RDM; } // Reliable Multicast Protocol must be installed
	else if(str == "SOCK_SEQPACKET") { return SOCK_SEQPACKET; } //pseudo-stream packet based on datagram
	else if(str == "Stream") { return SOCK_STREAM; } //TCP
	else if(str == "UDP") { return SOCK_DGRAM; } //User Datagram Protocol 
	else if(str == "Raw") { return SOCK_RAW; } // Must set  IP_HDRINCL 
	else if(str == "RDM") { return SOCK_RDM; } // Reliable Multicast Protocol must be installed
	else if(str == "pseudo-stream") { return SOCK_SEQPACKET; } //pseudo-stream packet based on datagram
	return 0;
}

int string_to_socket_protocol(std::string str)
{	
	if(str == "0") { return 0; }
	else if(str == "unspecified") { return 0; }
	else if(str == "IPPROTO_ICMP") { return IPPROTO_ICMP; }
	else if(str == "IPPROTO_IGMP") { return IPPROTO_IGMP; }
	else if(str == "IPPROTO_TCP") { return IPPROTO_TCP; }
	else if(str == "IPPROTO_UDP") { return IPPROTO_UDP; }
	else if(str == "IPPROTO_ICMPV6") { return IPPROTO_ICMPV6; }

	//The Internet Control Message Protocol (ICMP). 
	//This is a possible value when the af parameter is AF_UNSPEC, AF_INET, or AF_INET6 
	//and the type parameter is SOCK_RAW or unspecified.
	//This protocol value is supported on Windows XP and later.
	else if(str == "ICMP") { return IPPROTO_ICMP; }

	//The Internet Group Management Protocol (IGMP). 
	//This is a possible value when the af parameter is AF_UNSPEC, AF_INET, or AF_INET6 
	//and the type parameter is SOCK_RAW or unspecified.
	//This protocol value is supported on Windows XP and later.
	else if(str == "IGMP") { return IPPROTO_IGMP; }

	//The Bluetooth Radio Frequency Communications (Bluetooth RFCOMM) protocol. 
	//This is a possible value when the af parameter is AF_BTH and the type parameter is SOCK_STREAM. 
	//This protocol value is supported on Windows XP with SP2 or later.
	//else if(str == "Bluetooth") { return BTHPROTO_RFCOMM; }

	//The Transmission Control Protocol (TCP). 
	//This is a possible value when the af parameter is AF_INET or AF_INET6 and the type parameter is SOCK_STREAM.
	else if(str == "TCP") { return IPPROTO_TCP; }

	//The User Datagram Protocol (UDP). 
	//This is a possible value when the af parameter is AF_INET or AF_INET6 and the type parameter is SOCK_DGRAM.
	else if(str == "ICMP") { return IPPROTO_UDP; }

	//The Internet Control Message Protocol Version 6 (ICMPv6). 
	//This is a possible value when the af parameter is AF_UNSPEC, AF_INET, or AF_INET6 
	//and the type parameter is SOCK_RAW or unspecified.
	//This protocol value is supported on Windows XP and later.

	else if(str == "ICMPV6") { return IPPROTO_ICMPV6; }

	//The PGM protocol for reliable multicast. 
	//This is a possible value when the af parameter is AF_INET 
	//and the type parameter is SOCK_RDM.
	//On the Windows SDK released for Windows Vista and later, this protocol is also called IPPROTO_PGM. 
	//This protocol value is only supported if the Reliable Multicast Protocol is installed.
	//else if(str == "ICMP") { return IPPROTO_RM; }
	return 0;
}

bool initialize_Winsock(WSADATA* p_wsaData)
{
	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h 
	WORD wVersionRequested = MAKEWORD(2,2);
	int iResult = WSAStartup(wVersionRequested, p_wsaData);
	if (iResult != 0) {
		std::string str_error = "WSAStartup failed: %d\n";		
		std::cout << str_error;
		//MessageBox(NULL, str_error.c_str(), "error", MB_ICONINFORMATION | MB_OK);
		return false;
	}

	// Confirm that the WinSock DLL supports 2.2.
	// Note that if the DLL supports versions greater    
	// than 2.2 in addition to 2.2, it will still return 
	// 2.2 in wVersion since that is the version we      
	// requested.               
	if (LOBYTE(p_wsaData->wVersion) != 2 || HIBYTE(p_wsaData->wVersion) != 2) {
		std::string str_error = "Could not find a usable version of Winsock.dll\n";		
		std::cout << str_error;
		//MessageBox(NULL, str_error.c_str(), "error", MB_ICONINFORMATION | MB_OK);
		WSACleanup();
		return false;
	}

	//WSACleanup();
	return true;
}


bool get_remote_host(const char * hname, LPHOSTENT& lpHostEntry)
{
	lpHostEntry = gethostbyname(hname);
	if(!lpHostEntry) 
	{	
		std::string str_error = "unknown host";		
		std::cout << str_error;
		//MessageBox(NULL, str_error.c_str(), "error", MB_ICONINFORMATION | MB_OK);
		WSACleanup();
		return false;
	}
	return true;
}

bool get_service(const char * name, const char * protocol, LPSERVENT& lpServEntry)
{
	lpServEntry= getservbyname(name, protocol);
	if(!lpServEntry)  {  
		int error_code = WSAGetLastError();
		std::string str_error = "could not get service\n";		
		std::cout << service_error_to_string(error_code);
		//iProtocolPort = htons(IPPORT_SMTP);
	}
	//else { iProtocolPort = lpServEntry->s_port; }
	return true;
}

bool create_socket(SOCKET& pSocket, int af, int type, int protocol)
{
	pSocket = socket(af, type, protocol);
    if (pSocket == INVALID_SOCKET) {	
		int error_code = WSAGetLastError();
		std::string str_error = "could not create socket";		
		std::cout << str_error << "\n";
		std::cout << create_socket_error_to_string(error_code);	
		WSACleanup();

		return false;
    }
	return true;
}

bool create_socket(SOCKET& pSocket, ADDRINFOA *p_addrinfo)
{
	return create_socket(pSocket, p_addrinfo->ai_family, p_addrinfo->ai_socktype, p_addrinfo->ai_protocol);   
}

bool create_socket(SOCKET& pSocket, struct addrinfo *p_addrinfo, int socket_flags, int socket_address_family, int socket_type, int socket_protocol, std::string node_name, std::string service_name)
{	
	if(node_name == "INADDR_ANY")
	{			
		return create_socket(pSocket, socket_address_family, socket_type, socket_protocol);		
	}	
	else
	{	
		struct addrinfo hints;
		ZeroMemory(&hints, sizeof (hints));
		hints.ai_flags = socket_flags;
		hints.ai_family = socket_address_family;
		hints.ai_socktype = socket_type;
		hints.ai_protocol = socket_protocol;
	
		// Resolve the local address and port to be used
		// node_name host (node) name or a numeric host address string
		// service name or port number
		// A service name is a string alias for a port number.
		// For example, “http” is an alias for port 80 defined by the Internet Engineering Task Force (IETF) as the default port used by web servers for the HTTP protocol. 
		int iResult;	
		if(("" == node_name.c_str())&&("" == service_name.c_str())) { iResult = getaddrinfo(NULL, NULL, &hints, &p_addrinfo); }
		else if(("NULL" == node_name.c_str())&&("NULL" == service_name.c_str())) { iResult = getaddrinfo(NULL, NULL, &hints, &p_addrinfo); }
		else if("NULL" == node_name.c_str()) { iResult = getaddrinfo(NULL, service_name.c_str(), &hints, &p_addrinfo); }
		else if("NULL" == service_name.c_str()) { iResult = getaddrinfo(node_name.c_str(), NULL, &hints, &p_addrinfo); }
		else { iResult = getaddrinfo(node_name.c_str(), service_name.c_str(), &hints, &p_addrinfo); }
		if (iResult != 0) {
			int error_code = WSAGetLastError();
			std::string str_error = "getaddrinfo failed";		
			std::cout << str_error << "\n";
			std::cout << getaddrinfo_error_to_string(error_code);	
			WSACleanup();
			return false;
		}	
	}
	return create_socket(pSocket, p_addrinfo->ai_family, p_addrinfo->ai_socktype, p_addrinfo->ai_protocol);	
}

//A pointer to a NULL-terminated ANSI string that contains a host (node) name 
//or a numeric host address string. For the Internet protocol,
//the numeric host address string is a dotted-decimal IPv4 address or an IPv6 hex address.
bool get_host_address_information(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA *pHints, PADDRINFOA *ppResult)
{
	// Resolve the server address and port
	//The getaddrinfo function provides protocol-independent translation from an ANSI host name to an address.
	int iResult = getaddrinfo(pNodeName, pServiceName, pHints, ppResult);
	if (iResult != 0) {
		std::string str_error = "could not create socket";		
		std::cout << str_error << "\n";
		WSACleanup();
		return false;
	}
	return true;
}
//if (
//    IN6_IS_ADDR_V4MAPPED(&(ipv6_clientdata->sin6_addr)) &&
//    (ipv6_clientdata->sin6_port == ipv4_storeddata->sin_port) &&
//    (ipv6_clientdata->sin6_addr.in6_u.u6_addr32[3] == ipv4_storeddata->sin_addr.s_addr)
//) {
//    addrfound = true;
//}
char* get_local_IP(void)
{
	hostent* localHost;
	char* localIP;	
	localHost = gethostbyname("");
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

	return localIP;
}

unsigned long string_to_IPv4(std::string str)
{
	return inet_addr(str.c_str());
}

u_short u_short_to_TCP_IP(u_short port)
{
	//return htons(INADDR_ANY);
	return htons(port);
}

   //bzero((char *) &serv_addr, sizeof(serv_addr));
   // serv_addr.sin_family = AF_INET;
   // bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   // serv_addr.sin_port = htons(portno);
   // if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
   //     error("ERROR connecting");




   //struct sockaddr { 
   //     unsigned short    sa_family;    // address family, AF_xxx 
   //     char              sa_data[14];  // 14 bytes of protocol address 
   // };  


 //struct sockaddr_in { 
 //       short int          sin_family;  // Address family 
 //       unsigned short int sin_port;    // Port number 
 //       struct in_addr     sin_addr;    // Internet address 
 //       unsigned char      sin_zero[8]; // Same size as struct sockaddr 
 //   };  

 // Internet address (a structure for historical reasons) 
    //struct in_addr { 
    //    unsigned long s_addr; // that's a 32-bit long, or 4 bytes 
    //};  


//Remember: put your bytes in Network Byte Order before you put them on the network. 


bool connect_socket(SOCKET& pSocket, SOCKADDR_IN& SockAddr)
{	
	int iResult = connect(pSocket, (LPSOCKADDR)&SockAddr, sizeof(SockAddr));
	if(iResult != 0) {
		int error_code = WSAGetLastError();
		std::string str_error = "could not establish socket connection";		
		std::cout << str_error << "\n";
		std::cout << connect_socket_error_to_string(error_code);	
		WSACleanup();
		return false;
	}
	return true;
}

bool connect_socket(SOCKET& pSocket, int iProtocolPort, LPHOSTENT& lpHostEntry)
{
	SOCKADDR_IN SockAddr;
	SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = iProtocolPort;
    SockAddr.sin_addr = *((LPIN_ADDR) *(lpHostEntry->h_addr_list));
	return connect_socket(pSocket, SockAddr);	
}

//////////
//bind address to socket
//////////
bool bind_socket(SOCKET& pSocket, sockaddr * p_sockaddr, int namelen)
{
	int iResult = bind(pSocket, p_sockaddr, namelen);
	if (iResult == SOCKET_ERROR) {

		int error_code = WSAGetLastError();
		std::string str_error = "bind failed";		
		std::cout << str_error << "\n";
		std::cout << connect_socket_error_to_string(error_code);	
		WSACleanup();	
		//freeaddrinfo(result);
		//closesocket(socket.m_pSocket);
		return false;
	}
	return true;
}

bool send_socket_psz(SOCKET& pSocket, char *s)
{
	int iResult = send(pSocket, s, strlen(s), 0);
	if(SOCKET_ERROR == iResult)	{
		std::string str_error = "could not send socket";		
		std::cout << str_error;		
		return false;
	}
	return true;
}

bool recieve_socket_psz(SOCKET& pSocket, char *s)
{
    int iResult = recv(pSocket, s, strlen(s), 0);
	if(SOCKET_ERROR == iResult)	{
		std::string str_error = "could not recieve socket";		
		std::cout << str_error;		
		return false;
	}
	return true;
}

bool send_socket(SOCKET& pSocket, std::string s)
{
	int iResult = send(pSocket, s.c_str(), s.length(), 0);
	if(SOCKET_ERROR == iResult)	{
		std::string str_error = "could not send socket";		
		std::cout << str_error;		
		return false;
	}
	return true;
}

bool receive_socket(SOCKET& pSocket, std::string& s)
{
	char *cstr = new char[SOCKET_DATA_BUFSIZE + 1];
	//strcpy_s(cstr, sizeof(cstr), s.c_str());

	ZeroMemory(cstr, SOCKET_DATA_BUFSIZE + 1);
    int iResult = recv(pSocket, cstr, SOCKET_DATA_BUFSIZE, 0);
	if(SOCKET_ERROR == iResult)	{
		std::string str_error = "could not recieve socket";		
		std::cout << str_error;		
		return false;
	}
	s = cstr;
	delete [] cstr;

	return true;
}

//
//// Receive data until the server closes the connection
//do {
//    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
//    if (iResult > 0)
//        printf("Bytes received: %d\n", iResult);
//    else if (iResult == 0)
//        printf("Connection closed\n");
//    else
//        printf("recv failed: %d\n", WSAGetLastError());
//} while (iResult > 0);
void initialize_host_address_information(ADDRINFOA *p_HAI, int family, int socktype, int protocol)
{
	ZeroMemory( p_HAI, sizeof(*p_HAI) );
	p_HAI->ai_family = family;
	p_HAI->ai_socktype = socktype;
	p_HAI->ai_protocol = protocol;
}

void update_HAI_with_string_values(ADDRINFOA *p_HAI, std::string str_socket_address_family, std::string str_socket_type, std::string str_socket_protocol)
{
	p_HAI->ai_family= string_to_socket_address_family(str_socket_address_family);
	p_HAI->ai_socktype= string_to_socket_type(str_socket_type);
	p_HAI->ai_protocol= string_to_socket_protocol(str_socket_protocol);	
}

#define ECHO_PORT (2002)
#define MAX_LINE (1000)
//#include <unistd.h>             /*  for ssize_t data type  */

#define LISTENQ        (1024)   /*  Backlog for listen()   */
//
//ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
//    ssize_t n, rc;
//    char    c, *buffer;
//
//    buffer = vptr;
//
//    for ( n = 1; n < maxlen; n++ ) {
//	
//	if ( (rc = read(sockd, &c, 1)) == 1 ) {
//	    *buffer++ = c;
//	    if ( c == '\n' )
//		break;
//	}
//	else if ( rc == 0 ) {
//	    if ( n == 1 )
//		return 0;
//	    else
//		break;
//	}
//	else {
//	    if ( errno == EINTR )
//		continue;
//	    return -1;
//	}
//    }
//
//    *buffer = 0;
//    return n;
//}
//
//
///*  Write a line to a socket  */
//
//ssize_t Writeline(int sockd, const void *vptr, size_t n) {
//    size_t      nleft;
//    ssize_t     nwritten;
//    const char *buffer;
//
//    buffer = vptr;
//    nleft  = n;
//
//    while ( nleft > 0 ) {
//	if ( (nwritten = write(sockd, buffer, nleft)) <= 0 ) {
//	    if ( errno == EINTR )
//		nwritten = 0;
//	    else
//		return -1;
//	}
//	nleft  -= nwritten;
//	buffer += nwritten;
//    }
//
//    return n;
//}



void simple_TCPIP()
{   
	int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */
	      
	port = ECHO_PORT;  

	
    /*  Create the listening socket  */

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the 
	listening socket, and call listen()  */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error calling bind()\n");
	exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error calling listen()\n");
	exit(EXIT_FAILURE);
    }

    
    /*  Enter an infinite loop to respond
        to client requests and echo input  */

    while ( 1 ) {

		/*  Wait for a connection, then accept() it  */

		if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
			fprintf(stderr, "ECHOSERV: Error calling accept()\n");
			exit(EXIT_FAILURE);
		}


		/*  Retrieve an input line from the connected socket
			then simply write it back to the same socket.     */

		//Readline(conn_s, buffer, MAX_LINE-1);
		//Writeline(conn_s, buffer, strlen(buffer));


		/*  Close the connected socket  */

		if ( closesocket(conn_s) < 0 ) {
			fprintf(stderr, "ECHOSERV: Error calling close()\n");
			exit(EXIT_FAILURE);
		}
	} 
}