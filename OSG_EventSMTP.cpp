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
#include "OSG_DES.h"
#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"
#include "OSG_XML_Save_environment_tree.h"
#include <sstream>

extern CEventHandler g_event_handler;

bool CEvent_SMTP::Create(CEventDesign* p_design, CEnvironment* p_env)
{		
	m_VRFY = false;
	m_transport_layer_security = false;
	m_str_from = "matthewstephens@hotmail.com";
	m_str_to = "matthewstephens@hotmail.com";
	m_str_data = "some text";
		
	//m_host_id= "smtp.gmail.com";//587	
	//m_host_id= "smtp.gmail.com";//465	
	//m_host_id= "smtp.live.com";//25
	//m_host_id= "smtp.aol.com";//587
	//m_host_id= "plus.smtp.mail.yahoo.com";//465
	m_host_id= "localhost";//465

	m_port = 25;

	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter)
	{
		if(iter->m_identifier == "HostID")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_host_id;					
		}	
		else if(iter->m_identifier == "Port")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_port;					
		}	
		
		else if(iter->m_identifier == "From")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_str_from;					
		}		
			
		else if(iter->m_identifier == "To")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_str_to;					
		}		

		else if(iter->m_identifier == "Data")
		{
			std::istringstream iss(iter->m_str_value);
			iss >> m_str_data;					
		}		
	}

	return true;
}
bool CEvent_SMTP::Update(CEnvironment* p_env)
{	 
	std::string buf;
	std::ostringstream oss;
	WSADATA wsaData;	
	SOCKET pSocket= INVALID_SOCKET;  
	LPHOSTENT lpHostEntry= NULL;
    LPSERVENT lpServEntry= NULL;
	SOCKADDR_IN SockAddr;  	

	if(!initialize_Winsock(&wsaData)) { return false; }		
	if(!create_socket(pSocket, PF_INET, SOCK_STREAM, 0)) { return false; }		
	if(!get_remote_host(m_host_id.c_str(), lpHostEntry)) { return false; }
	if(!get_service("mail", 0, lpServEntry)) { return false; }	
	int iProtocolPort = htons(m_port);
	
	if(m_transport_layer_security) {
		unsigned long ul = 1;
		if(ioctlsocket(pSocket, FIONBIO, (unsigned long*)&ul) == SOCKET_ERROR) { return false; }
	}	
	
	SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = iProtocolPort;
    //SockAddr.sin_addr = *((LPIN_ADDR) *(lpHostEntry->h_addr_list));
	memcpy(&SockAddr.sin_addr, lpHostEntry->h_addr_list[0], lpHostEntry->h_length);
	if(!connect_socket(pSocket, SockAddr)) { return false; }	

    receive_socket(pSocket, buf); 
    send_socket(pSocket, "HELO\r\n"); 
    receive_socket(pSocket, buf);  

	if(m_transport_layer_security) {	
		//incomplete..
		send_socket(pSocket, "STARTTLS\r\n"); 
		receive_socket(pSocket, buf); 
	}

	
	oss.clear();
	oss << "MAIL FROM:<" << m_str_from << ">\r\n";    
	send_socket(pSocket, oss.str());
    receive_socket(pSocket, buf); 
	
	if(m_VRFY) 	{
		oss.clear();
		oss << "VRFY <" << m_str_from << ">\r\n"; 	
		send_socket(pSocket, oss.str());
		receive_socket(pSocket, buf);   		
	}

	oss.clear();
	oss << "RCPT TO:<" << m_str_to << ">\r\n"; 	
	send_socket(pSocket, oss.str());
    receive_socket(pSocket, buf);

	oss.clear();
	oss << "DATA" << "\r\n"; 	
	send_socket(pSocket, oss.str());
    receive_socket(pSocket, buf);  
	
	oss.clear();
	oss << m_str_data << "\r\n\r\n"; 	
	send_socket(pSocket, oss.str());
    receive_socket(pSocket, buf); 

    send_socket(pSocket, "QUIT\r\n"); 
    receive_socket(pSocket, buf);

    closesocket(pSocket);
	WSACleanup();
	return true;
}






//DNS_RECORD* ppQueryResultsSet = NULL;

 //   // dns mx lookup
 //   DnsQuery(L"hotmail.com", DNS_TYPE_MX, DNS_QUERY_STANDARD, NULL, &ppQueryResultsSet, NULL);

 //   if (ppQueryResultsSet == NULL)
 //       return false;

 //   // i am getting the mx servers (i.e: mx3.hotmail.com)
 //   wcout << "MX Host name: " << ppQueryResultsSet->Data.MX.pNameExchange << endl;
 //   DnsRecordListFree(ppQueryResultsSet, DnsFreeRecordList);

 //   string host_name = String_Convertion::WStringToString(ppQueryResultsSet->Data.MX.pNameExchange);

 //   // setting up socket connection
 //   boost::asio::io_service io_service;
 //   tcp::resolver resolver(io_service);
 //   tcp::resolver::query query(host_name, "25");
 //   tcp::resolver::iterator endpoint_iterator = resolver.resolve(query), end;
 //   tcp::socket socket(io_service);

 //   // connecting to the server
 //   boost::system::error_code error_code_connect;
 //   boost::asio::connect(socket, endpoint_iterator, end, error_code_connect);
 //   if (error_code_connect)
 //   {
 //       cout << "Error connecting to SMTP Server" << endl;
 //       return false;
 //   }

 //   boost::system::error_code error;
 //   std::string temp_buf;
 //   boost::array<char, 128> buf;
 //   size_t len;

 //   // settings the packets to send
 //   list<string> packets;
 //   packets.push_back("HELO");
 //   packets.push_back("MAIL FROM: xxx@xxx.xxx");
 //   packets.push_back("VRFY xxx@xxx.xxx");
 //   packets.push_back("RCPT TO: x@hotmail.com"); 
 //   packets.push_back("DATA");
 //   packets.push_back("Subject: subject!");
 //   packets.push_back("hi how are you doing?.");
 //   packets.push_back("QUIT");
 //   list<string>::iterator it;
 //   // iterating to send the packets
 //   for (it = packets.begin(); it != packets.end(); it++)
 //   {
 //       socket.write_some(boost::asio::buffer(*it+"\r\n"));
 //       len = socket.read_some(boost::asio::buffer(buf), error);
 //       temp_buf = buf.data();
 //       temp_buf = temp_buf.substr(0, len);
 //       cout << temp_buf << endl;
 //   }