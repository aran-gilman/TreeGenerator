add_library(mesh mesh_data.cpp)
set_property(TARGET mesh PROPERTY CXX_STANDARD 20)
target_link_libraries(mesh PRIVATE glm)