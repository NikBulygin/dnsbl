#ifndef DNS_SERVER_CONFIGURATION_TEST_H
#define DNS_SERVER_CONFIGURATION_TEST_H

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "configuration_structure.h"

void test_create_and_free_config() {
    ServerConfig* config = create_config();
    CU_ASSERT_PTR_NOT_NULL(config);
    // how to check if the memory is cleared?
    free_config(config);
}

void test_read_config_from_file() {
    ServerConfig* config = create_config();
    read_config_from_file(config, "test-config.cfg");
    CU_ASSERT(config->port == 53);
    CU_ASSERT_STRING_EQUAL(config->upstream_server, "8.8.8.8");
    CU_ASSERT_STRING_EQUAL(config->domainBlockResponse, "not resolved");
    free_config(config);
}

void test_is_black_listed() {
    ServerConfig* config = create_config();
    config->blacklist_count = 1;
    config->blacklist = malloc(sizeof(char*));
    config->blacklist[0] = strdup("blacklisted.com");
    CU_ASSERT_TRUE(is_black_listed("blacklisted.com", config));
    CU_ASSERT_FALSE(is_black_listed("safe.com", config));
    free_config(config);
}

#endif //DNS_SERVER_CONFIGURATION_TEST_H
