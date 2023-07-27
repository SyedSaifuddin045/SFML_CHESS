cmake_minimum_required(VERSION 3.8)
project(SFML_CHESS)

# Enable export of compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

# Set directories
set(include_dir "${PROJECT_SOURCE_DIR}/include/")
set(libraries "${PROJECT_SOURCE_DIR}/lib/")
set(source_dir "${PROJECT_SOURCE_DIR}/src/")

# Collect source and header files
file(GLOB_RECURSE source_files "${source_dir}/*.cpp" "${source_dir}/*.c")
file(GLOB_RECURSE header_files "${include_dir}/*.h")

# Include directories
include_directories(${include_dir})

# Create the executable target
add_executable(${PROJECT_NAME} ${source_files} ${header_files})
add_compile_definitions(RESOURCE="${CMAKE_BINARY_DIR}/bin/res")

# Link libraries based on the build configuration and system
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(libraries "${PROJECT_SOURCE_DIR}/lib/Debug/")
  target_link_directories(SFML_CHESS PRIVATE ${libraries})
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(SFML_CHESS PRIVATE sfml-audio-d sfml-graphics-d sfml-network-d sfml-system-d sfml-window-d opengl32)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(SFML_CHESS PRIVATE sfml-audio sfml-graphics sfml-network sfml-system sfml-window)
  endif()
else()
  target_link_directories(SFML_CHESS PRIVATE ${libraries})
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(SFML_CHESS PRIVATE sfml-audio sfml-graphics sfml-network sfml-system sfml-window opengl32)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(SFML_CHESS PRIVATE sfml-audio sfml-graphics sfml-network sfml-system sfml-window)
  endif()
endif()

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Copy SFML DLLs to the output directory (Windows)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(TARGET SFML_CHESS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bin $<TARGET_FILE_DIR:SFML_CHESS>)
endif()
