#include "configuration_structure.h"

ServerConfig* create_config() {
    ServerConfig* config = (ServerConfig*)malloc(sizeof(ServerConfig));

    if (!config) {
        perror("Unable to allocate memory for ServerConfig");
        exit(1);
    }

    config->upstream_server = NULL;
    config->blacklist = NULL;
    config->blacklist_count = 0;
    config->domainBlockResponse = NULL;

    return config;
}


void free_config(ServerConfig* config) {
    free(config->upstream_server);
    for (int i = 0; i < config->blacklist_count; i++) {
        free(config->blacklist[i]);
    }
    free(config->blacklist);
    free(config->domainBlockResponse);
    free(config);
}



void read_config_from_file(ServerConfig* config, const char* filename) {
    if (!config) {
        fprintf(stderr, "Config pointer is NULL.\n");
        return;
    }

    config_t cfg;
    config_init(&cfg);

    const char* file_to_open = (filename) ? filename : "config.cfg";

    if (!config_read_file(&cfg, file_to_open)) {
        fprintf(stderr, "Error on line %d: %s\n", config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return;
    }

    // Reading port
    config_lookup_int(&cfg, "server.port", &config->port);

    // Reading upstream_server
    const char* str;
    config_lookup_string(&cfg, "server.upstream_server", &str);
    config->upstream_server = strdup(str);

    // Reading blacklist
    config_setting_t *bl = config_lookup(&cfg, "server.blacklist");
    config->blacklist_count = config_setting_length(bl);
    config->blacklist = (char**)malloc(config->blacklist_count * sizeof(char*));
    for (int i = 0; i < config->blacklist_count; i++) {
        config->blacklist[i] = strdup(config_setting_get_string_elem(bl, i));
    }

    // Reading domainBlockResponse
    config_lookup_string(&cfg, "server.domainBlockResponse", &str);
    config->domainBlockResponse = strdup(str);

    config_destroy(&cfg);
}


bool is_black_listed(const char* domain, ServerConfig* config) {
    for (int i = 0; i < config->blacklist_count; i++) {
        if (strcmp(domain, config->blacklist[i]) == 0) {
            return true;
        }
    }
    return false;
}
