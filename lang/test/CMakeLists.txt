include_directories(../src ../../src ../../googletest/include)

enable_testing()

add_executable(ast_test ast_test.cpp)
target_link_libraries(ast_test ToPS gtest gtest_main pthread)
add_test(tops-lang-ast-test ast_test)

add_executable(parser_test parser_test.cpp)
target_link_libraries(parser_test ToPS gtest gtest_main pthread)
add_test(tops-lang-parser-test parser_test)
