add_executable(TestExample test_example.cc)
target_link_libraries(TestExample GTest::GTest GTest::Main)
target_compile_options(TestExample PRIVATE ${COMPILER_WARNINGS})

gtest_add_tests(TARGET      TestExample
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)