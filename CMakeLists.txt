cmake_minimum_required(VERSION 3.20)
project(LockedAndFlow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point to your SFML installation
set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML/lib/cmake/SFML)

# Use corrected component names (capitalized)
find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics Audio)

# Add executable
add_executable(LockedAndFlow src/main.cpp "src/Timer.h" "src/Timer.cpp" "src/TimerDisplay.h" "src/TimerDisplay.cpp")

# Link with corrected target names (SFML:: namespace)
target_link_libraries(LockedAndFlow PRIVATE 
    SFML::System 
    SFML::Window 
    SFML::Graphics 
    SFML::Audio
)

# Copy SFML DLLs to output directory (Windows)
if(WIN32)
    add_custom_command(TARGET LockedAndFlow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML/bin
        $<TARGET_FILE_DIR:LockedAndFlow>)
endif()