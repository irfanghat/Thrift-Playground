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

######################################
# Apache Thrift library
######################################
if(NOT TARGET thrift::thrift)

    add_library(thrift::thrift UNKNOWN IMPORTED)

    set_target_properties(
        thrift::thrift
        PROPERTIES
            IMPORTED_LOCATION
                "${THRIFT_LIBRARY}"

            INTERFACE_INCLUDE_DIRECTORIES
                "${THRIFT_INCLUDE_DIR}"
    )

endif()

######################################
# Thrift definitions
######################################

set(
    THRIFT_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../../thrift"
)

file(
    GLOB THRIFT_FILES
    CONFIGURE_DEPENDS
    "${THRIFT_SOURCE_DIR}/*.thrift"
)

if(NOT THRIFT_FILES)

    message(FATAL_ERROR
        "No .thrift files found in ${THRIFT_SOURCE_DIR}"
    )

endif()

######################################
# Generated files
######################################
set(
    THRIFT_OUT_DIR
    "${CMAKE_CURRENT_BINARY_DIR}/generated/gen-cpp"
)

file(
    MAKE_DIRECTORY
    "${THRIFT_OUT_DIR}"
)

set(
    GENERATED_THRIFT_CPP
)

set(
    GENERATED_THRIFT_HEADERS
)

######################################
# Generate Thrift code
######################################
foreach(THRIFT_FILE IN LISTS THRIFT_FILES)

    get_filename_component(
        THRIFT_NAME
        "${THRIFT_FILE}"
        NAME_WE
    )

    string(
        SUBSTRING
        "${THRIFT_NAME}"
        0
        1
        THRIFT_FIRST_CHAR
    )

    string(
        TOUPPER
        "${THRIFT_FIRST_CHAR}"
        THRIFT_FIRST_CHAR
    )

    string(
        SUBSTRING
        "${THRIFT_NAME}"
        1
        -1
        THRIFT_REST
    )

    set(
        THRIFT_CLASS_NAME
        "${THRIFT_FIRST_CHAR}${THRIFT_REST}"
    )

    set(
        GENERATED_CPP
        "${THRIFT_OUT_DIR}/${THRIFT_CLASS_NAME}.cpp"
    )

    set(
        GENERATED_HEADER
        "${THRIFT_OUT_DIR}/${THRIFT_CLASS_NAME}.h"
    )

    set(
        GENERATED_TYPES_HEADER
        "${THRIFT_OUT_DIR}/${THRIFT_NAME}_types.h"
    )

    add_custom_command(
        OUTPUT
            "${GENERATED_CPP}"
            "${GENERATED_HEADER}"
            "${GENERATED_TYPES_HEADER}"

        COMMAND
            "${THRIFT_EXECUTABLE}"
            --gen cpp
            -out "${THRIFT_OUT_DIR}"
            "${THRIFT_FILE}"

        DEPENDS
            "${THRIFT_FILE}"

        COMMENT
            "Generating C++ Thrift files for ${THRIFT_NAME}.thrift"

        VERBATIM
    )

    list(
        APPEND
        GENERATED_THRIFT_CPP
        "${GENERATED_CPP}"
    )

    list(
        APPEND
        GENERATED_THRIFT_HEADERS
        "${GENERATED_HEADER}"
        "${GENERATED_TYPES_HEADER}"
    )

endforeach()

######################################
# Generated Thrift library
######################################
add_library(
    thrift-playground
    STATIC
        ${GENERATED_THRIFT_CPP}
        ${GENERATED_THRIFT_HEADERS}
)

target_include_directories(
    thrift-playground
    PUBLIC
        "${THRIFT_OUT_DIR}"
        "${THRIFT_INCLUDE_DIR}"
)

target_link_libraries(
    thrift-playground
    PUBLIC
        thrift::thrift
)