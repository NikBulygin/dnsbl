#ifndef DNS_SERVER_CACHE_H
#define DNS_SERVER_CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../configuration/configuration_structure.h"

typedef struct {
    char dns_name[DNS_SERVER_MAX_SIZE_OF_DNS_NAME];
    char ip_address[DNS_SERVER_MAX_SIZE_OF_IP];
} DnsCacheEntry;

typedef struct {
    DnsCacheEntry entries[DNS_SERVER_MAX_ENTRIES];
    int count;
} DnsCache;

void init_cache(DnsCache *cache);

void add_to_cache(DnsCache *cache, const char *dns_name, const char *ip_address);
        
char* get_from_cache(DnsCache *cache, const char *dns_name);

void save_cache_to_file(DnsCache *cache);

void load_cache_from_file(DnsCache *cache);

#endif //DNS_SERVER_CACHE_H
