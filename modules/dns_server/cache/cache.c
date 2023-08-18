#include "cache.h"

void init_cache(DnsCache *cache) {
    cache->count = 0;
}


void add_to_cache(DnsCache *cache, const char *dns_name, const char *ip_address) {
    if (cache->count < DNS_SERVER_MAX_ENTRIES) {
        strncpy(cache->entries[cache->count].dns_name, dns_name, sizeof(cache->entries[cache->count].dns_name));
        strncpy(cache->entries[cache->count].ip_address, ip_address, sizeof(cache->entries[cache->count].ip_address));
        cache->count++;
    }
}

char* get_from_cache(DnsCache *cache, const char *dns_name) {
    for (int i = 0; i < cache->count; i++) {
        if (strcmp(cache->entries[i].dns_name, dns_name) == 0) {
            return cache->entries[i].ip_address;
        }
    }
    return NULL;
}


void save_cache_to_file(DnsCache *cache) {
    FILE *file = fopen(DNS_SERVER_FILENAME, "w");
    for (int i = 0; i < cache->count; i++) {
        fprintf(file, "%s %s\n", cache->entries[i].dns_name, cache->entries[i].ip_address);
    }
    fclose(file);
}


void load_cache_from_file(DnsCache *cache) {
    FILE *file = fopen(DNS_SERVER_FILENAME, "r");
    if (file) {
        while (fscanf(file, "%255s %15s", cache->entries[cache->count].dns_name, cache->entries[cache->count].ip_address) != EOF) {
            cache->count++;
        }
        fclose(file);
    }
}
