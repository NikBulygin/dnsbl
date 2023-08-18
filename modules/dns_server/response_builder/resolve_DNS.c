#include "resolve_DNS.h"

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool is_valid_domain(const char* domain) {
    if (!domain) return false; // Null pointer check

    int len = strlen(domain);


    // Rule 1: Domain names can be up to 253 characters long.
    if (len < 1 || len > 253) return false;

    int label_len = 0;
    bool last_was_dot = true; // Consider starting as if there was a dot to ensure the first character is not a dot

    for (int i = 0; i < len; i++) {
        char c = domain[i];

        // Rule 2: Valid characters in a domain name are ASCII letters, digits, and hyphens.
        if (!isalnum(c) && c != '-' && c != '.') return false;

        if (c == '.') {
            // Rule 3: No two dots can appear consecutively. Labels can be up to 63 characters long.
            if (last_was_dot || label_len == 0 || label_len > 63) return false;
            label_len = 0;
            last_was_dot = true;
        } else {
            // Rule 4: A domain name cannot start or end with a hyphen.
            if ((i == 0 || i == len - 1) && c == '-') return false;
            label_len++;
            last_was_dot = false;
        }
    }

    // Rule 5: Ensure the last character wasn't a dot and there's at least one dot
    return !last_was_dot && strchr(domain, '.') != NULL;
}


char* fetch_from_upstream(const char* domain, ServerConfig* config) {
    struct addrinfo hints, *res;
    int errcode;
    char addrstr[100];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // Limiting to IPv4 for simplicity
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;

    errcode = getaddrinfo(domain, NULL, &hints, &res);
    if (errcode != 0) {
        fprintf(stderr, "Failed to resolve domain: %s\n", gai_strerror(errcode));
        return strdup("Domain resolution failed");
    }

    // Convert network address to a human-readable string
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char *ipAddress = inet_ntoa(ipv4->sin_addr);

    strncpy(addrstr, ipAddress, sizeof(addrstr));
    addrstr[sizeof(addrstr) - 1] = '\0'; // Ensure null termination

    freeaddrinfo(res);
    
    return strdup(addrstr);
}

char* remove_spaces_and_newline(const char* input) {
    int length = strlen(input);
    char *result = (char *)malloc(length + 1); // +1 для нуль-терминатора

    int j = 0; // индекс для результата
    for (int i = 0; i < length; i++) {
        if (input[i] != ' ' && input[i] != '\n') { // 27 - это код ESC-символа
            result[j] = input[i];
            j++;
        }
    }
    result[j] = '\0'; // завершающий нуль-терминатор

    return result;
}


char* resolve_DNS(char* domain, ServerConfig* config,
                  DnsCache *cache) {

    char* domain_without_space = remove_spaces_and_newline(domain);
    
    if (!is_valid_domain(domain_without_space)) {
        return "Invalid domain";
    }

    if (is_black_listed(domain_without_space, config)) {
        return config->domainBlockResponse;
    }

    char* cachedResponse = get_from_cache(cache, domain_without_space);
    if (cachedResponse != NULL) {
        return cachedResponse;
    }

    char* upstreamResponse = fetch_from_upstream(domain_without_space, config);
    add_to_cache(cache, domain_without_space, upstreamResponse);
    return upstreamResponse;
}

