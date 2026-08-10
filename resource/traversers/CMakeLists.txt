add_library(dfu_match_params STATIC
    dfu_match.h
    dfu_match.c
    )
target_link_libraries(dfu_match_params PUBLIC
    PkgConfig::JANSSON
    match_op
    )

add_subdirectory(test)
