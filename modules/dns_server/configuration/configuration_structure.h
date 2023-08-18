#ifndef DNS_SERVER_CONFIGURATION_STRUCTURE_H
#define DNS_SERVER_CONFIGURATION_STRUCTURE_H

#define DNS_SERVER_DEFAULT_CONFIG_FILE "config.cfg"
#define DNS_SERVER_MAX_ENTRIES 1000
#define DNS_SERVER_FILENAME "cache.txt"
#define DNS_SERVER_MAX_SIZE_OF_DNS_NAME 256
#define DNS_SERVER_MAX_SIZE_OF_IP 16


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>



typedef struct {
    int port;
    char* upstream_server; 
    char** blacklist;
    int blacklist_count; 
    char* domainBlockResponse;
} ServerConfig;


ServerConfig* create_config();
void free_config(ServerConfig* config);
void read_config_from_file(ServerConfig* config, const char* filename);
bool is_black_listed(const char* domain, ServerConfig* config);

#endif // DNS_SERVER_CONFIGURATION_STRUCTURE_H
