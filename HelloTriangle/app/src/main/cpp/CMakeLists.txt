# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/shader_helper
        ${CMAKE_CURRENT_SOURCE_DIR}/draw
        ${CMAKE_CURRENT_SOURCE_DIR}/header_out
)

set (LIST_SRC_FILES  native.cpp processor.cpp)

list(APPEND LIST_SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/shader_helper/Shader_Helper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/draw/draw_utils.cpp
        )

add_library( # Sets the name of the library.
             processor

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${LIST_SRC_FILES})

set(LIST_TARGET_LINK  GLESv3 log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       processor

                       # Links the target library to the log library
                       # included in the NDK.
                       ${LIST_TARGET_LINK} )