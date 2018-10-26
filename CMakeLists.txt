cmake_minimum_required(VERSION 3.1)
project(libflowtuple C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -lwandio)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-files/cmake")
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${libflowtuple_BINARY_DIR}/bin)
endif()

#
# Version - read from 'version' file.
#
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/cmake-files/version _version)
string(REGEX REPLACE
        "^([0-9])[0-9][0-9][0-9][0-9][0-9][0-9][a-z]*$" "\\1" _major ${_version})
string(REGEX REPLACE
        "^[0-9]([0-9][0-9][0-9])[0-9][0-9][0-9][a-z]*$" "\\1" _minor ${_version})
string(REGEX REPLACE
        "^[0-9][0-9][0-9][0-9]([0-9][0-9][0-9])[a-z]*$" "\\1" _revision ${_version})
string(REGEX REPLACE
        "^[0-9][0-9][0-9][0-9][0-9][0-9][0-9]([a-z]*)$" "\\1" _quality ${_version})
set(_version_number ${_major}${_minor}${_revision})
string(REGEX REPLACE "[0]*([^0]*[0-9])$" "\\1" _trimmed_minor ${_minor})
string(REGEX REPLACE "[0]*([^0]*[0-9])$" "\\1" _trimmed_revision ${_revision})

set(VERSION "${_major}.${_trimmed_minor}.${_trimmed_revision}${_quality}")
set(LIBFLOWTUPLE_VERSION_NUMBER "${_version_number}")
set(LIBFLOWTUPLE_VERSION_STRING "${VERSION}")

option(ENABLE_INSTALL "Enable installing of libraries" ON)

find_library(WANDIO wandio)

if(NOT WANDIO)
  message(FATAL_ERROR "libwandio not found")
endif()

include_directories(lib/libflowtuple)

# include(CreatePkgConfigFile)

add_library(flowtuple SHARED
        lib/libflowtuple/flowtuple.c
        lib/libflowtuple/flowtuple.h
        lib/libflowtuple/util.c
        lib/libflowtuple/util.h
        lib/libflowtuple/fttypes.h
        lib/libflowtuple/record.c
        lib/libflowtuple/record.h
        lib/libflowtuple/class.c
        lib/libflowtuple/data.c
        lib/libflowtuple/header.c
        lib/libflowtuple/interval.c
        lib/libflowtuple/trailer.c)

add_executable(flow2ascii tools/flow2ascii.c)
target_link_libraries(flow2ascii flowtuple)

install(FILES lib/libflowtuple/flowtuple.h DESTINATION include)
install(TARGETS flowtuple flow2ascii
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
