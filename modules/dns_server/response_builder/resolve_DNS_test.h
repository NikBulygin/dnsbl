#ifndef DNS_SERVER_RESOLVE_DNS_TEST_H
#define DNS_SERVER_RESOLVE_DNS_TEST_H

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "resolve_DNS.h"

void test_is_valid_domain() {
    CU_ASSERT_TRUE(is_valid_domain("google.com"));
    CU_ASSERT_FALSE(is_valid_domain("google..com"));
}

void test_is_black_listed_resolve_DNS() {
    ServerConfig* config = create_config();
    config->blacklist_count = 1;
    config->blacklist = malloc(sizeof(char*));
    config->blacklist[0] = strdup("blacklisted.com");
    
    CU_ASSERT_TRUE(is_black_listed("blacklisted.com", config));
    CU_ASSERT_FALSE(is_black_listed("safe.com", config));
    
    free_config(config);
}

void test_fetch_from_upstream() {
    ServerConfig* config = create_config();
    config->upstream_server = strdup("8.8.8.8"); // Пример для Google Public DNS

    char* ip = fetch_from_upstream("google.com", config);
    CU_ASSERT_PTR_NOT_NULL(ip);
    // В зависимости от реализации, можете проверить дополнительные условия

    free_config(config);
    free(ip);
}

void test_resolve_DNS() {
    ServerConfig* config = create_config();
    config->upstream_server = strdup("8.8.8.8");
    DnsCache cache;
    init_cache(&cache);

    char* ip = resolve_DNS("google.com", config, &cache);
    CU_ASSERT_PTR_NOT_NULL(ip);
    // Можете добавить дополнительные проверки на кеширование и другие условия

    free_config(config);
    free(ip);
}



#endif //DNS_SERVER_RESOLVE_DNS_TEST_H
