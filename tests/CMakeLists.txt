add_executable(TestC8
    test_c8_interface.cc
    test_c8_opcode.cc
    test_c8_random.cc
    test_c8_display.cc
    test_c8_read.cc
    test_c8_keyboard.cc)
target_link_libraries(TestC8 PRIVATE CONAN_PKG::gtest c8_lib)
target_compile_options(TestC8 PRIVATE ${COMPILER_WARNINGS})

gtest_add_tests(TARGET      TestC8
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)