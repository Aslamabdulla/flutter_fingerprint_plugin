cmake_minimum_required(VERSION 3.10)

project(my_fingerprint_plugin)

set(PLUGIN_NAME "my_fingerprint_plugin_plugin")

add_library(${PLUGIN_NAME} SHARED
  "src/my_fingerprint_plugin_plugin.cpp"
)

target_include_directories(${PLUGIN_NAME} PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  ${PLUGIN_SOURCES}
)
# Detect architecture and set the correct library paths
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  # 64-bit
  set(SGFPLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgfplib.dll")
  set(SGFPAMX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgfpamx.dll")
  set(SGWSQLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgwsqlib.dll")
  set(SGFDUSDA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgfdusdax64.dll")
  set(SGBLEDEV_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgbledev.dll")
  set(SGFPLIB_LIB "${CMAKE_CURRENT_SOURCE_DIR}/x64/sgfplib.lib")  # Add this line for the .lib file
else()
  # 32-bit
  set(SGFPLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgfplib.dll")
  set(SGFPAMX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgfpamx.dll")
  set(SGWSQLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgwsqlib.dll")
  set(SGFDUSDA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgfdusda.dll")
  set(SGBLEDEV_PATH "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgbledev.dll")
  set(SGFPLIB_LIB "${CMAKE_CURRENT_SOURCE_DIR}/x86/sgfplib.lib")  # Add this line for the .lib file
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

# Link the SDK libraries and .lib file
target_link_libraries(${PLUGIN_NAME} PRIVATE
  "${SGFPLIB_PATH}"
  "${SGFPAMX_PATH}"
  "${SGWSQLIB_PATH}"
  "${SGFDUSDA_PATH}"
  "${SGBLEDEV_PATH}"
  "${SGFPLIB_LIB}" 
  PARENT_SCOPE # Link the .lib file
)

# Link to Flutter's Windows library
target_link_libraries(${PLUGIN_NAME} PRIVATE "${FLUTTER_LIBRARY}")
