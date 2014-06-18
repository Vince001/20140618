/**
@file camera.h
@brief The camera class declaration file.

  - Version: 1.0

  - Function List:
    -# camera class declaration.

  - Dependencies:
    -# V4L2
    -# OpenCV
    -# Boost::program_option

  - Notice:
     -# .

  - Modify Record:
    -#      Mar27,2013. Vince create.
    -# V1.0 Mar27,2013. Vince complete.
*/

#ifndef __Camera_h_
#define __Camera_h_

#define HAVE_CAMV4L   ///< like name
#define HAVE_CAMV4L2  ///< like name

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <asm/types.h>          /* for videodev2.h */
#include <libv4l1-videodev.h>
#include <linux/videodev2.h>
#include <libv4l1.h>
#include <libv4l2.h>
#include <opencv2/opencv.hpp>

#include <ctype.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <sstream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

namespace po = boost::program_options;

#define CLEAR(x) memset (&(x), 0, sizeof (x))   ///< a macro function
//#define CLEAR(x,y) memcpy (&(x), 0, sizeof (x))

/* Defaults - If your board can do better, set it here.  Set for the most common type inputs. */
#define DEFAULT_V4L_WIDTH  640  ///< default width size to open camera
#define DEFAULT_V4L_HEIGHT 480  ///< default height size to open camera

#define CHANNEL_NUMBER 1  ///< set the camera camera num,VIDIOC_ENUMINPUT
#define MAX_CAMERAS 8   ///< the max search cycles counter

#define MAX_V4L_BUFFERS 10   ///< default and maximum number of V4L buffers, not including last, 'special' buffer
#define DEFAULT_V4L_BUFFERS 4  ///< the V4L device buffer amount will be allocated.

#define USE_TEMP_BUFFER  ///< if enabled, copies data from the buffer. this uses a bit more memory,but much more reliable for some UVC cameras
#define MAX_DEVICE_DRIVER_NAME 80  ///< the max size of device name


#define Photo_name_type_cnt 5  ///< the counter of photo type
#define P_type_leng 5   ///< the length of type name of photo

#define Resolution_cnt 12   ///< resolution counter, data will load from config file

#define DEBUG 0  ///< a macro define flag for debug

#define DEBUG_EXT_SYNC 1  ///< for take photo with robot

#define Display_output 1  ///< control wether console output.

#define CONFIG_FILE ("config.cfg")   ///< the config file of camera class and object

/* Device Capture Objects */
/* V4L2 structure */
/// V4L2 structure
struct buffer
{
		void * start;   ///< a void pointor to point the start position
		size_t length;   ///< the size of itselt
};

/// A struct contain datails of an item.
typedef struct v4l2_ctrl_range
{
		__u32 ctrl_id;   ///< stored corresponding id of following details
		__s32 initial_value;   ///< stored corresponding init value of id
		__s32 current_value;   ///< stored corresponding current value of id
		__s32 minimum;   ///< stored corresponding minimum value of id
		__s32 maximum;   ///< stored corresponding maximum value of id
		__s32 default_value;  ///< stored corresponding default value of id
} v4l2_ctrl_range;

/// A struct contain all elements about the camera which inherited from old camera class.
typedef struct CvCaptureCAM_V4L
{
		char* deviceName;   ///< device name
		int deviceHandle;   ///< file descriptor returned by open() function
		int bufferIndex;    ///< buffer index
		int FirstCapture;   ///< first captrue flag

		int width;   ///< the resolution width
		int height;   ///< the resolution height

		struct video_capability capability;    ///< like name
		struct video_window captureWindow;   ///< like name
		struct video_picture imageProperties;   ///< like name
		struct video_mbuf memoryBuffer;   ///< like name
		struct video_mmap *mmaps;    ///< like name
		char *memoryMap;    ///< like name
		IplImage frame;   ///< like name

		/* V4L2 variables */
		buffer buffers[MAX_V4L_BUFFERS + 1];  ///< allocate buffer
		struct v4l2_capability cap;     ///< query device details struct will be fill by driver
		struct v4l2_input inp;   ///< like name
		struct v4l2_format form;   ///< resolution and pixel format setting
		struct v4l2_crop crop;   ///< like name
		struct v4l2_cropcap cropcap;   ///< like name
		struct v4l2_requestbuffers req;   ///< like name
		struct v4l2_jpegcompression compr;   ///< like name
		struct v4l2_control control;   ///< user base control struct
		struct v4l2_ext_control ext_ctrl;  ///< user extend control struct
		struct v4l2_ext_controls ext_ctrls; ///< user extend control struct include ext_ctrl
		enum v4l2_buf_type type;    ///< like name
		struct v4l2_queryctrl queryctrl;  ///< query control details
		struct v4l2_querymenu querymenu;   ///< qurry menu list

		/* V4L2 control variables */
		v4l2_ctrl_range** v4l2_ctrl_ranges;   ///< a point will be stored camera item's detail reallocate many times.
		int v4l2_ctrl_count;   ///< a control item counter
		int is_v4l2_device;   ///< a flag wether is v4l2 device
} CvCaptureCAM_V4L;

