#include <iostream>
#include <SDL.h>
#include <SDL_net.h>

int main(int argc, char* argv[]) {
    // Initialize SDL and SDL_net
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1 || SDLNet_Init() == -1) {
        std::cerr << "SDL_Init or SDLNet_Init failed: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    // Open a socket for listening
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, nullptr, 1234) == -1) {
        std::cerr << "SDLNet_ResolveHost failed: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }

    TCPsocket server = SDLNet_TCP_Open(&ip);
    if (!server) {
        std::cerr << "SDLNet_TCP_Open failed: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }

    std::cout << "Server is running. Waiting for a connection..." << std::endl;

    while (true) {
        // Wait for a client connection
        TCPsocket client = SDLNet_TCP_Accept(server);
        if (!client) {
            SDL_Delay(100);  // Wait a little bit before trying again (prevents CPU overuse)
            continue;        // No client yet, keep trying
        }

        std::cout << "Client connected!" << std::endl;

        // Receive a message from the client
        char buffer[512];
        int receivedBytes = SDLNet_TCP_Recv(client, buffer, sizeof(buffer));
        if (receivedBytes > 0) {
            std::cout << "Received from client: " << buffer << std::endl;
        }

        SDLNet_TCP_Close(client);  // Close the client socket after communication
    }

    // Cleanup (although this is unreachable in this example)
    SDLNet_TCP_Close(server);
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}