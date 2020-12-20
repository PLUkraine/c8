add_executable(TestC8 test_c8_interface.cc test_c8_opcode.cc)
target_link_libraries(TestC8 GTest::GTest GTest::Main c8_lib)
target_compile_options(TestC8 PRIVATE ${COMPILER_WARNINGS})

gtest_add_tests(TARGET      TestC8
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)