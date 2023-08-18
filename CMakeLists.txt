cmake_minimum_required(VERSION 3.10)

project(dns_server)

# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG_MODE)
endif()

# ASan
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

# Find libconfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig)

# Include directories for both libconfig and CUnit
include_directories(/usr/include)
link_directories(/lib64)

# List of source files
set(SOURCE_FILES
  main.c
  modules/dns_server/cache/cache.c
  modules/dns_server/cache/cache.h
  modules/dns_server/configuration/configuration_structure.c
  modules/dns_server/configuration/configuration_structure.h
  modules/dns_server/port_listener/listener.c
  modules/dns_server/port_listener/listener.h
  modules/dns_server/response_builder/resolve_DNS.c
  modules/dns_server/response_builder/resolve_DNS.h
)

# If debug mode, add files for testing
if(CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND SOURCE_FILES
      modules/dns_server/cache/cache_test.h
      modules/dns_server/configuration/configuration_test.h
      modules/dns_server/port_listener/listener_test.h
      modules/dns_server/response_builder/resolve_DNS_test.h
    )
endif()

# Ensure the source files exist
foreach(SOURCE_FILE ${SOURCE_FILES})
  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")
    message(FATAL_ERROR "File ${SOURCE_FILE} does not exist.")
  endif()
endforeach()

# Create executable
add_executable(dns_server ${SOURCE_FILES})

# Link with libconfig and CUnit
target_link_libraries(dns_server ${LIBCONFIG_LIBRARIES} cunit)

# Copy configuration file to build directory
configure_file(${CMAKE_SOURCE_DIR}/config.cfg ${CMAKE_BINARY_DIR}/config.cfg COPYONLY)
if(CMAKE_BUILD_TYPE MATCHES Debug)
  configure_file(${CMAKE_SOURCE_DIR}/test-config.cfg ${CMAKE_BINARY_DIR}/test-config.cfg COPYONLY)
endif()
