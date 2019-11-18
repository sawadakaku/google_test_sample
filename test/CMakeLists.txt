add_executable(segtree_test segtree_test.cpp)

target_link_libraries(segtree_test gtest_main)
target_link_libraries(segtree_test segtree_lib)

add_test(NAME segtree_test COMMAND segtree_test)
