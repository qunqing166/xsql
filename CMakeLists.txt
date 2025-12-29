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
    ${PROJECT_SOURCE_DIR}/include/db_type
    #    /usr/include/mysql/
)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_COMMON)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/db_type/mysql SRC_MYSQL)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/db_type/sqlite SRC_SQLITE)

set(SRC_LISTS
    ${SRC_COMMON}
    ${SRC_MYSQL}
)

set(LIBS
    libmysqlclient.so   
)

include_directories(${INCLUDE_DIRS})

add_library(xsql_core STATIC ${SRC_LISTS})
add_library(xsql_mysql STATIC ${SRC_MYSQL})
target_link_libraries(xsql_mysql PRIVATE xsql_core mysqlclient)

add_library(xsql_sqlite STATIC ${SRC_SQLITE})
target_link_libraries(xsql_sqlite PRIVATE xsql_core sqlite3)

add_library(xsql::mysql ALIAS xsql_mysql)
add_library(xsql::sqlite ALIAS xsql_sqlite)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