/// A int array used by validation in boost.
typedef struct _cfg_resoltype_Arr
{
  int arr[ Resolution_cnt ];   ///< an array of resolution for boost
} cfg_resoltype_Arr;

/// A string array used by validation in boost.
typedef struct _cfg_ptype_Arr
{
  std::string arr[ Photo_name_type_cnt ];   ///< an array of photo type for boost
} cfg_ptype_Arr;

/// assign the simple command name with their true value
enum cam_ctrl_item
{
  BRIGHTNESS = V4L2_CID_BRIGHTNESS,
  CONTRAST = V4L2_CID_CONTRAST,
  SATURATION = V4L2_CID_SATURATION,
  AUTO_WHITE_BALANCE = V4L2_CID_AUTO_WHITE_BALANCE,
  GAIN = V4L2_CID_GAIN,
  WHITE_BALANCE_TEMPERATURE = V4L2_CID_WHITE_BALANCE_TEMPERATURE,
  SHARPNESS = V4L2_CID_SHARPNESS,
  BACKLIGHT_COMPENSATION = V4L2_CID_BACKLIGHT_COMPENSATION,
  EXPOSURE_AUTO = V4L2_CID_EXPOSURE_AUTO,
  EXPOSURE_ABSOLUTE = V4L2_CID_EXPOSURE_ABSOLUTE,
  EXPOSURE_AUTO_PRIORITY = V4L2_CID_EXPOSURE_AUTO_PRIORITY,
  FOCUS_ABSOLUTE = V4L2_CID_FOCUS_ABSOLUTE,
  FOCUS_AUTO = V4L2_CID_FOCUS_AUTO,
} ;

/// A struct contains almost all elements of one control item. Used as link list node.
typedef struct _cam_val
{
  struct v4l2_control control;           ///< user base control struct
  struct v4l2_queryctrl queryctrl;      ///< query control struct
  struct v4l2_querymenu querymenu;      ///< query menu list struct
  struct v4l2_ext_control ext_ctrl;     ///< user extend control struct
  struct v4l2_ext_controls ext_ctrls;   ///< user extend control struct include ext_ctrl

  int cnt;   ///< indicate the sequence and counter
  int user_value; ///< tmp to store the value from camera setting file

  struct _cam_val *last;   ///< the link list last link pointor
  struct _cam_val *next;   ///< the link list next link pointor
} cam_val;

/**
@class Camera
@brief camera class declaration.

  - Version 1.5

  - Description:
    -# Include nessary data and method.
    -# Construct a object can be used as single camera program.

  - Data Member:
    -# index     # Index which camera in use in current system.
    -# key       # User command buffer captured from keyboard.
    -# stop      # Stop flag of camera used by external call.
    -# ext_sync  # Sync flag,used to check .

  - Methods:
    -# help_menu() # Print help info so that you can use class object as a single program.
    -# display()   # Grab and scale a frame to the view window create by OpenCV.
    -# User_ctrl() # Execuate user command from keyboard.
    -# cv::Mat take_photo();                 # Capture a frame and reture a Mat.
    -# void take_photo( cv::Mat &var );      # Capture a frame and fill a Mat.
    -# void take_photo( char *photo_name );  # Take a photo use photo_name
    -# int set_cam_default( void );          # Set all camera inner parameters to default.
    -# int set_cam_ctrl( const int item, const int value );  # Set item(V4L2_CID_XXX or eg. Focus) with value.

  - Header List:
    -# std libs
    -# <boost/program_options.hpp>
    -# <boost/lexical_cast.hpp>
    -# <boost/optional.hpp>
    -# <libv4l1-videodev.h>
    -# <linux/videodev2.h>
    -# <libv4l1.h>
    -# <libv4l2.h>
    -# <opencv2/opencv.hpp>

  - Dependencies:
    -# V4L2
    -# OpenCV
    -# Boost::program_option

  - Inherence:
    -# NO

  - Notice:
    -# NO

  - Modify Record:
    -#      2013-3-7 Vince Create.
    -# V1.0 2013-5-22 Vince Complete.
    -# V1.1 May25,2013 Vince Add a function int load_cam_file( void ) to load camera
                       config fiel dynamically.Use current camera name.
    -# V1.2 May25,2013 Vince Add a parameter to write_cam_file.
    -# V1.3 May27,2013 Vince Add a function int write_config_file( void ); Generate
                             config file if it doesn't exists.
    -# V1.4 May27,2013 Vince Add a overload constructor function to index which camera.
    -# V1.5 May27,2013 Vince Change a member to static to counter the photo number.
*/
class Camera
{
private:
  int n_buffers ;
  int numCameras ;
  int indexList ;

