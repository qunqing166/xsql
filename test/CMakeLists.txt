include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY git@github.com:google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable (test_xsql ${TEST_SRCS})
target_link_libraries(test_xsql PRIVATE xsql::lib ) 
target_link_libraries(test_xsql PRIVATE ${LIBS} GTest::gtest_main)
target_include_directories(test_xsql PRIVATE ${GoogleTest_SOURCE_DIR}/include)

include(GoogleTest)
  
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
