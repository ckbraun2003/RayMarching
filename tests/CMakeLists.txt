project(RayTracingEngineTests)

# Define the test executable
add_executable(RayTracingEngineTests
    CameraTests.cpp
    EventHandlerTests.cpp
    ../src/Camera.cpp
    ../src/EventHandler.cpp
)

# Include directories (to find headers in ../include)
target_include_directories(RayTracingEngineTests PRIVATE ../include)

# Link dependencies
target_link_libraries(RayTracingEngineTests PRIVATE
    gtest_main
    gtest
    glm::glm
    glfw
    glad
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(RayTracingEngineTests)
