cmake_minimum_required(VERSION 3.25)

project(bizu LANGUAGES CXX)

include(cmake/static_analyzers.cmake)
include(cmake/get_cpm.cmake)

# set(bizu_CompilerOptions ${bizu_CompilerOptions})
# set(bizu_LinkerOptions ${bizu_LinkerOptions})

find_package(Threads REQUIRED)

CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage("gh:Dobiasd/FunctionalPlus#v0.2.24")

CPMAddPackage(
    NAME oatpp
    GITHUB_REPOSITORY oatpp/oatpp
    GIT_TAG master
    PATCHES "patches/oatpp.patch"
    OPTIONS "OATPP_BUILD_TESTS OFF" "OATPP_LINK_TEST_LIBRARY OFF"
)

if (oatpp_ADDED)
    add_library(oatpp::oatpp ALIAS oatpp)
endif()

CPMAddPackage(
    NAME oatpp-sqlite
    GITHUB_REPOSITORY oatpp/oatpp-sqlite
    GIT_TAG master
    PATCHES "patches/oatpp_sqlite.patch"
    OPTIONS "OATPP_SQLITE_AMALGAMATION ON" "OATPP_BUILD_TESTS OFF"
)

if (oatpp-sqlite_ADDED)
    add_library(oatpp::oatpp-sqlite ALIAS oatpp-sqlite)
endif()

set(bizu_ExternalLibraries Threads::Threads oatpp::oatpp oatpp::oatpp-sqlite fmt::fmt FunctionalPlus::fplus)

add_subdirectory(bizu)

