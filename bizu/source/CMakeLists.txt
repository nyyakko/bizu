add_subdirectory(User)
add_subdirectory(Group)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(bizu_SourceFiles ${bizu_SourceFiles}
    "${DIR}/Main.cpp"

    PARENT_SCOPE
)

