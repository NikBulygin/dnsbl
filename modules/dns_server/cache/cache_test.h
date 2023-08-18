#ifndef DNS_SERVER_CACHE_TEST_H
#define DNS_SERVER_CACHE_TEST_H

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "cache.h"

void test_init_cache() {
    DnsCache cache;
    init_cache(&cache);
    CU_ASSERT(cache.count == 0);
}

void test_add_and_get_from_cache() {
    DnsCache cache;
    init_cache(&cache);
    
    add_to_cache(&cache, "www.example.com", "93.184.216.34");
    char *ip = get_from_cache(&cache, "www.example.com");
    CU_ASSERT_PTR_NOT_NULL(ip);
    CU_ASSERT_STRING_EQUAL(ip, "93.184.216.34");
    
    char *not_present = get_from_cache(&cache, "not.present");
    CU_ASSERT_PTR_NULL(not_present);
}

void test_save_and_load_cache() {
    DnsCache cache1, cache2;
    init_cache(&cache1);
    init_cache(&cache2);
    
    add_to_cache(&cache1, "www.example.com", "93.184.216.34");
    save_cache_to_file(&cache1);

    load_cache_from_file(&cache2);
    char *ip = get_from_cache(&cache2, "www.example.com");
    CU_ASSERT_PTR_NOT_NULL(ip);
    CU_ASSERT_STRING_EQUAL(ip, "93.184.216.34");
}


void cache_test() {
    
    
};


#endif //DNS_SERVER_CACHE_TEST_H
