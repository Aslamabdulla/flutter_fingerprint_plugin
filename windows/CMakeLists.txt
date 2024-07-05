cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME "my_fingerprint_plugin")
project(${PROJECT_NAME} LANGUAGES CXX)

cmake_policy(VERSION 3.14...3.24)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "${PROJECT_NAME}_plugin")

# List all your plugin source files here
list(APPEND PLUGIN_SOURCES
  "src/my_fingerprint_plugin_plugin.cpp"
)

add_library(${PLUGIN_NAME} SHARED
"src/my_fingerprint_plugin_plugin.cpp"
  "include/my_fingerprint_plugin/my_fingerprint_plugin_c_api.h"
  ${PLUGIN_SOURCES}
)

apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin)

# Set paths for DLL and LIB files
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  # 64-bit
  set(SGFPLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgfplib.dll")
  set(SGFPAMX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgfpamx.dll")
  set(SGWSQLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgwsqlib.dll")
  set(SGFDUSDA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgfdusdax64.dll")
  set(SGBLEDEV_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgbledev.dll")
  set(SGFPLIB_LIB "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgfplib.lib")
else()
  # 32-bit
  set(SGFPLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgfplib.dll")
  set(SGFPAMX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgfpamx.dll")
  set(SGWSQLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgwsqlib.dll")
  set(SGFDUSDA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgfdusda.dll")
  set(SGBLEDEV_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgbledev.dll")
  set(SGFPLIB_LIB "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgfplib.lib")
endif()

# Ensure the libraries and .lib file exist
if(NOT EXISTS "${SGFPLIB_PATH}")
  message(FATAL_ERROR "sgfplib.dll not found in ${SGFPLIB_PATH}")
endif()
if(NOT EXISTS "${SGFPAMX_PATH}")
  message(FATAL_ERROR "sgfpamx.dll not found in ${SGFPAMX_PATH}")
endif()
if(NOT EXISTS "${SGWSQLIB_PATH}")
  message(FATAL_ERROR "sgwsqlib.dll not found in ${SGWSQLIB_PATH}")
endif()
if(NOT EXISTS "${SGFDUSDA_PATH}")
  message(FATAL_ERROR "sgfdusda.dll not found in ${SGFDUSDA_PATH}")
endif()
if(NOT EXISTS "${SGBLEDEV_PATH}")
  message(FATAL_ERROR "sgbledev.dll not found in ${SGBLEDEV_PATH}")
endif()
if(NOT EXISTS "${SGFPLIB_LIB}")
  message(FATAL_ERROR "sgfplib.lib not found in ${SGFPLIB_LIB}")
endif()

# Copy the necessary DLL files to the build directory
add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${SGFPLIB_PATH}" $<TARGET_FILE_DIR:${PLUGIN_NAME}>
)
add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${SGFPAMX_PATH}" $<TARGET_FILE_DIR:${PLUGIN_NAME}>
)
add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${SGWSQLIB_PATH}" $<TARGET_FILE_DIR:${PLUGIN_NAME}>
)
add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${SGFDUSDA_PATH}" $<TARGET_FILE_DIR:${PLUGIN_NAME}>
)
add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${SGBLEDEV_PATH}" $<TARGET_FILE_DIR:${PLUGIN_NAME}>
)

# Link the SDK libraries and .lib file
target_link_libraries(${PLUGIN_NAME} PRIVATE
  "${SGFPLIB_LIB}"
)

# Link to Flutter's Windows library
target_link_libraries(${PLUGIN_NAME} PRIVATE "${FLUTTER_LIBRARY}")

# === Tests ===

if(${include_${PROJECT_NAME}_tests})
  set(TEST_RUNNER "${PROJECT_NAME}_test")
  enable_testing()
  
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.11.0.zip
  )
  
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF CACHE BOOL "Disable installation of googletest" FORCE)

  FetchContent_MakeAvailable(googletest)

  add_executable(${TEST_RUNNER}
    test/mocks.h
    test/plugin_test.cpp
    ${PLUGIN_SOURCES}
  )
  apply_standard_settings(${TEST_RUNNER})
  target_include_directories(${TEST_RUNNER} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
  target_link_libraries(${TEST_RUNNER} PRIVATE flutter_wrapper_plugin)
  target_link_libraries(${TEST_RUNNER} PRIVATE gtest_main gmock)
  
  add_custom_command(TARGET ${TEST_RUNNER} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FLUTTER_LIBRARY}" $<TARGET_FILE_DIR:${TEST_RUNNER}>
  )

  include(GoogleTest)
  gtest_discover_tests(${TEST_RUNNER})
endif()
