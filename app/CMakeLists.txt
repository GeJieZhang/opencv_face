#================================================================================
#======================================================================cmake版本
#================================================================================
cmake_minimum_required(VERSION 3.4.1)





#================================================================================
#======================================================================声明资源目录
#================================================================================
set(JNI_DIR "${CMAKE_SOURCE_DIR}/src/main/jni")
set(CPP_DIR "${CMAKE_SOURCE_DIR}/src/main/cpp")








#================================================================================
#=========================================================================引入资源
#================================================================================
include_directories(${JNI_DIR}/include/)








#================================================================================
#=============================================================================系统
#================================================================================
add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${CPP_DIR}/native-lib.cpp )


find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )



target_link_libraries( # Specifies the target library.
                       native-lib
                       libopencv_java3
                       jnigraphics


                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )




#================================================================================
#==================================================================添加第3方库的引用
#================================================================================
add_library(libopencv_java3
            SHARED
            IMPORTED)
set_target_properties(libopencv_java3
                      PROPERTIES IMPORTED_LOCATION
                      ${JNI_DIR}/${ANDROID_ABI}/libopencv_java3.so )

add_library(opencv
            SHARED
            IMPORTED)
set_target_properties(opencv
                      PROPERTIES IMPORTED_LOCATION
                      ${JNI_DIR}/include/opencv2/opencv.hpp )