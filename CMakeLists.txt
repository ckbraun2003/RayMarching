cmake_minimum_required(VERSION 3.10)

project(RayTracingEngine)

# Optional: Set toolchain if running via command line
# Note: Pass -DCMAKE_TOOLCHAIN_FILE=[path/to/vcpkg]/scripts/buildsystems/vcpkg.cmake when configuring

# Find libraries via vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

message(STATUS "GLAD_INCLUDE_DIRS is: ${glad_INCLUDE_DIRS}")

# Add source files
add_executable(RayTracingEngine
    main.cpp
    src/Camera.cpp
    src/EventHandler.cpp
    src/GpuRayTracer.cpp
    src/CpuRayTracer.cpp
    src/World.cpp
)

# Link libraries properly
target_link_libraries(RayTracingEngine PRIVATE glfw glad::glad glm::glm)

# Include your own headers
target_include_directories(RayTracingEngine PRIVATE include)

# Set C++ standard
set_target_properties(RayTracingEngine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)

# --- Testing Setup ---
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Copy shaders to build directory
add_custom_command(TARGET RayTracingEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:RayTracingEngine>/shaders
)

add_subdirectory(tests)