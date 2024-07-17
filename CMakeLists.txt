cmake_minimum_required(VERSION 3.25)

project(bizu LANGUAGES CXX)

include(cmake/static_analyzers.cmake)
include(cmake/get_cpm.cmake)

# set(bizu_CompilerOptions ${bizu_CompilerOptions})
# set(bizu_LinkerOptions ${bizu_LinkerOptions})

find_package(Threads            REQUIRED)
find_package(oatpp        1.4.0 REQUIRED)
find_package(oatpp-sqlite 1.4.0 REQUIRED)

set(bizu_ExternalLibraries Threads::Threads oatpp::oatpp oatpp::oatpp-sqlite)

add_subdirectory(bizu)

