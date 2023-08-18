#include "listener.h"

int create_server(ServerConfig* config) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        exit(1);
    }
    return server_socket;
}



void delete_server(int server_socket) {
    close(server_socket);
}

void start_server(int server_socket, ServerConfig* config,
                  DnsCache* cache) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config->port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    listen(server_socket, 5);
    printf("Listening on port %d...\n", config->port);

    // Основной цикл сервера для принятия запросов от клиентов
    while(1) {
        client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        handle_client(client_socket, &client_addr, config, cache);
    }
}

void handle_client(int client_socket, struct sockaddr_in* client_addr,
                   ServerConfig* config,
                   DnsCache* cache) {
    char buffer[DNS_SERVER_MAX_SIZE_OF_DNS_NAME];
    int bytes_received;

    // Выводим IP и порт клиента
    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

    // Чтение данных от клиента
    bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("Error reading from socket");
        close(client_socket);
        return;
    }
    buffer[bytes_received] = '\0'; // Завершающий нуль, чтобы строка была правильно завершена

    printf("Received message from client: %s\n", buffer);
    char *result = resolve_DNS(buffer, config, cache);
    
    // Отправляем обратно то же сообщение
    send(client_socket, result, strlen(result), 0);
    close(client_socket);
}
