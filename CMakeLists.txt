cmake_minimum_required(VERSION 3.20)
project(four_in_a_row LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable
add_executable(four_in_a_row
        src/four-in-a-row.cpp
        src/Board.cpp
        src/Rules.cpp
        src/GameController.cpp
)
if (WIN32)
    set_target_properties(four_in_a_row PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# Connect include folder
target_include_directories(four_in_a_row PRIVATE include)

# Windows libraries (WinAPI + GDI+)
if (WIN32)
    target_link_libraries(four_in_a_row PRIVATE gdiplus)
endif()

# Copy assets next to the executable so relative paths work
add_custom_command(TARGET four_in_a_row POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:four_in_a_row>/assets
)

# (Optional) Create stats folder next to exe on build (runtime can also do it)
add_custom_command(TARGET four_in_a_row POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:four_in_a_row>/stats
)

# Testing scripts
add_executable(tests
        tests/TestRules.cpp
        src/Board.cpp
        src/Rules.cpp
)

target_include_directories(tests PRIVATE include)

