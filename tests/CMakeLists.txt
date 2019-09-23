project(Google_Tests_run)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run RamoTest.cpp CajonTest.cpp)

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(Google_Tests_run Ramo_lib Cajon_lib Utils_lib )

target_link_libraries(Google_Tests_run gtest gtest_main)