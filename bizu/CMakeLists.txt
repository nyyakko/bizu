add_subdirectory(source)
add_subdirectory(include/${PROJECT_NAME})

add_executable(${PROJECT_NAME} "${bizu_SourceFiles}")

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
        HOME="${CMAKE_SOURCE_DIR}"
)

if (ENABLE_CLANGTIDY)
    enable_clang_tidy(${PROJECT_NAME})
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_link_options(${PROJECT_NAME} PRIVATE ${bizu_LinkerOptions})
target_compile_options(${PROJECT_NAME} PRIVATE ${bizu_CompilerOptions})
target_link_libraries(${PROJECT_NAME} PRIVATE ${bizu_ExternalLibraries})

