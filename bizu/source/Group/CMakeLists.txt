add_subdirectory(Entities)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(bizu_SourceFiles ${bizu_SourceFiles}
    "${DIR}/GroupRepository.cpp"

    PARENT_SCOPE
)

