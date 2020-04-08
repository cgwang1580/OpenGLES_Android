# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(TARGET_ARCH_FLAG ${ANDROID_ABI})
set(TARGET_ABI ${TARGET_ARCH_FLAG})
message(STATUS "TARGET_ABI is ${TARGET_ABI}")

set(PROCESSOR_SO_NAME processor.draw)

set(IF_USE_STATIC_PNG_LIB 1)
if (IF_USE_STATIC_PNG_LIB EQUAL 1)
    set(LIB_PNG_NAME png16_static)
else ()
    set(LIB_PNG_NAME png16)
endif ()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/header_out
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/draw
        ${CMAKE_CURRENT_SOURCE_DIR}/shader_helper
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${CMAKE_CURRENT_SOURCE_DIR}/libpng
)

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/source_lib/${TARGET_ABI}
)

set(LIST_SRC_FILES native.cpp)

list(APPEND LIST_SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/shader_helper/Shader_Helper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/draw/draw_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/processor.cpp
        )

add_library(${PROCESSOR_SO_NAME} SHARED ${LIST_SRC_FILES})

set(LIST_TARGET_LINK ${LIB_PNG_NAME} GLESv3 log)

if (IF_USE_STATIC_PNG_LIB EQUAL 1)
    list(APPEND LIST_TARGET_LINK z)
endif()

target_link_libraries(
        ${PROCESSOR_SO_NAME}
        ${LIST_TARGET_LINK}
)

if (IF_USE_STATIC_PNG_LIB EQUAL 0)
    add_custom_command(
            TARGET ${PROCESSOR_SO_NAME}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/source_lib/${TARGET_ABI}/lib${LIB_PNG_NAME}.so ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/${TARGET_ABI}/lib${LIB_PNG_NAME}.so
    )
endif ()
