#ifndef DNS_SERVER_RESOLVE_DNS_H
#define DNS_SERVER_RESOLVE_DNS_H

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "../configuration/configuration_structure.h"
#include "../cache/cache.h"

bool is_valid_domain(const char* domain);
bool is_black_listed(const char* domain, ServerConfig* config);
char* fetch_from_upstream(const char* domain, ServerConfig* config);
char* resolve_DNS(char* domain, ServerConfig* config,
                  DnsCache *cache);

#endif //DNS_SERVER_RESOLVE_DNS_H
