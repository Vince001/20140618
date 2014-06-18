## 2013-5-25 V1.0 Vince create.

MAINTAINERCLEANFILES = Makefile.in

#AUTOMAKE_OPTIONS = subdir-objects

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

#SUB_DIRS = src ...

bin_PROGRAMS = camera

camera_SOURCES = camera.cpp camera.h main.cpp
			
#camera_CPPFLAGS = 
AM_CPPFLAGS = -I /usr/local/include/ -g -O3 -funroll-loops -fomit-frame-pointer
				
## opencv 24 or 12 libs	
## Use package install. opencv boost			
camera_LDADD =/usr/lib/libopencv_highgui.so.2.3.1 \
/usr/lib/libopencv_ml.so.2.3.1 \
/usr/lib/libopencv_imgproc.so.2.3.1 \
/usr/lib/libopencv_video.so.2.3.1 \
/usr/lib/libopencv_core.so.2.3.1 \
/usr/lib/libopencv_legacy.so.2.3.1 \
/usr/lib/libopencv_contrib.so.2.3.1 \
/usr/lib/libopencv_objdetect.so.2.3.1 \
/usr/lib/libopencv_flann.so.2.3.1 \
/usr/lib/libopencv_features2d.so.2.3.1 \
/usr/lib/libopencv_calib3d.so.2.3.1 \
/usr/lib/libopencv_gpu.so.2.3.1 \
/usr/local/lib/libv4l1.so.0 \
/usr/local/lib/libv4l2.so.0 \
/usr/local/lib/libv4lconvert.so.0


