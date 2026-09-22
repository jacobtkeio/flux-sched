add_library(dfu_match_attrs STATIC
    dfu_match_attrs.h
    dfu_match_attrs.c
    )
target_link_libraries(dfu_match_attrs PUBLIC
    PkgConfig::JANSSON
    )

add_subdirectory(test)
