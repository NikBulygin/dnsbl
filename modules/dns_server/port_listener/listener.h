#ifndef DNS_SERVER_PORT_LISTENER_H
#define DNS_SERVER_PORT_LISTENER_H

#include "../configuration/configuration_structure.h"
#include "../cache/cache.h"
#include "../response_builder/resolve_DNS.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int create_server(ServerConfig* config);
void start_server(int server_socket, ServerConfig* config, DnsCache* cache);
void delete_server(int server_socket);
void handle_client(int client_socket, struct sockaddr_in* client_addr,
                   ServerConfig* config,
                   DnsCache* cache);

#endif //  DNS_SERVER_PORT_LISTENER_H
