cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(BUILD_TESTS ON)
# option(BUILD_TESTS "build tests" ON)

project(x-sql)

add_subdirectory(${PROJECT_SOURCE_DIR}/third_part/xformat)

set(INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/utils
    ${PROJECT_SOURCE_DIR}/third_part/xformat/include
    /usr/include/mysql/
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

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY git@github.com:google/googletest.git
        GIT_TAG        v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

    add_executable (test_convert test/test_convert.cpp)
    target_link_libraries(test_convert PRIVATE xsql::lib ) 
    target_link_libraries(test_convert PRIVATE ${LIBS} GTest::gtest_main)
    target_include_directories(test_convert PRIVATE ${GoogleTest_SOURCE_DIR}/include)
    include(GoogleTest)
    
endif()

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

