#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int PORT = 8080;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Server listening on port 8080" << std::endl;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address,
                            (socklen_t*)&addrlen);

        std::string response =
            "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n"
            "Served by backend container\n";

        send(new_socket, response.c_str(), response.size(), 0);
        close(new_socket);
    }

    return 0;
}
