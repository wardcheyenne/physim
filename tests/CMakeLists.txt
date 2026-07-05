find_package(GTest QUIET)
if(NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/v1.14.0.zip)
  FetchContent_MakeAvailable(googletest)
endif()

add_executable(physim_tests
  test_vector.cpp
  test_simulation.cpp
)
target_link_libraries(physim_tests PRIVATE physim_core GTest::gtest_main)
add_test(NAME physim_tests COMMAND physim_tests)
