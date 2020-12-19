add_executable(TestExample test_example.cc)
target_link_libraries(TestExample GTest::GTest GTest::Main)

gtest_add_tests(TARGET      TestExample
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)