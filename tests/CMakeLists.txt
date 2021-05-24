add_executable(c8_test
    test_c8_interface.cc
    test_c8_opcode.cc
    test_c8_random.cc
    test_c8_display.cc
    test_c8_read.cc
    test_c8_keyboard.cc)
target_compile_options(c8_test PRIVATE ${COMPILER_WARNINGS})

if (${USE_CONAN})
    target_link_libraries(c8_test PRIVATE CONAN_PKG::gtest c8_lib)
else()
    target_link_libraries(c8_test PRIVATE GTest::GTest GTest::Main c8_lib)
endif()

gtest_add_tests(TARGET      c8_test
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)