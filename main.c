#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "./modules/dns_server/configuration/configuration_structure.h"
#include "./modules/dns_server/port_listener/listener.h"
#include "./modules/dns_server/cache/cache.h"


#ifdef DEBUG_MODE
#include "./modules/dns_server/cache/cache_test.h"
#include "./modules/dns_server/configuration/configuration_test.h"
#include "./modules/dns_server/port_listener/listener_test.h"
#include "./modules/dns_server/response_builder/resolve_DNS_test.h"

void unit_test() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("DNS Cache Test Suite", NULL, NULL);

    // Cache
    CU_add_test(suite, "test_init_cache", test_init_cache);
    CU_add_test(suite, "test_add_and_get_from_cache",
                test_add_and_get_from_cache);
    CU_add_test(suite, "test_save_and_load_cache",
                test_save_and_load_cache);

    //Configuration
    CU_add_test(suite, "test_create_and_free_config",
                test_create_and_free_config);
    CU_add_test(suite, "test_read_config_from_file",
                test_read_config_from_file);
    /* CU_add_test(suite, "test_is_black_listed", */
    /*             test_is_black_listed); */

    //how check listener?

    //Resolve DNS
    CU_add_test(suite, "Test Valid Domain", test_is_valid_domain);
    CU_add_test(suite, "Test Blacklist Checking",
                test_is_black_listed_resolve_DNS);
    CU_add_test(suite, "Test Fetching from Upstream", test_fetch_from_upstream);
    CU_add_test(suite, "Test DNS Resolution", test_resolve_DNS);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    CU_cleanup_registry();
}
#endif

int main() {

    #ifdef DEBUG_MODE
    printf("%d\n%d\n%d\n",
           is_valid_domain("8.8.8.8"),
           is_valid_domain("google.com"),
           is_valid_domain("google..com"));
    unit_test();
    #endif
    
    ServerConfig* cfg = create_config();
    read_config_from_file(cfg, "config.cfg");

    DnsCache cache;
    init_cache(&cache);
    load_cache_from_file(&cache);

    int server_socket = create_server(cfg);
    start_server(server_socket, cfg, &cache);
    
    /* free_config(cfg); */
    return 0;
}
