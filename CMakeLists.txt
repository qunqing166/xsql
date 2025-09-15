cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(x-sql)

add_subdirectory(${PROJECT_SOURCE_DIR}/subpro/xformat)

set(INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/subpro/xformat/include
)

set(LIBS
    libmysqlclient.so   
)

include_directories(${INCLUDE_DIRS})

aux_source_directory(src SRC_LIST)

add_library(xsql STATIC ${SRC_LIST})
add_library(xsql::lib ALIAS xsql)
target_link_libraries(xsql PRIVATE xformat::static)

add_executable (main test/main.cpp)
target_link_libraries(main PRIVATE xsql::lib ) 
target_link_libraries(main PRIVATE ${LIBS})

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

