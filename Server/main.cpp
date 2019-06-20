// Std Includes
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <string>

// Windows Includes
#include <winsock2.h>
#include <Ws2tcpip.h>

// Global constants
const uint_least16_t PORT = 55555;

int main()
{
   // Start WSAData
   WSADATA WSAData;
   WSAStartup(MAKEWORD(2, 2), &WSAData);

   // Setup the socket
   SOCKET server, client;
   SOCKADDR_IN serverAddr, clientAddr;
   server = socket(AF_INET, SOCK_STREAM, 0);
   serverAddr.sin_addr.s_addr = INADDR_ANY;
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(PORT);

   // Bind the socket to the port
   const int bind_result = bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
   if (bind_result == SOCKET_ERROR)
   {
      throw std::runtime_error("Error binding socket!");
   }

   // Start to listen on the socket
   const int listen_result = listen(server, 0);
   if (listen_result == SOCKET_ERROR)
   {
      throw std::runtime_error("Error listening on socket!");
   }
   std::cout << "Listening on port [" << std::to_string(PORT) << "]" << std::endl;

   char buffer[1024];
   int clientAddrSize = sizeof(clientAddr);
   std::cout << "Waiting for connections..." << std::endl;
   if ((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
   {
      char clientAddrStr[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &clientAddr.sin_addr, clientAddrStr, INET_ADDRSTRLEN);
      std::cout << "Client Connected from [" << clientAddrStr << "]" << std::endl;
      int bytes_recvd = recv(client, buffer, sizeof(buffer), 0);
      std::cout << "Client says: " << buffer << " (" << bytes_recvd << "b)" << std::endl;
   }

   // Shutdown
   closesocket(client);
   closesocket(server);
   WSACleanup();
   return 0;
}