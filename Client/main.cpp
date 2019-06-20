// Std Includes
#include <string>
#include <iostream>
#include <sstream>

// Windows Includes
#include <winsock2.h>
#include <Ws2tcpip.h> // inet_pton

const USHORT PORT = 55555;
const const char* LOCALHOST = "127.0.0.1";

int main()
{
   // Hardcode a message
   std::string message = "Hello, World!";

   // Start WSAData
   WSADATA WSAData;
   WSAStartup(MAKEWORD(2, 2), &WSAData);

   // Setup the socket
   SOCKADDR_IN addr;
   SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
   // The call to inet_pton replaces the equivalent (deprecated) call: addr.sin_addr.s_addr = inet_addr(LOCALHOST);
   inet_pton(AF_INET, LOCALHOST, &addr.sin_addr.s_addr); 
   addr.sin_family = AF_INET;
   addr.sin_port = htons(PORT);

   // Connect to the server
   const int connect_result = connect(server, (SOCKADDR *)&addr, sizeof(addr));
   if (connect_result == SOCKET_ERROR)
   {
      throw std::runtime_error("Error connecting to server!");
   }
   std::cout << "Connected to server!" << std::endl;

   // Send the message
   // The +1 to the size here is just to include null terminated character,
   // so that the message is printable by the client
   const int send_result = send(server, message.c_str(), message.size()+1, 0);
   if (send_result == SOCKET_ERROR)
   {
      DWORD error_code = GetLastError();
      std::stringstream ss;
      ss << "Error sending message to server: " << error_code;
      throw std::runtime_error(ss.str());
   }
   std::cout << "Message sent (" << message.size()+1 << "b)" << std::endl;

   // Close the server socket and cleanup WSA after we have sent the message
   closesocket(server);
   WSACleanup();
   return 0;
}
