cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(BUILD_TESTS ON)
# option(BUILD_TESTS "build tests" ON)

project(x-sql)

set(INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/utils
    /usr/include/mysql/
)

set(LIBS
    libmysqlclient.so   
)

include_directories(${INCLUDE_DIRS})

aux_source_directory(src SRC_LIST)

add_library(xsql STATIC ${SRC_LIST})
add_library(xsql::lib ALIAS xsql)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

