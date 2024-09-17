#include <iostream>
#include <SDL.h>
#include <SDL_net.h>

int main(int argc, char* argv[]) {
    // Initialize SDL_net
    if (SDL_Init(0) == -1 || SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        return -1;
    }

    IPaddress ip;
    TCPsocket server, client;

    // Resolve server host (NULL for INADDR_ANY) and port
    if (SDLNet_ResolveHost(&ip, NULL, 12345) == -1) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        return -1;
    }

    // Open a server socket
    server = SDLNet_TCP_Open(&ip);
    if (!server) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return -1;
    }

    std::cout << "Server listening on port 12345..." << std::endl;

    // Wait for a connection
    client = SDLNet_TCP_Accept(server);
    if (client) {
        char message[512];
        int result = SDLNet_TCP_Recv(client, message, 512);
        if (result > 0) {
            message[result] = '\0';
            std::cout << "Received message: " << message << std::endl;

            // Send a response
            const char* response = "Hello from server!";
            SDLNet_TCP_Send(client, response, strlen(response) + 1);
        }
        else {
            std::cerr << "SDLNet_TCP_Recv: " << SDLNet_GetError() << std::endl;
        }

        SDLNet_TCP_Close(client);
    }

    // Close server socket and quit SDL_net
    SDLNet_TCP_Close(server);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}
