add_library(segtree_lib INTERFACE)
target_include_directories(segtree_lib INTERFACE ${PROJECT_SOURCE_DIR}/src)
target_sources(segtree_lib INTERFACE ${PROJECT_SOURCE_DIR}/src/segtree.h)