  CvCaptureCAM_V4L * captureV4L;
  IplImage *Image;
  IplImage *Image_photo;
  IplImage *img ;
  IplImage *view_window ;
  struct v4l2_capability dev_cap;

  cam_val *cam_table ;  ///< camera setting table
  int cam_ctrl_cnt;  ///< record the counter of setting

/************ Resolution setting ************/
  int view_size_width ;
  int view_size_height ;
  int photo_size_width ;
  int photo_size_height ;
  int resol[Resolution_cnt][2];
  //int **resol;
  int resol_index ;

/************ Photo setting ***************/
  char save_image_name[1000][14] ;
  //char **save_image_name ;
  char *img_name_prefix ;
  //string img_prefix;
  char *img_name_suffix[Photo_name_type_cnt] ;
  int img_typ_index ;
  //static unsigned int pic_cnt ;
  unsigned int pic_cnt ;
  //unsigned int pic_cnt ;
  int pic_num_start;
  int photo_amount;

  std::string Win_Name;
  std::string cam_config_file;

public:
  int index;   ///< index which camera in current system
  int stop;    ///< stop flag
  char key ;   ///< key value buffer

#if DEBUG_EXT_SYNC
  int ext_sync;   ///< extern sync flag
#endif
  //cv::Mat a;

  // *************************functions*****************************
  Camera(const unsigned int cam_index, char *configfile,const char *camfile);
  Camera(const unsigned int cam_index);
  Camera();
  ~Camera();

private:
  int _capture_V4L(CvCaptureCAM_V4L *capture, char *deviceName);
  int _capture_V4L2(CvCaptureCAM_V4L *capture, char *deviceName);
  CvCaptureCAM_V4L * icvCaptureFromCAM_V4L(int index);
  void icvCloseCAM_V4L(CvCaptureCAM_V4L* capture);
  int icvGrabFrameCAM_V4L(CvCaptureCAM_V4L* capture);
  IplImage* icvRetrieveFrameCAM_V4L(CvCaptureCAM_V4L* capture, int);

  int icvSetPropertyCAM_V4L(CvCaptureCAM_V4L* capture, int property_id, double value);
  int icvSetVideoSize(CvCaptureCAM_V4L* capture, int w, int h);

  void icvInitCapture_V4L(void);
  int try_init_v4l(CvCaptureCAM_V4L* capture, char *deviceName);
  int try_init_v4l2(CvCaptureCAM_V4L* capture, char *deviceName);
  void v4l2_scan_controls_enumerate_menu(CvCaptureCAM_V4L* capture);
  void v4l2_free_ranges(CvCaptureCAM_V4L* capture);
  void v4l2_add_ctrl_range(CvCaptureCAM_V4L* capture, v4l2_control* ctrl);
  int v4l2_get_ctrl_current(CvCaptureCAM_V4L* capture, __u32 id);
  int v4l2_get_ctrl_default(CvCaptureCAM_V4L* capture, __u32 id);
  int v4l2_get_ctrl_min(CvCaptureCAM_V4L* capture, __u32 id);
  int v4l2_get_ctrl_max(CvCaptureCAM_V4L* capture, __u32 id);
  void v4l2_scan_controls(CvCaptureCAM_V4L* capture);
  int read_frame_v4l2(CvCaptureCAM_V4L* capture);
  void mainloop_v4l2(CvCaptureCAM_V4L* capture);
  int icvSetControl(CvCaptureCAM_V4L* capture, int property_id, double value);
  //void usr_det_capture(const char *name , usr_setting * usr , int onoff=1);
  void set_photograph_name( void );
  void set_focus( void );
  void set_exposure ( void );
  void set_brightness( void );
  int xioctl(int fd, int request, void *arg);

  int load_config_parameter( const char *configfile );
  int load_config_parameter( void );
  int load_cam_parameter( void );
  void c_2D_arr( int **p, int row, int coln );
  void c_2D_arr( char **p, int row, int coln );
  void gen_window_name( void );
  int set_cam_ctrl( const int num );
  void clean_link_list( void );
  void seek_last_point( cam_val *tmp );
  int write_cam_file( const char *filename, struct v4l2_capability *cap );
  int write_config_file( void );
  int load_cam_file( const char *filename );
  int load_cam_file( void );
  int link_to_begin( void );
  int link_to_end( void );
  void get_control_list( void );
  int refresh_cam_link( void );
  cam_val* seek_link_list_end( cam_val *p );
#if Display_output
  void list_ctrls( cam_val *input );
#endif

public:
#if Display_output
  void display();
  void help_menu( void );
#endif
  void User_ctrl( char key, char *photo_name );
  cv::Mat take_photo(); // return a Mat
  void take_photo( cv::Mat &var ); // fill a Mat
  void take_photo( char *photo_name );  // capture a photo with photo_name
  int set_cam_default( void );
  int set_cam_ctrl( const int item, const int value );
};

#endif
