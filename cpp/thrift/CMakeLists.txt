find_program(
    THRIFT_EXECUTABLE
    thrift
    REQUIRED
)

find_path(
    THRIFT_INCLUDE_DIR
    NAMES thrift/Thrift.h
    REQUIRED
)

find_library(
    THRIFT_LIBRARY
    NAMES thrift
    REQUIRED
)

if(NOT TARGET thrift::thrift)
    add_library(thrift::thrift UNKNOWN IMPORTED)

    set_target_properties(
        thrift::thrift
        PROPERTIES
            IMPORTED_LOCATION "${THRIFT_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${THRIFT_INCLUDE_DIR}"
    )
endif()

######################################
# Collect Thrift definitions
######################################
set(
    THRIFT_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../../thrift"
)

file(
    GLOB THRIFT_FILES
    "${THRIFT_SOURCE_DIR}/*.thrift"
)

if(NOT THRIFT_FILES)
    message(FATAL_ERROR
        "No .thrift files found in ${THRIFT_SOURCE_DIR}"
    )
endif()

######################################
# Generated output
######################################
set(
    GEN_DIR
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

set(
    THRIFT_OUT_DIR
    "${GEN_DIR}/gen-cpp"
)

set(
    THRIFT_GEN_DIR
    "${THRIFT_OUT_DIR}"
)

file(MAKE_DIRECTORY "${THRIFT_OUT_DIR}")

set(GENERATED_THRIFT_FILES)

foreach(THRIFT_FILE ${THRIFT_FILES})

    get_filename_component(
        FIL_WE
        "${THRIFT_FILE}"
        NAME_WE
    )

    set(
        THRIFT_HEADER
        "${THRIFT_GEN_DIR}/${FIL_WE}_types.h"
    )

    add_custom_command(
        OUTPUT
            "${THRIFT_HEADER}"

        COMMAND
            "${THRIFT_EXECUTABLE}"
            --gen cpp
            -out "${THRIFT_OUT_DIR}"
            "${THRIFT_FILE}"

        DEPENDS
            "${THRIFT_FILE}"

        COMMENT
            "Generating C++ Thrift files for ${FIL_WE}.thrift"

        VERBATIM
    )

    list(
        APPEND
        GENERATED_THRIFT_FILES
        "${THRIFT_HEADER}"
    )

endforeach()

######################################
# Generation target
######################################
add_custom_target(
    generate-thrift
    DEPENDS
        ${GENERATED_THRIFT_FILES}
)

######################################
# Thrift interface target
######################################
add_library(thrift-playground INTERFACE)

add_dependencies(
    thrift-playground
    generate-thrift
)

target_include_directories(
    thrift-playground
    INTERFACE
        "${THRIFT_GEN_DIR}"
        "${THRIFT_INCLUDE_DIR}"
)

target_link_libraries(
    thrift-playground
    INTERFACE
        thrift::thrift
)
