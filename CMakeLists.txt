cmake_minimum_required(VERSION 3.8)
project(SFML_CHESS)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Enable export of compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

# Set directories
set(include_dir "${PROJECT_SOURCE_DIR}/include/")
set(libraries "${PROJECT_SOURCE_DIR}/lib/")
set(source_dir "${PROJECT_SOURCE_DIR}/src/")

# Set library search paths
link_directories(${libraries})

# Collect source and header files
file(GLOB source_files "${source_dir}/*.cpp" "${source_dir}/*.c")
file(GLOB header_files "${include_dir}/*.h")

# Include directories
include_directories(${include_dir})

# Set the library directory based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(libraries "${PROJECT_SOURCE_DIR}/lib/Debug/")
else()
  set(libraries "${PROJECT_SOURCE_DIR}/lib/")
endif()

# Create the executable target
add_executable(SFML_CHESS ${source_files} ${header_files})

# Specify library search directories
target_link_directories(SFML_CHESS PRIVATE ${libraries})

# Link libraries based on the build configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_libraries(SFML_CHESS PRIVATE sfml-audio-d sfml-graphics-d sfml-network-d sfml-system-d sfml-window-d)
else()
  target_link_libraries(SFML_CHESS PRIVATE sfml-audio sfml-graphics sfml-network sfml-system sfml-window)
endif()

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Copy SFML DLLs to the output directory
add_custom_command(TARGET SFML_CHESS POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/bin $<TARGET_FILE_DIR:SFML_CHESS>)
