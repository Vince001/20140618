cmake_minimum_required(VERSION 2.8)

project(CMake_project)
# Revise version info:
# V1.0 by Vince make it works well in windows 2012-11-11
# v1.1 by Vince add path variable to file describe 2012-11-19
# v2.0 by Vince make it works well in linux 2012-12-7
# =============================system configure ==========================>>
option(Release_mode "" OFF)
option(USE_SOURCE_DIR "use source dir to load the file" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(USE_SOURCE_DIR)
set(SOURCE_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src CACHE PATH "the dir of source")
set(HEADER_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/header CACHE PATH "the dir of header")
endif(USE_SOURCE_DIR)

# find_package(OpenCV REQUIRED) used for removable software
# the cmake_find file OpenCV_ICG can't use in linux, but in windows is OK.

# select debug/release mode
if(WIN32)

else(WIN32)
	if(Release_mode)
	SET(CMAKE_BUILD_TYPE Release)
	SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops -fomit-frame-pointer")
	SET(CMAKE_C_FLAGS_RELEASE "-O3 -funroll-loops -fomit-frame-pointer")
	else(Release_mode)
	SET(CMAKE_BUILD_TYPE Debug)
	SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
	SET(CMAKE_C_FLAGS_DEBUG "-ggdb")
	endif(Release_mode)
endif(WIN32)

# indicate the lib path of currnet system
if(WIN32)
# Note the \ must be / in windows 32 bit
set(OPENCV_DIR "input the path of opencv compile" CACHE PATH "Note the version of OpenCV")
set(OPENCV_LIB_DIR "D:/Program Files/opencv/vc2010/install/lib/" CACHE PATH "lib")
set(OPENCV_INCLUDE_DIR "D:/Program Files/opencv/vc2010/install/include" CACHE PATH "header")
set(OPENCV_RELEASE_LIB_DIR "D:/Program Files/opencv/vc2010/lib/Release")
else(WIN32)
set(OPENCV_INCLUDE_DIR "/home/vince/Documents/src/calibration_src/header" CACHE PATH "header")
set(OPENCV_INCLUDE_DIR ${OPENCV_INCLUDE_DIR} "/home/vince/Documents/src/calibration_src/header/opencv" CACHE PATH "header")
set(OPENCV_INCLUDE_DIR ${OPENCV_INCLUDE_DIR} "/home/vince/Documents/src/calibration_src/header/opencv2" CACHE PATH "header")
#set(Boost_INCLUDE_DIR "/home/frank/Documents/install/boost_1_50_0/" CACHE PATH "header")
set(SYS_HEADER "/usr/include")
set(OPENCV_LIB_DIR "/home/vince/Documents/src/calibration_src/opencv_lib")
find_package( OpenCV )
#if( OpenCV_FOUND )
#  message("Find opencv !!!")
#  message(${OpenCV_INCLUDE_DIRS})
#endif()
#find_package( Boost )
#if(Boost_FOUND)
#  include_directories(${Boost_INCLUDE_DIRS})
#  message(${Boost_INCLUDE_DIRS})
#endif()
endif(WIN32)

# select build target platform ===========================>>
if(USE_SOURCE_DIR)
add_executable(camera
    main.cpp

    camera.cpp
    camera.h

    #${SYS_HEADER}/linux/videodev2.h
    #/usr/local/include/libv4l1-videodev.h
)
else(USE_SOURCE_DIR)
add_executable(camera
    main.cpp

	Robot_Calibration.cpp
	Robot_Calibration.h

	Matrix_Pointer_Operation.cpp
	Matrix_Pointer_Operation.h

	Camera_Calibration.cpp
	Camera_Calibration.h

	#if(Have_IO_csv)
	IO_csv.cpp
	IO_csv.h
	#endif(Have_IO_csv)
)
endif(USE_SOURCE_DIR)

if(WIN32)
include_directories(
    ${OPENCV_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	"D:/Program Files/opencv/vc2010/install/include/opencv"
)
link_directories(
    ${OPENCV_LIB_DIR}
    ${OPENCV_LINK_DIR}
	${OpenCV_lib}
	${OPENCV_RELEASE_LIB_DIR}
    ${Boost_INCLUDE_DIR}
)

else(WIN32)
include_directories(
    ${OPENCV_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SYS_HEADER}
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)
link_directories(
    ${OPENCV_LIB_DIR}
	${OPENCV_LINK_DIR}
	${OpenCV_lib}
)
endif(WIN32)

#message(${OpenCV_LIBRARIES})

target_link_libraries(camera
# if your system install the libs, so the following steps is redunt.
        /usr/local/lib/libopencv_calib3d.so.2.3.1
    /usr/local/lib/libopencv_contrib.so.2.3.1
    /usr/local/lib/libopencv_core.so.2.3.1
    /usr/local/lib/libopencv_features2d.so.2.3.1
    /usr/local/lib/libopencv_flann.so.2.3.1
    /usr/local/lib/libopencv_gpu.so.2.3.1
	/usr/local/lib/libopencv_highgui.so.2.3.1
	/usr/local/lib/libopencv_imgproc.so.2.3.1
	/usr/local/lib/libopencv_legacy.so.2.3.1
	/usr/local/lib/libopencv_ml.so.2.3.1
	#/usr/local/lib/libopencv_nonfree.so.2.3.1
	/usr/local/lib/libopencv_objdetect.so.2.3.1
	#/usr/local/lib/libopencv_photo.so.2.3.1
	#/usr/local/lib/libopencv_stitching.so.2.3.1
	/usr/local/lib/libopencv_ts.so.2.3.1
	/usr/local/lib/libopencv_video.so.2.3.1
	#/usr/local/lib/libopencv_videostab.so.2.3.1
	/usr/local/lib/libv4l1.so
	/usr/local/lib/libv4l1.so.0
	/usr/local/lib/libv4l2.so
	/usr/local/lib/libv4l2.so.0
	/usr/local/lib/libv4lconvert.so
	/usr/local/lib/libv4lconvert.so.0
	
	/usr/local/lib/libboost_program_options.so
	${OpenCV_LIBRARIES}
	${Boost_LIBRARIES}
    # ${OpenCV_LIBRARIES}
    # use "path/lib_name.so.2.3.1", usr/bin/ld could find the lib
)

# ====================== Details =========================>>
# lastest modify-date is Dec-7-2012
# CMakelists.txt can use in windows, but linux only read CMakeLists.txt
# In windows, the class fistream constructor can use string *, but in linux only char *
# In windows, the path represent use '//' but in linux is '/'
# the cmake_find file OpenCV_ICG can't use in linux, but in windows is OK.
