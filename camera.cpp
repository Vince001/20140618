/**
@file camera.cpp
@brief The camera class definition file.

@version V1.0

  - Function List:
    -# int Camera::xioctl(int fd, int request, void *arg)
    -# void Camera::icvInitCapture_V4L
    -# int Camera::try_init_v4l(CvCaptureCAM_V4L* capture, char *deviceName)
    -# int Camera::try_init_v4l2(CvCaptureCAM_V4L* capture, char *deviceName)
    -# void Camera::v4l2_scan_controls_enumerate_menu(CvCaptureCAM_V4L* capture)
    -# void Camera::v4l2_free_ranges(CvCaptureCAM_V4L* capture)
    -# void Camera::v4l2_add_ctrl_range(CvCaptureCAM_V4L* capture, v4l2_control* ctrl)
    -# int Camera::v4l2_get_ctrl_current(CvCaptureCAM_V4L* capture, __u32 id)
    -# int Camera::v4l2_get_ctrl_default(CvCaptureCAM_V4L* capture, __u32 id)
    -# int Camera::v4l2_get_ctrl_min(CvCaptureCAM_V4L* capture, __u32 id)
    -# int Camera::v4l2_get_ctrl_max(CvCaptureCAM_V4L* capture, __u32 id)
    -# void Camera::v4l2_scan_controls(CvCaptureCAM_V4L* capture)
    -# int Camera::_capture_V4L2(CvCaptureCAM_V4L *capture, char *deviceName)
    -# int Camera::_capture_V4L(CvCaptureCAM_V4L *capture, char *deviceName)
    -# CvCaptureCAM_V4L * Camera::icvCaptureFromCAM_V4L(int index)
    -# int Camera::read_frame_v4l2(CvCaptureCAM_V4L* capture)
    -# void Camera::mainloop_v4l2(CvCaptureCAM_V4L* capture)
    -# int Camera::icvGrabFrameCAM_V4L(CvCaptureCAM_V4L* capture)
    -# IplImage* Camera::icvRetrieveFrameCAM_V4L(CvCaptureCAM_V4L* capture, int)
    -# int Camera::icvSetVideoSize(CvCaptureCAM_V4L* capture, int w, int h)
    -# int Camera::icvSetControl(CvCaptureCAM_V4L* capture, int property_id, double value)
    -# int Camera::icvSetPropertyCAM_V4L(CvCaptureCAM_V4L* capture, int property_id, double value)
    -# void Camera::icvCloseCAM_V4L(CvCaptureCAM_V4L* capture)
    -# void Camera::help_menu( void )
    -# cam_val* Camera::seek_link_list_end( cam_val *p )
    -# void Camera::clean_link_list( void )
    -# void Camera::get_control_list( void )
    -# void Camera::list_ctrls( cam_val *input )
    -# void Camera::set_photograph_name( void )
    -# void Camera::gen_window_name( void )
    -# void Camera::User_ctrl( char key, char *photo_name )
    -# int Camera::set_cam_ctrl( const int num )
    -# int Camera::set_cam_ctrl( const int item, const int value )
    -# int Camera::set_cam_default( void )
    -# int Camera::write_cam_file( const char *filename,struct v4l2_capability *cap )
    -# int write_config_file( void );
    -# int Camera::load_cam_file( const char *filename )
    -# int Camera::link_to_begin( void )
    -# int Camera::link_to_end( void )
    -# int Camera::refresh_cam_link( void )
    -# Camera::Camera( void )
    -# Camera::~Camera()
    -# void Camera::display()
    -# Mat Camera::take_photo()
    -# void Camera::take_photo( Mat &var )
    -# void Camera::take_photo( char *photo_name )
    -# void validate( boost::any &v, const vector<string> &values, cfg_resoltype_Arr*,int)
    -# void validate( boost::any &v, const vector<string> &values, cfg_ptype_Arr*,int)
    -# int Camera::load_config_parameter( void )
    -# void Camera::c_2D_arr( int **p, int row, int coln)
    -# void Camera::c_2D_arr( char **p, int row, int coln)

  - Dependencies:
    -# V4L2
    -# OpenCV
    -# Boost::program_option

  - Notice:
     -# .
     -# .

  - Modify Record:
    -# V1.0 Mar27,2013. Vince create.
    -# V1.0 May22,2013. Vince complete.
    -# V1.1 May25,2013. Vince Modify v4l2_scan_controls_enumerate_menu,close redundance perror function.
    -# V1.2 May25,2013. Vicne Add a function load_cam_file(void).
    -# V1.3 May27,2013. Vince Add a function int write_config_file( void );
*/

#include "camera.h"

using namespace std;
using namespace cv;

// IOCTL handling for V4L2
/**
@fn int Camera::xioctl(int fd, int request, void *arg)
@brief IO operation of V4L2 driver.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# fd       # Device file descriptor.
      -# request  # Command code which defined in libv4l2.h.eg.VIDIOC_QUERYCAP.
      -# arg      # The value for request is void type.

  - Return Value:
      -# result   # The result of v4l2_ioctl(fd, request, arg).

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
int Camera::xioctl(int fd, int request, void *arg)
{
	int r;

	do
	{
		r = v4l2_ioctl(fd, request, arg);
	}
	while (-1 == r && EINTR == errno);

	return r;
}

/**
@fn void Camera::icvInitCapture_V4L(void)
@brief Search cameras in current system.

  - Version V1.0

  - Description:
      -# Search cameras and mark corresponding bit of indexlist.
      -# The counter of camera is numCameras.
      -# Simple test program: Find number of Video Sources available.
         Start from 0 and go to MAX_CAMERAS while checking for the device with that name.
         If it fails on the first attempt of /dev/video0, then check if /dev/video is valid.
         Returns the global numCameras with the correct value (we hope).

  - Parameter List:
      -# void

  - Return Value:
      -# void

  - Dependencies:
      -# .

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void Camera::icvInitCapture_V4L(void)
{
  int deviceHandle;
	int CameraNumber;
	char deviceName[MAX_DEVICE_DRIVER_NAME];

	CameraNumber = 0;
	while (CameraNumber < MAX_CAMERAS)
	{
		/* Print the CameraNumber at the end of the string with a width of one character */
		sprintf(deviceName, "/dev/video%1d", CameraNumber);

		/* Test using an open to see if this new device name really does exists. */
		deviceHandle = open(deviceName, O_RDONLY);

		if (deviceHandle != -1)
		{
			/* This device does indeed exist - add it to the total so far */
			// add indexList
			indexList |= (1 << CameraNumber);
			numCameras++;
		}

		if (deviceHandle != -1)
			close(deviceHandle);

		/* Set up to test the next /dev/video source in line */
		CameraNumber++;
	}

}

/**
@fn int Camera::try_init_v4l(CvCaptureCAM_V4L* capture, char *deviceName)
@brief Try to open camera and get capability with V4L1 method.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# capture      # Camera property collection.
      -# devicename   # A char pointer to device name.Such as,/dev/video0.

  - Return Value:
      -# -1       # Device can't be open and get file descriptor.Close camera.
      -# 0        # Camera can be open,but can't be get capability.
      -# 1        # Device is a V4L1 device.It can be get capability.

  - Dependencies:
      -# V4L1.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
int Camera::try_init_v4l(CvCaptureCAM_V4L* capture, char *deviceName)
{
	// if detect = -1: Unable to open device
	// if detect = 0 : Detected nothing
	// if detect = 1 : V4L device
	int detect = 0;

	// Test device for V4L compability
	/* Test using an open to see if this new device name really does exists. */
	/* No matter what the name - it still must be opened! */
	capture->deviceHandle = v4l1_open(deviceName, O_RDWR);

	if (capture->deviceHandle == 0)
	{
		detect = -1;
		icvCloseCAM_V4L(capture);
	}

	if (detect == 0)
	{
		/* Query the newly opened device for its capabilities */
		if (v4l1_ioctl(capture->deviceHandle, VIDIOCGCAP, &capture->capability)
				< 0)
		{
			detect = 0;
			icvCloseCAM_V4L(capture);
		}
		else
		{
			detect = 1;
		}
	}

	return detect;
}

/**
@fn int Camera::try_init_v4l2(CvCaptureCAM_V4L* capture, char *deviceName)
@brief Try to open camera and get capabilitywith with V4L2 method.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# capture      # Camera property collection.
      -# devicename   # A char pointer to device name.Such as,/dev/video0.

  - Return Value:
      -# -1       # Device can't be open and get file descriptor.Close camera.
      -# 0        # Device can be open,but can't be get capability.
      -# 1        # Device is a V4L2 device.It can be get capability.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
int Camera::try_init_v4l2(CvCaptureCAM_V4L* capture, char *deviceName)
{
	// if detect = -1: Unable to open device
	// if detect = 0 : Detected nothing
	// if detect = 1 : V4L2 device
	int detect = 0;

	// Test device for V4L2 compability
	/* Open and test V4L2 device */
	capture->deviceHandle = v4l2_open(deviceName, O_RDWR /* required */| O_NONBLOCK, 0);

	if (capture->deviceHandle == 0)
	{
		detect = -1;
		icvCloseCAM_V4L(capture);
	}

	if (detect == 0)
	{
		CLEAR(capture->cap);
		if (-1 == xioctl(capture->deviceHandle, VIDIOC_QUERYCAP, &capture->cap))
		{
			detect = 0;
			icvCloseCAM_V4L(capture);
		}
		else
		{
			CLEAR(capture->capability);
			capture->capability.type = capture->cap.capabilities;

			/* Query channels number */
			if (-1 != xioctl(capture->deviceHandle, VIDIOC_G_INPUT, &capture->capability.channels))
			{
				detect = 1;
			}
		}
	}

	return detect;
}

/**
@fn void Camera::v4l2_scan_controls_enumerate_menu(CvCaptureCAM_V4L* capture)
@brief Enumerate menu of capture->queryctrl.id.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# capture  # Camera property collection.

  - Return Value:
      -# void.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .

  - Modify Record:
      -# V1.1 2013-5-25 Vince Debug for more querymenu Invalid arguments.Close perror function.
                              Because the menu list of item maybe not continuation.So,unnessary.
*/
void Camera::v4l2_scan_controls_enumerate_menu(CvCaptureCAM_V4L* capture)
{
//  printf (" Menu items:\n");
	CLEAR(capture->querymenu);
	capture->querymenu.id = capture->queryctrl.id;
	for (capture->querymenu.index = capture->queryctrl.minimum;
			(int) capture->querymenu.index <= capture->queryctrl.maximum;
			capture->querymenu.index++)
	{
		if (0 == xioctl(capture->deviceHandle, VIDIOC_QUERYMENU, &capture->querymenu))
		{
			//printf (" %s\n", capture->querymenu.name);
		}
		else
		{
			//perror("VIDIOC_QUERYMENU");
			//cout << "Can't find menu..." << endl;
		}
	}
}

/**
@fn void Camera::v4l2_free_ranges(CvCaptureCAM_V4L* capture)
@brief Free the memory of capture->v4l2_ctrl_ranges.

  - Version V1.0

  - Description:
      -# capture->v4l2_ctrl_ranges is a 2D pointer.
      -# A object of v4l2_ctrl_ranges include min max default step,etc.

  - Parameter List:
      -# capture  # Camera property collection.

  - Return Value:
      -# void

  - Dependencies:
      -# NULL.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void Camera::v4l2_free_ranges(CvCaptureCAM_V4L* capture)
{
	int i;

	if (capture->v4l2_ctrl_ranges != NULL)
	{
		for (i = 0; i < capture->v4l2_ctrl_count; i++)
		{
			/* Return device to initial values: */
			/* double value = (capture->v4l2_ctrl_ranges[i]->initial_value == 0)?0.0:((float)capture->v4l2_ctrl_ranges[i]->initial_value - capture->v4l2_ctrl_ranges[i]->minimum) / (capture->v4l2_ctrl_ranges[i]->maximum - capture->v4l2_ctrl_ranges[i]->minimum); */
			/* Return device to default values: */
			/* double value = (capture->v4l2_ctrl_ranges[i]->default_value == 0)?0.0:((float)capture->v4l2_ctrl_ranges[i]->default_value - capture->v4l2_ctrl_ranges[i]->minimum + 1) / (capture->v4l2_ctrl_ranges[i]->maximum - capture->v4l2_ctrl_ranges[i]->minimum); */

			/* icvSetPropertyCAM_V4L(capture, capture->v4l2_ctrl_ranges[i]->ctrl_id, value); */
			free(capture->v4l2_ctrl_ranges[i]);
		}
	}

	free(capture->v4l2_ctrl_ranges);
	capture->v4l2_ctrl_count = 0;
	capture->v4l2_ctrl_ranges = NULL;
}

/**
@fn void Camera::v4l2_add_ctrl_range(CvCaptureCAM_V4L* capture, v4l2_control* ctrl)
@brief Add a range object to the capture->v4l2_ctrl_ranges.

  - Version V1.0

  - Description:
      -# Copy id and value of v4l2_control to the range object.
      -# Copy min max default_value of capture->queryctrl to the range object.
      -# Rellocate memory for capture->v4l2_ctrl_ranges.

  - Parameter List:
      -# capture  # Camera property collection.
      -# ctrl     # A v4l2_control pointer which include id, value.

  - Return Value:
      -# void.

  - Dependencies:
      -# NULL.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void Camera::v4l2_add_ctrl_range(CvCaptureCAM_V4L* capture, v4l2_control* ctrl)
{
	v4l2_ctrl_range* range = (v4l2_ctrl_range*) malloc(sizeof(v4l2_ctrl_range));
	range->ctrl_id = ctrl->id;
	range->initial_value = ctrl->value;
	range->current_value = ctrl->value;
	range->minimum = capture->queryctrl.minimum;
	range->maximum = capture->queryctrl.maximum;
	range->default_value = capture->queryctrl.default_value;
	capture->v4l2_ctrl_ranges[capture->v4l2_ctrl_count] = range;
	capture->v4l2_ctrl_count += 1;
	capture->v4l2_ctrl_ranges = (v4l2_ctrl_range**) realloc((v4l2_ctrl_range**) capture->v4l2_ctrl_ranges,
                                                         (capture->v4l2_ctrl_count + 1) * sizeof(v4l2_ctrl_range*));
}

/**
@fn int Camera::v4l2_get_ctrl_current(CvCaptureCAM_V4L* capture, __u32 id)
@brief Get value of id store in capture->v4l2_ctrl_ranges.

  - Version V1.0

  - Description:
      -# capture->v4l2_ctrl_ranges is a 2D pointer.
      -# A v4l2_ctrl_ranges object include min max default_value,etc.
      -# Search capture->v4l2_ctrl_ranges with id to get value.

  - Parameter List:
      -# capture      #Camera property collection.
      -# property_id  #Command code of camera which define in libv4l2.h.
                       And stored in v4l2_ctrl_ranges object.

  - Return Value:
      -# -1       #If can't find id in capture->v4l2_ctrl_ranges.
      -# value    #The value of id had been store previously.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .


  - Modify Record:
      -# .
*/
int Camera::v4l2_get_ctrl_current(CvCaptureCAM_V4L* capture, __u32 id)
{
	for (int i = 0; i < capture->v4l2_ctrl_count; i++)
	{
		if (id == capture->v4l2_ctrl_ranges[i]->ctrl_id)
		{
			return capture->v4l2_ctrl_ranges[i]->current_value;
		}
	}
	return -1;
}

/**
@fn int Camera::v4l2_get_ctrl_default(CvCaptureCAM_V4L* capture, __u32 id)
@brief Get default value of id store in capture->v4l2_ctrl_ranges.

  - Version V1.0

  - Description:
      -# capture->v4l2_ctrl_ranges is a 2D pointer.
      -# A v4l2_ctrl_ranges object include min max default_value,etc.
      -# Search capture->v4l2_ctrl_ranges with id to get default value.

  - Parameter List:
      -# capture      # Camera property collection.
      -# property_id  # Command code of camera which define in libv4l2.h.
                       And stored in v4l2_ctrl_ranges object.

  - Return Value:
      -# -1       # If can't find id in capture->v4l2_ctrl_ranges.
      -# value    # The default value of id had been store previously.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .


  - Modify Record:
      -# .
*/
int Camera::v4l2_get_ctrl_default(CvCaptureCAM_V4L* capture, __u32 id)
{
	for (int i = 0; i < capture->v4l2_ctrl_count; i++)
	{
		if (id == capture->v4l2_ctrl_ranges[i]->ctrl_id)
		{
			return capture->v4l2_ctrl_ranges[i]->default_value;
		}
	}
	return -1;
}

/**
@fn int Camera::v4l2_get_ctrl_min(CvCaptureCAM_V4L* capture, __u32 id)
@brief Get minimum value of id store in capture->v4l2_ctrl_ranges.

  - Version V1.0

  - Description:
      -# capture->v4l2_ctrl_ranges is a 2D pointer.
      -# A v4l2_ctrl_ranges object include min max default_value,etc.
      -# Search capture->v4l2_ctrl_ranges with id to get minimum value.

  - Parameter List:
      -# capture      # Camera property collection.
      -# property_id  # Command code of camera which define in libv4l2.h.
                        And stored in v4l2_ctrl_ranges object.

  - Return Value:
      -# -1       # If can't find id in capture->v4l2_ctrl_ranges.
      -# value    # The minimum value of id had been store previously.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .


  - Modify Record:
      -# .
*/
int Camera::v4l2_get_ctrl_min(CvCaptureCAM_V4L* capture, __u32 id)
{
	for (int i = 0; i < capture->v4l2_ctrl_count; i++)
	{
		if (id == capture->v4l2_ctrl_ranges[i]->ctrl_id)
		{
			return capture->v4l2_ctrl_ranges[i]->minimum;
		}
	}
	return -1;
}

/**
@fn int Camera::v4l2_get_ctrl_max(CvCaptureCAM_V4L* capture, __u32 id)
@brief Get maximum value of id store in capture->v4l2_ctrl_ranges.

  - Version V1.0

  - Description:
      -# capture->v4l2_ctrl_ranges is a 2D pointer.
      -# A v4l2_ctrl_ranges object include min max default_value,etc.
      -# Search capture->v4l2_ctrl_ranges with id to get maximum value.

  - Parameter List:
      -# capture      # Camera property collection.
      -# property_id  # Command code of camera which define in libv4l2.h.
                        And stored in v4l2_ctrl_ranges object.

  - Return Value:
      -# -1       # If can't find id in capture->v4l2_ctrl_ranges.
      -# value    # The maximum value of id had been store previously.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .


  - Modify Record:
      -# .
*/
int Camera::v4l2_get_ctrl_max(CvCaptureCAM_V4L* capture, __u32 id)
{
	for (int i = 0; i < capture->v4l2_ctrl_count; i++)
	{
		if (id == capture->v4l2_ctrl_ranges[i]->ctrl_id)
		{
			return capture->v4l2_ctrl_ranges[i]->maximum;
		}
	}
	return -1;
}

/**
@fn void Camera::v4l2_scan_controls(CvCaptureCAM_V4L* capture)
@brief Scan control items and store in capture->v4l2_ctrl_ranges.

  - Version V1.0

  - Description:
      -# If capture->v4l2_ctrl_ranges isn't NULL,free it.
      -# capture->v4l2_ctrl_ranges is a 2D pointer.
      -# A v4l2_ctrl_ranges object include min max default_value,etc.

  - Parameter List:
      -# capture  # Camera property collection.

  - Return Value:
      -# void.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void Camera::v4l2_scan_controls(CvCaptureCAM_V4L* capture)
{
	__u32 ctrl_id;
	struct v4l2_control c;

	if (capture->v4l2_ctrl_ranges != NULL)
	{
		v4l2_free_ranges(capture);
	}

	capture->v4l2_ctrl_ranges = (v4l2_ctrl_range**) malloc(sizeof(v4l2_ctrl_range*));

#ifdef V4L2_CTRL_FLAG_NEXT_CTRL
	/* Try the extended control API first */
	capture->queryctrl.id = V4L2_CTRL_FLAG_NEXT_CTRL;
	if (0 == v4l2_ioctl(capture->deviceHandle, VIDIOC_QUERYCTRL, &capture->queryctrl))
	{
		do
		{
			c.id = capture->queryctrl.id;
			capture->queryctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;

			if (capture->queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
			{
				continue;
			}

			if (capture->queryctrl.type == V4L2_CTRL_TYPE_MENU)
			{
				v4l2_scan_controls_enumerate_menu(capture);
			}

			if (capture->queryctrl.type != V4L2_CTRL_TYPE_INTEGER &&
				capture->queryctrl.type != V4L2_CTRL_TYPE_BOOLEAN &&
				capture->queryctrl.type != V4L2_CTRL_TYPE_MENU)
			{
				continue;
			}

			if (v4l2_ioctl(capture->deviceHandle, VIDIOC_G_CTRL, &c) == 0)
			{
				v4l2_add_ctrl_range(capture, &c);
			}

		}
		while (0 == v4l2_ioctl(capture->deviceHandle, VIDIOC_QUERYCTRL, &capture->queryctrl));
	}
	else
#endif
	{
		/* Check all the standard controls */
		for (ctrl_id = V4L2_CID_BASE; ctrl_id < V4L2_CID_LASTP1; ctrl_id++)
		{
			capture->queryctrl.id = ctrl_id;

			if (v4l2_ioctl(capture->deviceHandle, VIDIOC_QUERYCTRL, &capture->queryctrl) == 0)
			{
				if (capture->queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
				{
					continue;
				}

				if (capture->queryctrl.type == V4L2_CTRL_TYPE_MENU)
				{
					v4l2_scan_controls_enumerate_menu(capture);
				}

				if (capture->queryctrl.type != V4L2_CTRL_TYPE_INTEGER
						&& capture->queryctrl.type != V4L2_CTRL_TYPE_BOOLEAN
						&& capture->queryctrl.type != V4L2_CTRL_TYPE_MENU )
				{
					continue;
				}

				c.id = ctrl_id;

				if (v4l2_ioctl(capture->deviceHandle, VIDIOC_G_CTRL, &c) == 0)
				{
					v4l2_add_ctrl_range(capture, &c);
				}
			}
		}

		/* Check any custom controls */
		for (ctrl_id = V4L2_CID_PRIVATE_BASE;; ctrl_id++)
		{
			capture->queryctrl.id = ctrl_id;

			if (v4l2_ioctl(capture->deviceHandle, VIDIOC_QUERYCTRL, &capture->queryctrl) == 0)
			{
				if (capture->queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
				{
					continue;
				}

				if (capture->queryctrl.type == V4L2_CTRL_TYPE_MENU)
				{
					v4l2_scan_controls_enumerate_menu(capture);
				}

				if (capture->queryctrl.type != V4L2_CTRL_TYPE_INTEGER
						&& capture->queryctrl.type != V4L2_CTRL_TYPE_BOOLEAN
						&& capture->queryctrl.type != V4L2_CTRL_TYPE_MENU)
				{
					continue;
				}

				c.id = ctrl_id;

				if (v4l2_ioctl(capture->deviceHandle, VIDIOC_G_CTRL, &c) == 0)
				{
					v4l2_add_ctrl_range(capture, &c);
				}
			}
			else
			{
				break;
			}
		}
	}
}

/**
@fn int Camera::_capture_V4L2(CvCaptureCAM_V4L *capture, char *deviceName)
@brief Init camera with V4L2 method.

  - Version V1.0

  - Description:
      -# Confirm devicename is V4L2 type.
      -# Get control ranges of capture.
      -# Enum input and set channel.
      -# Set resolution.
      -# Set pixel format(BGR24 24bits).
      -# Request buffer.
      -# Use memory map to init capture->buffers.
      -# Set frame resolution and allocate memory.

  - Parameter List:
      -# capture   # Camera property collection.
      -# deviename # The device name in system(eg./dev/video1)

  - Return Value:
      -# -1       # If driver is not V4L2 type.
      -# -1       # device %s is unable to capture video memory.
      -# -1       # V4L2: Aren't able to set channel number.
      -# -1       # V4L2: Could not obtain specifics of capture window.
      -# -1       # libv4l unable to ioctl S_FMT.
      -# -1       # libv4l unable convert to requested pixfmt.
      -# -1       # does not support memory mapping.
      -# -1       # can't querybuffer.
      -# -1       # Memory map error.
      -#  1       # If OK.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .


  - Modify Record:
      -# .
*/
int Camera::_capture_V4L2(CvCaptureCAM_V4L *capture, char *deviceName)
{
	int detect_v4l2 = 0;

	capture->deviceName = strdup(deviceName);

	detect_v4l2 = try_init_v4l2(capture, deviceName);

	if (detect_v4l2 != 1)
	{
		/* init of the v4l2 device is not OK */
		return -1;
	}

	/* starting from here, we assume we are in V4L2 mode */
	capture->is_v4l2_device = 1;

	capture->v4l2_ctrl_ranges = NULL;
	capture->v4l2_ctrl_count = 0;

	/* Scan V4L2 controls */
	v4l2_scan_controls(capture);

	if ((capture->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0)
	{
		/* Nope. */
		fprintf( stderr, "HIGHGUI ERROR: V4L2: device %s is unable to capture video memory.\n", deviceName);
		icvCloseCAM_V4L(capture);
		return -1;
	}

	/* The following code sets the CHANNEL_NUMBER of the video input.  Some video sources
	 have sub "Channel Numbers".  For a typical V4L TV capture card, this is usually 1.
	 I myself am using a simple NTSC video input capture card that uses the value of 1.
	 If you are not in North America or have a different video standard, you WILL have to change
	 the following settings and recompile/reinstall.  This set of settings is based on
	 the most commonly encountered input video source types (like my bttv card) */

	if (capture->inp.index > 0)
	{
		CLEAR(capture->inp);
		capture->inp.index = CHANNEL_NUMBER;
		/* Set only channel number to CHANNEL_NUMBER */

		/* V4L2 have a status field from selected video mode */
		if (-1 == xioctl(capture->deviceHandle, VIDIOC_ENUMINPUT, &capture->inp))
		{
			fprintf(stderr, "HIGHGUI ERROR: V4L2: Aren't able to set channel number\n");
			icvCloseCAM_V4L(capture);
			return -1;
		}
	}

	/* Find Window info */
	CLEAR(capture->form);
	capture->form.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(capture->deviceHandle, VIDIOC_G_FMT, &capture->form))
	{
		fprintf( stderr, "HIGHGUI ERROR: V4L2: Could not obtain specifics of capture window.\n\n");
		icvCloseCAM_V4L(capture);
		return -1;
	}

	/* libv4l will convert from any format to V4L2_PIX_FMT_BGR24 */
	CLEAR(capture->form);
	capture->form.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	capture->form.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
	capture->form.fmt.pix.field = V4L2_FIELD_ANY;
	capture->form.fmt.pix.width = capture->width;
	capture->form.fmt.pix.height = capture->height;

	if (-1 == xioctl(capture->deviceHandle, VIDIOC_S_FMT, &capture->form))
	{
		fprintf(stderr, "HIGHGUI ERROR: libv4l unable to ioctl S_FMT\n");
		return -1;
	}

	if (V4L2_PIX_FMT_BGR24 != capture->form.fmt.pix.pixelformat)
	{
		fprintf(stderr, "HIGHGUI ERROR: libv4l unable convert to requested pixfmt\n");
		return -1;
	}

	/* icvSetVideoSize(capture, DEFAULT_V4L_WIDTH, DEFAULT_V4L_HEIGHT); */

	unsigned int min;

	/* Buggy driver paranoia. */
	min = capture->form.fmt.pix.width * 2;

	if (capture->form.fmt.pix.bytesperline < min)
		capture->form.fmt.pix.bytesperline = min;

	min = capture->form.fmt.pix.bytesperline * capture->form.fmt.pix.height;

	if (capture->form.fmt.pix.sizeimage < min)
		capture->form.fmt.pix.sizeimage = min;

	CLEAR(capture->req);

	unsigned int buffer_number = DEFAULT_V4L_BUFFERS;

	try_again:

	capture->req.count = buffer_number;
	capture->req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	capture->req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(capture->deviceHandle, VIDIOC_REQBUFS, &capture->req))
	{
		if (EINVAL == errno)
		{
			fprintf(stderr, "%s does not support memory mapping\n", deviceName);
		}
		else
		{
			perror("VIDIOC_REQBUFS");
		}
		/* free capture, and returns an error code */
		icvCloseCAM_V4L(capture);
		return -1;
	}

	if (capture->req.count < buffer_number)
	{
		if (buffer_number == 1)
		{
			fprintf(stderr, "Insufficient buffer memory on %s\n", deviceName);

			/* free capture, and returns an error code */
			icvCloseCAM_V4L(capture);
			return -1;
		}
		else
		{
			buffer_number--;
			fprintf(stderr, "Insufficient buffer memory on %s -- decreaseing buffers\n", deviceName);
			goto try_again;
		}
	}

	for (n_buffers = 0; n_buffers < capture->req.count; ++n_buffers)
	{
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;

		if (-1 == xioctl(capture->deviceHandle, VIDIOC_QUERYBUF, &buf))
		{
			perror("VIDIOC_QUERYBUF");

			/* free capture, and returns an error code */
			icvCloseCAM_V4L(capture);
			return -1;
		}

		capture->buffers[n_buffers].length = buf.length;
		capture->buffers[n_buffers].start = v4l2_mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, capture->deviceHandle, buf.m.offset);

		if (MAP_FAILED == capture->buffers[n_buffers].start)
		{
			perror("mmap");

			/* free capture, and returns an error code */
			icvCloseCAM_V4L(capture);
			return -1;
		}

#ifdef USE_TEMP_BUFFER
		if (n_buffers == 0)
		{
			capture->buffers[MAX_V4L_BUFFERS].start = malloc(buf.length);
			capture->buffers[MAX_V4L_BUFFERS].length = buf.length;
		};
#endif
	}

	/* Set up Image data */
	cvInitImageHeader(&capture->frame, cvSize(capture->captureWindow.width, capture->captureWindow.height), IPL_DEPTH_8U, 3, IPL_ORIGIN_TL, 4);
	/* Allocate space for RGBA data */
	capture->frame.imageData = (char *) cvAlloc(capture->frame.imageSize);

	return 1;
}

/**
@fn int Camera::_capture_V4L(CvCaptureCAM_V4L *capture, char *deviceName)
@brief Init camera with V4L method.

  - Version V1.0

  - Description:
      -# Confirm devicename is V4L type.
      -# Confirm VID_TYPE_CAPTURE supported.
      -# set channel.
      -# Get specifics of capture window.
      -# Get image property.
      -# Get picture property and set picture property with RGB24 24bits.Confirm it.
      -# Get memory map buffer.Allocate memory for memoryMap.
      -# Set frame resolution and allocate memory.

  - Parameter List:
      -# capture  #Camera property collection.
      -# deviename #The device name in system(eg./dev/video1)

  - Return Value:
      -# -1       # If driver is not V4L2 type.
      -# -1       # V4L: device %s: Unable to open for READ ONLY.
      -# -1       # V4L: device %s: Unable to query number of channels.
      -# -1       # V4L: device %s is unable to capture video memory
      -# -1       # V4L: Could not obtain specifics of capture window.
      -# -1       # V4L: Mapping Memmory from video source error
      -# -1       # V4L: Unable to determine size of incoming image
      -# -1       # libv4l unable to ioctl VIDIOCSPICT
      -# -1       # libv4l unable convert to requested pixfmt
      -# -1       # V4L: Could not memory map video frames
      -#  1       # If OK.

  - Dependencies:
      -# V4L.

  - Notice:
      -# .


  - Modify Record:
      -# .
*/
int Camera::_capture_V4L(CvCaptureCAM_V4L *capture, char *deviceName)
{
	int detect_v4l = 0;

	detect_v4l = try_init_v4l(capture, deviceName);

	if ((detect_v4l == -1))
	{
		fprintf(stderr, "HIGHGUI ERROR: V4L: device %s: Unable to open for READ ONLY\n", deviceName);
		return -1;
	}

	if ((detect_v4l <= 0))
	{
		fprintf(stderr, "HIGHGUI ERROR: V4L: device %s: Unable to query number of channels\n", deviceName);
		return -1;
	}

	{
		if ((capture->capability.type & VID_TYPE_CAPTURE) == 0)
		{
			/* Nope. */
			fprintf(stderr, "HIGHGUI ERROR: V4L: device %s is unable to capture video memory.\n", deviceName);
			icvCloseCAM_V4L(capture);
			return -1;
		}
	}

	/* The following code sets the CHANNEL_NUMBER of the video input.  Some video sources
	 have sub "Channel Numbers".  For a typical V4L TV capture card, this is usually 1.
	 I myself am using a simple NTSC video input capture card that uses the value of 1.
	 If you are not in North America or have a different video standard, you WILL have to change
	 the following settings and recompile/reinstall.  This set of settings is based on
	 the most commonly encountered input video source types (like my bttv card) */

	{
		if (capture->capability.channels > 0)
		{
			struct video_channel selectedChannel;

			selectedChannel.channel = CHANNEL_NUMBER;
			if (v4l1_ioctl(capture->deviceHandle, VIDIOCGCHAN, &selectedChannel) != -1)
			{
				/* set the video mode to ( VIDEO_MODE_PAL, VIDEO_MODE_NTSC, VIDEO_MODE_SECAM) */
				// selectedChannel.norm = VIDEO_MODE_NTSC;
				if (v4l1_ioctl(capture->deviceHandle, VIDIOCSCHAN, &selectedChannel) == -1)
				{
					/* Could not set selected channel - Oh well */
					//printf("\n%d, %s not NTSC capable.\n",selectedChannel.channel, selectedChannel.name);
				}
			}
		}
	}

	{
		if (v4l1_ioctl(capture->deviceHandle, VIDIOCGWIN, &capture->captureWindow) == -1)
		{
			fprintf(stderr, "HIGHGUI ERROR: V4L: "
					"Could not obtain specifics of capture window.\n\n");
			icvCloseCAM_V4L(capture);
			return -1;
		}
	}

	{
		if (v4l1_ioctl(capture->deviceHandle, VIDIOCGPICT, &capture->imageProperties) < 0)
		{
			fprintf( stderr, "HIGHGUI ERROR: V4L: Unable to determine size of incoming image\n");
			icvCloseCAM_V4L(capture);
			return -1;
		}

		capture->imageProperties.palette = VIDEO_PALETTE_RGB24;
		capture->imageProperties.depth = 24;
		if (v4l1_ioctl(capture->deviceHandle, VIDIOCSPICT, &capture->imageProperties) < 0)
		{
			fprintf(stderr, "HIGHGUI ERROR: libv4l unable to ioctl VIDIOCSPICT\n\n");
			icvCloseCAM_V4L(capture);
			return -1;
		}
		if (v4l1_ioctl(capture->deviceHandle, VIDIOCGPICT, &capture->imageProperties) < 0)
		{
			fprintf(stderr, "HIGHGUI ERROR: libv4l unable to ioctl VIDIOCGPICT\n\n");
			icvCloseCAM_V4L(capture);
			return -1;
		}
		if (capture->imageProperties.palette != VIDEO_PALETTE_RGB24)
		{
			fprintf(stderr, "HIGHGUI ERROR: libv4l unable convert to requested pixfmt\n\n");
			icvCloseCAM_V4L(capture);
			return -1;
		}
	}

	{
		v4l1_ioctl(capture->deviceHandle, VIDIOCGMBUF, &capture->memoryBuffer);
		capture->memoryMap = (char *) v4l1_mmap(0, capture->memoryBuffer.size, PROT_READ | PROT_WRITE, MAP_SHARED, capture->deviceHandle, 0);

		if (capture->memoryMap == MAP_FAILED)
		{
			fprintf(stderr, "HIGHGUI ERROR: V4L: Mapping Memmory from video source error: %s\n", strerror(errno));
			icvCloseCAM_V4L(capture);
		}

		/* Set up video_mmap structure pointing to this memory mapped area so each image may be
		 retrieved from an index value */
		capture->mmaps = (struct video_mmap *) (malloc(capture->memoryBuffer.frames * sizeof(struct video_mmap)));
		if (!capture->mmaps)
		{
			fprintf(stderr, "HIGHGUI ERROR: V4L: Could not memory map video frames.\n");
			icvCloseCAM_V4L(capture);
			return -1;
		}
	}

	/* Set up Image data */
	cvInitImageHeader(&capture->frame, cvSize(capture->captureWindow.width, capture->captureWindow.height), IPL_DEPTH_8U, 3, IPL_ORIGIN_TL, 4);
	/* Allocate space for RGBA data */
	capture->frame.imageData = (char *) cvAlloc(capture->frame.imageSize);

	return 1;
}

/**
@fn CvCaptureCAM_V4L * Camera::icvCaptureFromCAM_V4L(int index)
@brief Open a camera with index num.

  - Version V1.0

  - Description:
      -# If program hadn't search cameras,search it.If failed,return NULL.
      -# If index is out of range,print error message and return NULL.
      -# If allocate memory for tempority capture failed, return NULL.
      -# capture->FirstCapture = 1.
      -# If dev is V4L2 device.capture->is_v4l2_device = 1;.

  - Parameter List:
      -# capture      # Camera property collection.
      -# property_id  # Item of camera which define in OpenCV.
      -# value        # Value of width or height

  - Return Value:
      -# CvCaptureCAM_V4L *   # If OK.
      -# NULL                 # If search camera failed or no cameras in system.
      -# NULL                 # If index is out of range.
      -# NULL                 # If allocate memory for CvCaptureCAM_V4L object.
      -# NULL                 # If camera either V4L2 nor V4L.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .
      -# .


  - Modify Record:
      -# .
*/
CvCaptureCAM_V4L * Camera::icvCaptureFromCAM_V4L(int index)
{
	static int autoindex;
	autoindex = 0;

	char deviceName[MAX_DEVICE_DRIVER_NAME];

	if (!numCameras)
		icvInitCapture_V4L(); /* Havent called icvInitCapture yet - do it now! */
	if (!numCameras)
		return NULL; /* Are there any /dev/video input sources? */

	//search index in indexList
	if ((index > -1) && !((1 << index) & indexList))
	{
		fprintf(stderr, "HIGHGUI ERROR: V4L: index %d is not correct!\n", index);
		return NULL; /* Did someone ask for not correct video source number? */
	}
	/* Allocate memory for this humongus CvCaptureCAM_V4L structure that contains ALL
	 the handles for V4L processing */
	CvCaptureCAM_V4L * capture = (CvCaptureCAM_V4L*) cvAlloc(sizeof(CvCaptureCAM_V4L));
	if (!capture)
	{
		fprintf(stderr, "HIGHGUI ERROR: V4L: Could not allocate memory for capture process.\n");
		return NULL;
	}

	/* set the default size */
	capture->width = DEFAULT_V4L_WIDTH;
	capture->height = DEFAULT_V4L_HEIGHT;

	/* Select camera, or rather, V4L video source */
	if (index < 0)
	{ // Asking for the first device available
		for (; autoindex < MAX_CAMERAS; autoindex++)
			if (indexList & (1 << autoindex))
				break;
		if (autoindex == MAX_CAMERAS)
			return NULL;
		index = autoindex;
		autoindex++; // i can recall icvOpenCAM_V4l with index=-1 for next camera
	}
	/* Print the CameraNumber at the end of the string with a width of one character */
	sprintf(deviceName, "/dev/video%1d", index);

	/* w/o memset some parts  arent initialized - AKA: Fill it with zeros so it is clean */
	memset(capture, 0, sizeof(CvCaptureCAM_V4L));
	/* Present the routines needed for V4L funtionality.  They are inserted as part of
	 the standard set of cv calls promoting transparency.  "Vector Table" insertion. */
	capture->FirstCapture = 1;

	if (_capture_V4L2(capture, deviceName) == -1)
	{
		icvCloseCAM_V4L(capture);
		capture->is_v4l2_device = 0;
		if (_capture_V4L(capture, deviceName) == -1)
		{
			icvCloseCAM_V4L(capture);
			return NULL;
		}
	}
	else
	{
		capture->is_v4l2_device = 1;
	}

	return capture;
}


#ifdef HAVE_CAMV4L2
/**
@fn int Camera::read_frame_v4l2(CvCaptureCAM_V4L* capture)
@brief Query a frame from driver of camera.And copy it to capture->buffers.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# capture  # Camera property collection.

  - Return Value:
      -# 1   #Multi define.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .
      -# .


  - Modify Record:
      -# .
*/
int Camera::read_frame_v4l2(CvCaptureCAM_V4L* capture)
{
	struct v4l2_buffer buf;

	CLEAR(buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(capture->deviceHandle, VIDIOC_DQBUF, &buf))
	{
		switch(errno
)		{
			case EAGAIN:
			return 0;

			case EIO:
			/* Could ignore EIO, see spec. */

			/* fall through */

			default:
			/* display the error and stop processing */
			perror ("VIDIOC_DQBUF");
			return 1;
		}
	}

	assert(buf.index < capture->req.count);

#ifdef USE_TEMP_BUFFER
	memcpy(capture->buffers[MAX_V4L_BUFFERS].start, capture->buffers[buf.index].start, capture->buffers[MAX_V4L_BUFFERS].length);
	capture->bufferIndex = MAX_V4L_BUFFERS;
	//printf("got data in buff %d, len=%d, flags=0x%X, seq=%d, used=%d)\n",
	//	  buf.index, buf.length, buf.flags, buf.sequence, buf.bytesused);
#else
	capture->bufferIndex = buf.index;
#endif

	if (-1 == xioctl(capture->deviceHandle, VIDIOC_QBUF, &buf))
		perror("VIDIOC_QBUF");

	return 1;
}

/**
@fn void Camera::mainloop_v4l2(CvCaptureCAM_V4L* capture)
@brief Main loop to wait driver free.

  - Version V1.0

  - Description:
      -# If driver is free,and can be capture will exit.

  - Parameter List:
      -# capture  # Camera property collection.

  - Return Value:
      -# void.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .
      -# .


  - Modify Record:
      -# .
*/
void Camera::mainloop_v4l2(CvCaptureCAM_V4L* capture)
{
	unsigned int count;

	count = 1;

	while (count-- > 0)
	{
		for (;;)
		{
			fd_set fds;
			struct timeval tv;
			int r;

			FD_ZERO(&fds);
			FD_SET(capture->deviceHandle, &fds);

			/* Timeout. */
			tv.tv_sec = 10;
			tv.tv_usec = 0;

			r = select(capture->deviceHandle + 1, &fds, NULL, NULL, &tv);

			if (-1 == r)
			{
				if (EINTR == errno)
					continue;

				perror("select");
			}

			if (0 == r)
			{
				fprintf(stderr, "select timeout\n");

				/* end the infinite loop */
				break;
			}

			if (read_frame_v4l2(capture))
				break;
		}
	}
}

/**
@fn int Camera::icvGrabFrameCAM_V4L(CvCaptureCAM_V4L* capture)
@brief Grab a frame from camera.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# capture  # Camera property collection.

  - Return Value:
      -# 0  # VIDIOC_QBUF error
      -# 0  # VIDIOC_STREAMON error
      -# 0  # Initial Capture Error
      -# 1  # If OK.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .
      -# .


  - Modify Record:
      -# .
*/
int Camera::icvGrabFrameCAM_V4L(CvCaptureCAM_V4L* capture)
{

	if (capture->FirstCapture)
	{
		/* Some general initialization must take place the first time through */

		/* This is just a technicality, but all buffers must be filled up before any
		 staggered SYNC is applied.  SO, filler up. (see V4L HowTo) */

		if (capture->is_v4l2_device == 1)
		{
			for (capture->bufferIndex = 0; capture->bufferIndex < ((int) capture->req.count); ++capture->bufferIndex)
			{
				struct v4l2_buffer buf;

				CLEAR(buf);

				buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf.memory = V4L2_MEMORY_MMAP;
				buf.index = (unsigned long) capture->bufferIndex;

				if (-1 == xioctl(capture->deviceHandle, VIDIOC_QBUF, &buf))
				{
					perror("VIDIOC_QBUF");
					return 0;
				}
			}

			/* enable the streaming */
			capture->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			if (-1 == xioctl(capture->deviceHandle, VIDIOC_STREAMON, &capture->type))
			{
				/* error enabling the stream */
				perror("VIDIOC_STREAMON");
				return 0;
			}
		}
		else
		{
			for (capture->bufferIndex = 0; capture->bufferIndex < (capture->memoryBuffer.frames - 1); ++capture->bufferIndex)
			{
				capture->mmaps[capture->bufferIndex].frame  = capture->bufferIndex;
				capture->mmaps[capture->bufferIndex].width  = capture->captureWindow.width;
				capture->mmaps[capture->bufferIndex].height = capture->captureWindow.height;
				capture->mmaps[capture->bufferIndex].format = capture->imageProperties.palette;

				if (v4l1_ioctl(capture->deviceHandle, VIDIOCMCAPTURE, &capture->mmaps[capture->bufferIndex]) == -1)
				{
					fprintf(stderr, "HIGHGUI ERROR: V4L: Initial Capture Error: Unable to load initial memory buffers.\n");
					return 0;
				}
			}

		}

		/* preparation is ok */
		capture->FirstCapture = 0;
	}

	if (capture->is_v4l2_device == 1)
	{
		mainloop_v4l2(capture);
	}
	else
	{
		capture->mmaps[capture->bufferIndex].frame  = capture->bufferIndex;
		capture->mmaps[capture->bufferIndex].width  = capture->captureWindow.width;
		capture->mmaps[capture->bufferIndex].height = capture->captureWindow.height;
		capture->mmaps[capture->bufferIndex].format = capture->imageProperties.palette;

		if (v4l1_ioctl(capture->deviceHandle, VIDIOCMCAPTURE, &capture->mmaps[capture->bufferIndex]) == -1)
		{
			/* capture is on the way, so just exit */
			return 1;
		}

		++capture->bufferIndex;
		if (capture->bufferIndex == capture->memoryBuffer.frames)
		{
			capture->bufferIndex = 0;
		}
	}

	return (1);
}

/**
@fn IplImage* Camera::icvRetrieveFrameCAM_V4L(CvCaptureCAM_V4L* capture, int)
@brief Retrieve photo data from data grabbed.

  - Version V1.0

  - Description:
      -# After grab a frame, use it retrieve photo data.Then can be display or saved.

  - Parameter List:
      -# capture  # Camera property collection.
      -# int      # ???

  - Return Value:
      -# frame  #A frame data.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# It copy data from buffers to frame.imagedata.
      -# If resolution changed,reallocate memory to frame.imagedata.


  - Modify Record:
      -# .
*/
IplImage* Camera::icvRetrieveFrameCAM_V4L(CvCaptureCAM_V4L* capture, int)
{
	if (capture->is_v4l2_device == 0)
	{
		/* [FD] this really belongs here */
		if (v4l1_ioctl(capture->deviceHandle, VIDIOCSYNC, &capture->mmaps[capture->bufferIndex].frame) == -1)
		{
			fprintf(stderr, "HIGHGUI ERROR: V4L: Could not SYNC to video stream. %s\n", strerror(errno));
		}
	}

	/* Now get what has already been captured as a IplImage return */

	/* First, reallocate imageData if the frame size changed */

	if (capture->is_v4l2_device == 1)
	{
		if (((unsigned long) capture->frame.width != capture->form.fmt.pix.width)
				|| ((unsigned long) capture->frame.height != capture->form.fmt.pix.height))
		{
			cvFree(&capture->frame.imageData);
			cvInitImageHeader(&capture->frame, cvSize(capture->form.fmt.pix.width, capture->form.fmt.pix.height), IPL_DEPTH_8U, 3, IPL_ORIGIN_TL, 4);
			capture->frame.imageData = (char *) cvAlloc(capture->frame.imageSize);
		}
	}
	else
	{
		if ((capture->frame.width != capture->mmaps[capture->bufferIndex].width)
				|| (capture->frame.height != capture->mmaps[capture->bufferIndex].height))
		{
			cvFree(&capture->frame.imageData);
			cvInitImageHeader(&capture->frame, cvSize(capture->captureWindow.width, capture->captureWindow.height), IPL_DEPTH_8U, 3, IPL_ORIGIN_TL, 4);
			capture->frame.imageData = (char *) cvAlloc(capture->frame.imageSize);
		}
	}

	if (capture->is_v4l2_device == 1)
	{
		if (capture->buffers[capture->bufferIndex].start)
		{
			memcpy((char *) capture->frame.imageData, (char *) capture->buffers[capture->bufferIndex].start, capture->frame.imageSize);
		}
	}
	else
	{
		switch (capture->imageProperties.palette)
		{
			case VIDEO_PALETTE_RGB24:
				memcpy((char *) capture->frame.imageData, (char *) (capture->memoryMap + capture->memoryBuffer.offsets[capture->bufferIndex]), capture->frame.imageSize);
				break;
			default:
				fprintf(stderr, "HIGHGUI ERROR: V4L: Cannot convert from palette %d to RGB\n", capture->imageProperties.palette);
				return 0;
		}
	}

	return (&capture->frame);
}

#endif /* HAVE_CAMV4L2 */

/**
@fn int Camera::icvSetVideoSize(CvCaptureCAM_V4L* capture, int w, int h)
@brief Set camera resolution.

  - Version V1.0

  - Description:
      -# Be called by icvSetPropertyCAM_V4L.

  - Parameter List:
      -# capture  # Camera property collection.
      -# w        # Width of resolution.
      -# h        # Height of resolution.

  - Return Value:
      -# 0       # Could not obtain specifics of capture window
       -# ???

  - Dependencies:
      -# V4L2.

  - Notice:
      -# Need to optimize.
      -# Close camera first, and open again.


  - Modify Record:
      -# .
*/
int Camera::icvSetVideoSize(CvCaptureCAM_V4L* capture, int w, int h)
{

	if (capture->is_v4l2_device == 1)
	{
		char deviceName[MAX_DEVICE_DRIVER_NAME];
		sprintf(deviceName, "%s", capture->deviceName);
		icvCloseCAM_V4L(capture);
		_capture_V4L2(capture, deviceName);

		CLEAR(capture->crop);
		capture->crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		capture->crop.c.left = 0;
		capture->crop.c.top = 0;
		capture->crop.c.height = h * 24;
		capture->crop.c.width = w * 24;

		/* set the crop area, but don't exit if the device don't support croping */
		xioctl(capture->deviceHandle, VIDIOC_S_CROP, &capture->crop);

		CLEAR(capture->form);
		capture->form.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		/* read the current setting, mainly to retreive the pixelformat information */
		xioctl(capture->deviceHandle, VIDIOC_G_FMT, &capture->form);

		/* set the values we want to change */
		capture->form.fmt.pix.width = w;
		capture->form.fmt.pix.height = h;
		capture->form.fmt.win.chromakey = 0;
		capture->form.fmt.win.field = V4L2_FIELD_ANY;
		capture->form.fmt.win.clips = 0;
		capture->form.fmt.win.clipcount = 0;
		capture->form.fmt.pix.field = V4L2_FIELD_ANY;

		/* ask the device to change the size
		 * don't test if the set of the size is ok, because some device
		 * don't allow changing the size, and we will get the real size
		 * later */
		xioctl(capture->deviceHandle, VIDIOC_S_FMT, &capture->form);

		/* try to set framerate to 30 fps */
		struct v4l2_streamparm setfps;
		memset(&setfps, 0, sizeof(struct v4l2_streamparm));
		setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		setfps.parm.capture.timeperframe.numerator = 1;
		setfps.parm.capture.timeperframe.denominator = 30;
		xioctl(capture->deviceHandle, VIDIOC_S_PARM, &setfps);

		/* we need to re-initialize some things, like buffers, because the size has
		 * changed */
		capture->FirstCapture = 1;

		/* Get window info again, to get the real value */
		if (-1 == xioctl(capture->deviceHandle, VIDIOC_G_FMT, &capture->form))
		{
			fprintf(stderr, "HIGHGUI ERROR: V4L/V4L2: Could not obtain specifics of capture window.\n\n");
			icvCloseCAM_V4L(capture);
			return 0;
		}

		return 0;
	}
	else
	{
		if (capture == 0)
			return 0;
		if (w > capture->capability.maxwidth)
			w = capture->capability.maxwidth;
		if (h > capture->capability.maxheight)
			h = capture->capability.maxheight;

		capture->captureWindow.width = w;
		capture->captureWindow.height = h;

		if (ioctl(capture->deviceHandle, VIDIOCSWIN, &capture->captureWindow)
				< 0)
		{
			icvCloseCAM_V4L(capture);
			return 0;
		}

		if (ioctl(capture->deviceHandle, VIDIOCGWIN, &capture->captureWindow)
				< 0)
		{
			icvCloseCAM_V4L(capture);
			return 0;
		}

		capture->FirstCapture = 1;
	}

	return 0;
}

/**
@fn int Camera::icvSetControl(CvCaptureCAM_V4L* capture, int property_id, double value)
@brief Set item of camera with value(rate of range,eg 0.7).

  - Version V1.0

  - Description:
      -# Set item with rate of range.Eg.07 means 0.7*(Max - Min) + Min

  - Parameter List:
      -# capture      # Camera property collection.
      -# property_id  # Item of camera which define in OpenCV.
      -# value        # Value of range of item.

  - Return Value:
      -#  0       # If OK.
      -# -1       # If failed to get min or max of one item.
      -# -1       # V4L don't support.
      -# -1       # V4L1 unable to set video infomation.

  - Dependencies:
      -# V4L2.
      -# V4L1 Need to optimize.

  - Notice:
      -# It will match ID of OpenCV type to V4L2 type,eg CV_CAP_PROP_BRIGHTNESS
           -> V4L2_CID_BRIGHTNESS.

  - Modify Record:
      -# .
*/
int Camera::icvSetControl(CvCaptureCAM_V4L* capture, int property_id, double value)
{
	struct v4l2_control c;
	__s32 ctrl_value;
	char name[32];
	int is_v4l2 = 1;
	int v4l2_min = 0;
	int v4l2_max = 255;

	if (capture->v4l2_ctrl_ranges == NULL)
	{
		v4l2_scan_controls(capture);
	}

	CLEAR(capture->control);
	CLEAR(capture->queryctrl);

	/* get current values */
	switch (property_id)
	{
		case CV_CAP_PROP_BRIGHTNESS:
			sprintf(name, "Brightness");
			capture->control.id = V4L2_CID_BRIGHTNESS;
			break;

		case CV_CAP_PROP_CONTRAST:
			sprintf(name, "Contrast");
			capture->control.id = V4L2_CID_CONTRAST;
			break;

		case CV_CAP_PROP_SATURATION:
			sprintf(name, "Saturation");
			capture->control.id = V4L2_CID_SATURATION;
			break;

		case CV_CAP_PROP_HUE:
			sprintf(name, "Hue");
			capture->control.id = V4L2_CID_HUE;
			break;

		case CV_CAP_PROP_GAIN:
			sprintf(name, "Gain");
			capture->control.id = V4L2_CID_GAIN;
			break;

		case CV_CAP_PROP_EXPOSURE:
			sprintf(name, "Exposure");
			capture->control.id = V4L2_CID_EXPOSURE;
			break;

		default:
			sprintf(name, "<unknown property string>");
			capture->control.id = property_id;
			break;
	}

	v4l2_min = v4l2_get_ctrl_min(capture, capture->control.id);
	v4l2_max = v4l2_get_ctrl_max(capture, capture->control.id);

	if ((v4l2_min == -1) && (v4l2_max == -1))
	{
		fprintf(stderr, "HIGHGUI ERROR: V4L: Property %s(%u) not supported by device\n", name, property_id);
		return -1;
	}

	if (v4l2_ioctl(capture->deviceHandle, VIDIOC_G_CTRL, &capture->control)
			== 0)
	{
		/* all went well */
	}
	else
	{
		fprintf(stderr, "HIGHGUI ERROR: V4L2: Unable to get property %s(%u) - %s\n", name, capture->control.id, strerror(errno));
	}

	if (v4l2_max != 0)
	{
		double val = value;

		if (value < 0.0)
		{
			val = 0.0;
		}
		else if (value > 1.0)
		{
			val = 1.0;
		}
		ctrl_value = val * (double) (v4l2_max - v4l2_min) + v4l2_min;
	}
	else
	{
		ctrl_value = v4l2_get_ctrl_default(capture, capture->control.id) * (double) (v4l2_max - v4l2_min) + v4l2_min;
	}

	/* try and set value as if it was a v4l2 device */
	c.id = capture->control.id;
	c.value = ctrl_value;
	if (v4l2_ioctl(capture->deviceHandle, VIDIOC_S_CTRL, &c) != 0)
	{
		/* The driver may clamp the value or return ERANGE, ignored here */
		if (errno != ERANGE)
		{
			fprintf(stderr, "HIGHGUI ERROR: V4L2: Failed to set control \"%d\": %s (value %d)\n", c.id, strerror(errno), c.value);
			is_v4l2 = 0;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	if (is_v4l2 == 0)
	{ /* use v4l1_ioctl */
		fprintf(stderr, "HIGHGUI WARNING: Setting property %u through v4l2 failed. Trying with v4l1.\n", c.id);
		int v4l_value;
		/* scale the value to the wanted integer one */
		v4l_value = (int) (0xFFFF * value);

		switch (property_id)
		{
			case CV_CAP_PROP_BRIGHTNESS:
				capture->imageProperties.brightness = v4l_value;
				break;

			case CV_CAP_PROP_CONTRAST:
				capture->imageProperties.contrast = v4l_value;
				break;

			case CV_CAP_PROP_SATURATION:
				capture->imageProperties.colour = v4l_value;
				break;

			case CV_CAP_PROP_HUE:
				capture->imageProperties.hue = v4l_value;
				break;
			case CV_CAP_PROP_GAIN:
				fprintf(stderr, "HIGHGUI ERROR: V4L: Gain control in V4L is not supported\n");
				return -1;

			case CV_CAP_PROP_EXPOSURE:
				fprintf(stderr, "HIGHGUI ERROR: V4L: Exposure control in V4L is not supported\n");
				return -1;

			default:
				fprintf(stderr, "HIGHGUI ERROR: V4L: property #%d is not supported\n", property_id);
				return -1;
		}

		if (v4l1_ioctl(capture->deviceHandle, VIDIOCSPICT, &capture->imageProperties) < 0)
		{
			fprintf(stderr, "HIGHGUI ERROR: V4L: Unable to set video informations\n");
			icvCloseCAM_V4L(capture);
			return -1;
		}
	}

	/* all was OK */
	return 0;
}

/**
@fn int Camera::icvSetPropertyCAM_V4L(CvCaptureCAM_V4L* capture, int property_id, double value)
@brief Set resolution of camera.

  - Version V1.0

  - Description:
      -# Call twice to set cmaera resolution.

  - Parameter List:
      -# capture      # Camera property collection.
      -# property_id  # Resolutin width or height.
      -# value        # Value of width or height

  - Return Value:
      -# retval       # The result of execute of icvSetVideoSize or icvSetControl.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# This function only used to set resolution.
      -# If property_id isn't width or height,call icvSetControl(capture, property_id, value).


  - Modify Record:
      -# .
*/
int Camera::icvSetPropertyCAM_V4L(CvCaptureCAM_V4L* capture, int property_id, double value)
{
	static int width = 0, height = 0;
	int retval;

	/* initialization */
	retval = 0;

	/* two subsequent calls setting WIDTH and HEIGHT will change
	 the video size */
	/* the first one will return an error, though. */

	switch (property_id)
	{
		case CV_CAP_PROP_FRAME_WIDTH:
			width = cvRound(value);
			capture->width = width;
			if (width != 0 && height != 0)
			{
				retval = icvSetVideoSize(capture, width, height);
				width = height = 0;
			}
			break;

		case CV_CAP_PROP_FRAME_HEIGHT:
			height = cvRound(value);
			capture->height = height;
			if (width != 0 && height != 0)
			{
				retval = icvSetVideoSize(capture, width, height);
				width = height = 0;
			}
			break;

		default:
			retval = icvSetControl(capture, property_id, value);
			break;
	}

	/* return the the status */
	return retval;
}

/**
@fn void Camera::icvCloseCAM_V4L(CvCaptureCAM_V4L* capture)
@brief Close already opened camera.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# capture  # Camera property collection.

  - Return Value:
      -# void

  - Dependencies:
      -# NULL.

  - Notice:
      -# If stream can't be turn off.It will print error message and exit program.

  - Modify Record:
      -# .
*/
void Camera::icvCloseCAM_V4L(CvCaptureCAM_V4L* capture)
{
	/* Deallocate space - Hopefully, no leaks */
	if (capture)
	{
		v4l2_free_ranges(capture);
		if (capture->is_v4l2_device == 0)
		{
			if (capture->mmaps)
			{
				free(capture->mmaps);
			}
			if (capture->memoryMap)
			{
				v4l1_munmap(capture->memoryMap, capture->memoryBuffer.size);
			}
			if (capture->deviceHandle != -1)
			{
				v4l1_close(capture->deviceHandle);
			}
		}
		else
		{
			capture->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			if (xioctl(capture->deviceHandle, VIDIOC_STREAMOFF, &capture->type)
					< 0)
			{
				perror("Unable to stop the stream.");
			}
			for (unsigned int n_buffers = 0; n_buffers < capture->req.count; ++n_buffers)
			{
				if (-1 == v4l2_munmap(capture->buffers[n_buffers].start, capture->buffers[n_buffers].length))
				{
					perror("munmap");
				}
			}

			if (capture->deviceHandle != -1)
			{
				v4l2_close(capture->deviceHandle);
			}
		}

		if (capture->frame.imageData)
			cvFree(&capture->frame.imageData);

		//v4l2_free_ranges(capture);
		//cvFree((void **)capture);
	}
}

/**
@fn void Camera::help_menu( void )
@brief Print help message.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# void.

  - Return Value:
      -# void

  - Dependencies:
      -# NULL.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void Camera::help_menu( void )
{
	cout << "###########################################################################" << endl;
	cout << "Control Menu:" << endl;
	cout << "Esc:		--exit" << endl;
	cout << "Space:		--capture a photograph use photo size" << endl;
	cout << "o:		--set photo resolution" << endl;
	cout << "t:		--set photo name" << endl;
	cout << endl;
	cout << "c:		--Control camera with item NO. " << endl;
	cout << "h:		--Get [Control Menu] " << endl;
	cout << "d:		--set camera inner default " << endl;
	cout << "r:		--reload camera setting " << endl;
	cout << endl;
	cout << "About the details , please read readme.txt" << endl;
	cout << "======================================================================>>>>" << endl;

}

/**
@fn cam_val* Camera::seek_link_list_end( cam_val *p )
@brief Seek the end node and return it.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# p        # Which link list will be search.

  - Return Value:
      -# NULL     # if p == NULL.
      -# end node # If OK.

  - Dependencies:
      -# NULL.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
cam_val* Camera::seek_link_list_end( cam_val *p )
{
  cam_val *last;

  if( p == NULL )  // an empty link list
  {
    return p;
  }
  else
  {
    if( p->next == NULL )  // the end element of link list
      return p;
    else
    {
      last = p;   // p is a copy here ?
      //cout << "current cnt is " << last->cnt << endl;
      while( last->next != NULL )
      {
        //cout << last->cnt << endl;
        last = last->next;
      }
      return last;
    }
  }
}

/**
@fn void Camera::clean_link_list( void )
@brief Clean link list contents.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# void.

  - Return Value:
      -# void

  - Dependencies:
      -# NULL.

  - Notice:
      -# The link list is bi-direction.So clean it is easily.

  - Modify Record:
      -# .
*/
void Camera::clean_link_list( void )
{
  link_to_end();
  while( cam_table->last != NULL )
  {
    cam_table = cam_table->last ;
    delete cam_table->next ;
    cam_table->next = NULL ;
  }
  delete cam_table ;
  cam_table = NULL ;
}

/**
@fn void Camera::get_control_list( void )
@brief Get control list.If it exists,will be updated.

  - Version V1.0

  - Description:
      -# The link list is consist of cam_val node.The details pls refer cam_val.

  - Parameter List:
      -# void.

  - Return Value:
      -# void

  - Dependencies:
      -# V4L2.

  - Notice:
      -# The base user control and extend user control.
      -# The link list is bi-direction.

  - Modify Record:
      -# .
*/
void Camera::get_control_list( void )
{
	if( (cam_table != NULL) && (cam_table->last == NULL) )  // the 1st node
	{
	  //cout << "get control non-NULL link list" << endl;
	  clean_link_list();
	}
  cam_table = new cam_val ;
  cam_table->last = NULL ;  // 1st node
  cam_table->next = NULL ;

  cam_ctrl_cnt = 1 ;

	/* User base ctrl  */
	for(captureV4L->queryctrl.id=V4L2_CID_BRIGHTNESS;
	    captureV4L->queryctrl.id < V4L2_CID_LASTP1 ; captureV4L->queryctrl.id++)
	{
    if (0 == ioctl (captureV4L->deviceHandle, VIDIOC_QUERYCTRL, &captureV4L->queryctrl))
		{
		  // TO DO support next flag
			if (captureV4L->queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
			    continue;
      // menu type have many options
      if( (captureV4L->queryctrl.type == V4L2_CTRL_TYPE_MENU) ||
           (captureV4L->queryctrl.type == V4L2_CTRL_TYPE_INTEGER) ||
           (captureV4L->queryctrl.type == V4L2_CTRL_TYPE_BOOLEAN)
           ) // save item
			{
			  captureV4L->control.id = captureV4L->queryctrl.id ;
			  if ( 0 != v4l2_ioctl(captureV4L->deviceHandle, VIDIOC_G_CTRL, &captureV4L->control))
			  {
			    break;
			  }
        cam_table->queryctrl = captureV4L->queryctrl ;
        cam_table->control = captureV4L->control ;
        cam_table->cnt = cam_ctrl_cnt ;
        cam_ctrl_cnt++ ;
        cam_table->next = new cam_val ;
        cam_table->next->last = cam_table ;
        cam_table->next->next = NULL ;
        cam_table = cam_table->next ;
			}
			else
			{
				if (errno == EINVAL)
					continue;

				perror ("VIDIOC_QUERYCTRL");
				exit (EXIT_FAILURE);
			}
    } // if ( 0 =
	}

// extend control
	captureV4L->queryctrl.id = (V4L2_CID_EXPOSURE_AUTO -1) | V4L2_CTRL_FLAG_NEXT_CTRL;
	while( 0 == ioctl (captureV4L->deviceHandle, VIDIOC_QUERYCTRL, &captureV4L->queryctrl))
	{
      if (captureV4L->queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
			    continue;

      if( (captureV4L->queryctrl.type == V4L2_CTRL_TYPE_MENU) ||
           (captureV4L->queryctrl.type == V4L2_CTRL_TYPE_INTEGER) ||
           (captureV4L->queryctrl.type == V4L2_CTRL_TYPE_BOOLEAN)
           )
			{
			  captureV4L->ext_ctrl.id = captureV4L->queryctrl.id ;
	      captureV4L->ext_ctrls.count = 1 ;
	      captureV4L->ext_ctrls.controls = &captureV4L->ext_ctrl ;
	      if( 0 != v4l2_ioctl(captureV4L->deviceHandle, VIDIOC_G_EXT_CTRLS , &captureV4L->ext_ctrls) )
	      {
	        break ;
	      }
        cam_table->queryctrl = captureV4L->queryctrl ;
        cam_table->ext_ctrl = captureV4L->ext_ctrl ;
        cam_table->cnt = cam_ctrl_cnt ;
        cam_ctrl_cnt++;
        cam_table->next = new cam_val ;
        cam_table->next->last = cam_table ;
        cam_table->next->next = NULL ;
        cam_table = cam_table->next ;
			}

			else
			{
				if (errno == EINVAL)
					continue;

				perror ("VIDIOC_QUERYCTRL");
				exit (EXIT_FAILURE);
			}
			// the last ext ctrl V4L2_CID_SCENE_MODE
			if( captureV4L->queryctrl.id == V4L2_CID_IRIS_RELATIVE )
			{
			    break;
			}
			else
			{
			    captureV4L->queryctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
			}
	}
	// close the link list and delete the last allocate memory
	cam_table = cam_table->last ;
	delete cam_table->next ;
	cam_table->next = NULL ;
	link_to_begin();
	//cout << "run to here "<<  cam_table->cnt << endl ;
}

/**
@fn void Camera::list_ctrls( cam_val *input )
@brief Print id value and name of every item of control list.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# input  # Pointer of link list point to header node.

  - Return Value:
      -# void

  - Dependencies:
      -# NULL.

  - Notice:
      -# The base user control and extend user control.

  - Modify Record:
      -# .
*/
void Camera::list_ctrls( cam_val *input )
{
  cam_val *tmp = input ;

  cout << endl;
	cout << "====================Current property===========================" << endl;
  cout << "[NO.] " << "[Val] " << "[Min]   " << "[Def]   " << "[Max]   "
        << "[Step]  " << "[Name]"<< endl;
  while( tmp != NULL )
  {
    if( (tmp->queryctrl.id >= V4L2_CID_BRIGHTNESS) && (tmp->queryctrl.id < V4L2_CID_LASTP1) )
    {
      cout << tmp->cnt << "     " << tmp->control.value <<
          " " << tmp->queryctrl.minimum <<
          " " << tmp->queryctrl.default_value <<
          " " << tmp->queryctrl.maximum <<
          " " << tmp->queryctrl.step <<
          " " << tmp->queryctrl.name << endl;
    }
    else
    {
      cout << tmp->cnt << "     " << tmp->ext_ctrl.value <<
          " " << tmp->queryctrl.minimum <<
          " " << tmp->queryctrl.default_value <<
          " " << tmp->queryctrl.maximum <<
          " " << tmp->queryctrl.step <<
          " " << tmp->queryctrl.name << endl;
    }
    if( tmp->queryctrl.type == V4L2_CTRL_TYPE_MENU )
    {
      //cout << "      Menu items:" << endl;
      cout << "      " << tmp->queryctrl.name << " items:" << endl;
      memset( &tmp->querymenu, 0, sizeof( v4l2_querymenu ) ) ;
      tmp->querymenu.id = tmp->queryctrl.id ;
      for( tmp->querymenu.index = tmp->queryctrl.minimum ;
            tmp->querymenu.index <= tmp->queryctrl.maximum ;
            tmp->querymenu.index++ )
      {
        if( 0 == ioctl( captureV4L->deviceHandle, VIDIOC_QUERYMENU, &tmp->querymenu ) )
          cout << "          " << tmp->querymenu.name << endl ;
      }
    }
    if( tmp->queryctrl.type == V4L2_CTRL_TYPE_BOOLEAN )
    {
      cout << "      " << tmp->queryctrl.name << " is ON(1) OFF(0)." << endl;
    }

    tmp = tmp->next;
  }

	cout << "Next photo is " << save_image_name[pic_cnt] << endl;
	cout << "Current captured photo resolution is " << photo_size_width << " x " << photo_size_height << endl;
	cout << "=============================================================" << endl;
	cout << endl;
}

/**
@fn void Camera::set_photograph_name( void )
@brief Set photo name through console step by step.

  - Version V1.0

  - Description:
      -# Guid user to set every element of photo name.

  - Parameter List:
      -# void.

  - Return Value:
      -# void

  - Dependencies:
      -# NULL.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void Camera::set_photograph_name( void )
{
	int i,tmp;
	cout << endl;
	cout << endl;
	cout << "======================================================================>>>>" << endl;
	cout << "select photo captured type :" << endl;
	//cout << img_name_suffix[0] << endl;
	for ( i=1; img_name_suffix[i-1][0] != '0' ; i++ ){
		cout << i << ":  " << img_name_suffix[i-1] << endl;
	}
	//cout << endl;
	cout << ">>:" ;
	cin >> tmp ;
	if ( tmp < i ){
		img_typ_index = tmp - 1 ;
	}
	cout << "Change the photograph name ?"<< endl;
	cout << "1,Not change"<< endl;
	cout << "2,input the string "<< endl;
	cout << ">>:" ;
	cin >> tmp ;
	if (tmp == 2){
		cout << "Input the photograph name prefix (not over 10 char)" << endl;
		cout << ">>:" ;
		cin >> img_name_prefix;
	}
	cout << "Change the photograph name start number ?"<< endl;
	cout << "1,Not change"<< endl;
	cout << "2,input the number"<< endl;
	cout << ">>:" ;
	cin >> tmp ;
	if (tmp == 2){
		cout << "Input the number" << endl;
		cout << ">>:" ;
		cin >> pic_num_start;
	}

	char j;
	//cout << img_name_prefix << endl;
	for (int i = pic_num_start;i < (photo_amount+pic_num_start) ; i ++)
	{
		sprintf(&j,"%d",i);

		strcpy (save_image_name[i-pic_num_start],img_name_prefix);
		strcat( save_image_name[i-pic_num_start],&j);
		strcat( save_image_name[i-pic_num_start],".");
		//itoa(i-1,&j,10));
		strcat( save_image_name[i-pic_num_start],img_name_suffix[img_typ_index]);
	}
	cout << "Change the next photograph number ?  " << endl;
	cout << "1, Not change " << endl;
	cout << "2, input your value " << endl;
	cout << ">>:" ;
	cin >> tmp ;
	if ( tmp == 2 ){
		cout << "Input the start number of next photograph captured :" << endl;
		cout << ">>:" ;
		cin >> pic_cnt ;
		pic_cnt-- ;
	}
	cout << "Photograph type is " << img_name_suffix[img_typ_index] << ". Next photograph is " << save_image_name[pic_cnt] << endl;
}

/**
@fn void Camera::gen_window_name( void )
@brief Generate window title depend Camera::index.

  - Version V1.0

  - Description:
      -# Deferent camera use different window title.

  - Parameter List:
      -# void.

  - Return Value:
      -# void

  - Dependencies:
      -# NULL.

  - Notice:
      -# .

  - Modify Record:
      -# V1.1 May28 Vince Reallocate memory for n,because stack protect.
*/
void Camera::gen_window_name( void )
{
  /// Generate the window name
	Win_Name += "USB Camera";
	char *n = new char[2];
	// itoa( index, &n, 10 ); linux std C++ don't include itoa()
	sprintf( n, "%d", index );
	Win_Name.append( " " );
	Win_Name.append( n );
	delete n ;
}

 /**
@fn void Camera::User_ctrl( char key, char *photo_name )
@brief Execute user command depend help_menu.

  - Version V1.0

  - Description:
      -# Execute user control depend help_menu.
      -# Many config item can be update through console step by step.
      -# "Esc"  # Set stop flag.
      -# "d"    # Set all camera settings to default.
      -# "r"    # Reload camera config file.Make the values effective.Update control list.
      -# "p"    # Set ext_sync flag if you enable DEBUG_EXT_SYNC in camera.h.
      -# "h"    # Print help_menu and update control list.List available control items.
      -# "c"    # Control item through the counter which user input.This item will
                  guid user input value.If the value is out of range it will not effective.
      -# "o"    # Set photo resolution from it list options.View resolution can
                  be set in config file.
      -# "t"    # Set photo name through console step by step.
      -# " "    # Capture a frame and save as a photo.If photo_name is NULL,it will
                  use system default que.Photo size is photo_size_width and photo_size_height.
      -# default  Print key value in int and char.

  - Parameter List:
      -# key   # Value of input from keyboard.Char type.

  - Return Value:
      -# void

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void Camera::User_ctrl( char key, char *photo_name )
{
	int v4l2_min , v4l2_max , v4l2_def , tmp , i ;
	time_t time_end, time_start;
  //cout << img_name_prefix << endl;
	switch( key )
	{
		case 27:  // 'Esc'
			stop = 1;
		break;
		case 'd':
		  set_cam_default();
		break;
		case 'r':
      // load_config_parameter(); need regenerate photo name set view size ,etc.
		  //if( cam_table == NULL )
		  //   cout << "cam_table is empty ..." << endl;
		  load_cam_file( cam_config_file.c_str() );
		  refresh_cam_link();
		  get_control_list();
		break;
#if DEBUG_EXT_SYNC
    case 'p':
      cout << "start take photo with robot ..." << endl;
      ext_sync = 1 ;
    break;
#endif
		case 'h':
			help_menu();
			get_control_list();
			list_ctrls( cam_table );
		break;
		case 'c':
		  cout << "Select option No.:" ;
		  cin >> tmp ;
		  cout << endl;
		  set_cam_ctrl( tmp );
		break;
		case 'o':
			cout << endl;
			cout << endl;
			cout << "======================================================================>>>>" << endl;
			cout << "Select photograph's resolution width & height:" << endl;
			for ( i=1; resol[i-1][0] > 0 ; i++ ){
				cout << i << ":  " << resol[i-1][0] << " x " << resol[i-1][1] << endl;
			}
			cout << ">>:" ;
			cin >> tmp ;
			if ( tmp < i ){
				photo_size_width = resol[tmp-1][0] ;
				photo_size_height = resol[tmp-1][1] ;
				resol_index = tmp - 1 ;
			}
			icvSetPropertyCAM_V4L(captureV4L, CV_CAP_PROP_FRAME_WIDTH, photo_size_width);
			icvSetPropertyCAM_V4L(captureV4L, CV_CAP_PROP_FRAME_HEIGHT, photo_size_height);
			cout << "Current resolution is " << resol[resol_index][0] << " x " << resol[resol_index][1] << endl;
		break;
		case 't':
			set_photograph_name();
		break;
		case ' '://32:  // 'Space' to capture
			cout << "Save ..." ;
			icvGrabFrameCAM_V4L(captureV4L);
      Image_photo = icvRetrieveFrameCAM_V4L(captureV4L, 0);
			if( photo_name == NULL )
			{
        if ( 1 == cvSaveImage( save_image_name[pic_cnt], Image_photo)){
          cout << " is OK. Photo name is " << save_image_name[pic_cnt] << endl;
          pic_cnt++;
        }
        else
        {
          cout << " is Failed " << endl;
          //break;  // else return ???
        }
			}
			else
			{
        if ( 1 == cvSaveImage( photo_name, Image_photo)){
          cout << " is OK. Photo name is " << photo_name << endl;
          pic_cnt++;
        }
        else
        {
          cout << " is Failed " << endl;
          //break;  // else return ???
        }
			}
		break;
		default:
			if ( key != -1)
			{
				cout << " The key char is: "  << key << endl;
				cout << " The key data is: "  << (int)key << endl;
			}
		break;
	}

}

/**
@fn int Camera::set_cam_ctrl( const int num )
@brief Set item with value which user input through command line API.

  - Version V1.0

  - Description:
      -# Besides the num to find corresponding control item, and find ID and value.

  - Parameter List:
      -# num   # The counter of link list used to find ID and value.

  - Return Value:
      -# 0  If OK.
      -# -1 num is out of range.
      -# -2 value which user input is out of range.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
int Camera::set_cam_ctrl( const int num )
{
  if ( (num < 1 ) || ( num > cam_ctrl_cnt ) )
  {
    cout << "Invalid arguments, pls retry ..." << endl;
    return -1 ;
  }

  cam_val *tmp = cam_table;

  // find control item
  while( tmp->cnt != num )
  {
    tmp = tmp->next;
  }

  // print BOOLEAN and MENU control detail
  if( tmp->queryctrl.type == V4L2_CTRL_TYPE_MENU )
  {
    cout << "      " << tmp->queryctrl.name << " items:" << endl;
    memset( &tmp->querymenu, 0, sizeof( v4l2_querymenu ) ) ;
    tmp->querymenu.id = tmp->queryctrl.id ;
    for( tmp->querymenu.index = tmp->queryctrl.minimum ;
          tmp->querymenu.index <= tmp->queryctrl.maximum ;
          tmp->querymenu.index++ )
    {
      if( 0 == ioctl( captureV4L->deviceHandle, VIDIOC_QUERYMENU, &tmp->querymenu ) )
        cout << "          " << tmp->querymenu.name << endl ;
    }
  }
  if( tmp->queryctrl.type == V4L2_CTRL_TYPE_BOOLEAN )
  {
    cout << tmp->queryctrl.name << " is ON(1) OFF(0)." << endl;
  }

  // control camera item with the value which user input
  if( (tmp->queryctrl.id >= V4L2_CID_BRIGHTNESS) &&
       (tmp->queryctrl.id < V4L2_CID_LASTP1) )
  {
    // User base control
    cout << "Input value:" ;
    cin >> captureV4L->control.value ;
    cout << endl ;

    if( (captureV4L->control.value < tmp->queryctrl.minimum)
         || (captureV4L->control.value > tmp->queryctrl.maximum) )
    {
      return -2 ;
    }
    captureV4L->control.id =tmp->queryctrl.id ;
    if ( 0 != v4l2_ioctl(captureV4L->deviceHandle, VIDIOC_S_CTRL, &captureV4L->control))
      cout << "Control " << tmp->queryctrl.name << " Failed."<< endl ;
  }
  else
  {
    // extend control
    cout << "Input value:" ;
    int ctrl_val_tmp = 0 ;
    cin >> ctrl_val_tmp ;
    captureV4L->ext_ctrl.value = ctrl_val_tmp ;
    cout << endl ;
    if( (captureV4L->ext_ctrl.value < tmp->queryctrl.minimum)
         || (captureV4L->ext_ctrl.value > tmp->queryctrl.maximum) )
    {
      return -2 ;
    }
    captureV4L->ext_ctrl.id = tmp->queryctrl.id ;
    captureV4L->ext_ctrls.count = 1 ;
    captureV4L->ext_ctrls.controls = &captureV4L->ext_ctrl;
    if( 0 != v4l2_ioctl(captureV4L->deviceHandle, VIDIOC_S_EXT_CTRLS , &captureV4L->ext_ctrls) )
      cout << "Control " << tmp->queryctrl.name << " Failed."<< endl ;
  }

  return 0 ;
}

/**
@fn int Camera::set_cam_ctrl( const int item, const int value )
@brief Set item with value in camera.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# item   # Control ID,eg. V4L2_CID_BRIGHTNESS or BRIGHTNESS.
      -# value. # The value will set to item option.

  - Return Value:
      -# 0    # If OK.
      -# -1   # item is out of range.
      -# -2   # value is out of range.
      -# item # If Mode is auto,set absolote value.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# If mode is auto,set absolote value will failed and return item.
      -# So set mode first.

  - Modify Record:
      -# .
*/
int Camera::set_cam_ctrl( const int item, const int value )
{
  // control item with simply name
  cam_val *tmp = cam_table ;

  // search the control item in link list
  for( int i = 0 ; i < cam_ctrl_cnt ; i++ )
  {
    if( tmp->queryctrl.id == item )
      break ;
    tmp = tmp->next ;
  }
  if( (tmp == NULL) ) //|| (tmp->queryctrl.id != item) )
  {
    return -1 ;
  }

  // control camera item with the value which user input
  if( (tmp->queryctrl.id >= V4L2_CID_BRIGHTNESS) &&
       (tmp->queryctrl.id < V4L2_CID_LASTP1) )
  {
    // User base control
    if( (value < tmp->queryctrl.minimum) || (value > tmp->queryctrl.maximum) )
    {
      return -2 ;
    }
    captureV4L->control.value = value ;
    captureV4L->control.id = tmp->queryctrl.id ;
    if ( 0 != v4l2_ioctl(captureV4L->deviceHandle, VIDIOC_S_CTRL, &captureV4L->control))
    {
      cout << "Control " << tmp->queryctrl.name << " Failed."<< endl ;
      return item ;
    }
  }
  else
  {
    // extend control
    if( (value < tmp->queryctrl.minimum) || (value > tmp->queryctrl.maximum) )
    {
      return -2 ;
    }
    captureV4L->ext_ctrl.value = value ;
    captureV4L->ext_ctrl.id = tmp->queryctrl.id ;
    captureV4L->ext_ctrls.count = 1 ;
    captureV4L->ext_ctrls.controls = &captureV4L->ext_ctrl;
    if( 0 != v4l2_ioctl(captureV4L->deviceHandle, VIDIOC_S_EXT_CTRLS , &captureV4L->ext_ctrls) )
    {
      cout << "Control " << tmp->queryctrl.name << " Failed."<< endl ;
      return item ;
    }
  }

  return 0 ;
}

/**
@fn int Camera::set_cam_default( void )
@brief Set all the camera setting to default.

  - Version V1.0

  - Description:
      -# From queryctrl of link list get default values and set them to camera.

  - Parameter List:
      -# void.

  - Return Value:
      -# 0  # If OK.
      -# -1 # If link list pointer is NULL.

  - Dependencies:
      -# V4L2.

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
int Camera::set_cam_default( void )
{
  if( cam_table == NULL )
  {
    // cout << "Invalid arguments, pls retry ..." << endl;
    return -1 ;
  }

  cam_val *tmp = cam_table ;
  while( tmp != NULL )
  {
    if( (tmp->queryctrl.id >= V4L2_CID_BRIGHTNESS) &&
         (tmp->queryctrl.id < V4L2_CID_LASTP1) )
    {
      // User base control
      captureV4L->control.value = tmp->queryctrl.default_value ;
      captureV4L->control.id = tmp->queryctrl.id ;
      if ( 0 != v4l2_ioctl(captureV4L->deviceHandle, VIDIOC_S_CTRL, &captureV4L->control))
      {
        cout << "Control " << tmp->queryctrl.name << " Failed."<< endl ;
      }
    }
    else
    {
      // extend control
      captureV4L->ext_ctrl.value = tmp->queryctrl.default_value ;
      captureV4L->ext_ctrl.id = tmp->queryctrl.id ;
      captureV4L->ext_ctrls.count = 1 ;
      captureV4L->ext_ctrls.controls = &captureV4L->ext_ctrl ;
      if( 0 != v4l2_ioctl(captureV4L->deviceHandle, VIDIOC_S_EXT_CTRLS , &captureV4L->ext_ctrls) )
      {
        cout << "Control " << tmp->queryctrl.name << " Failed."<< endl ;
      }
    }
    tmp = tmp->next ;
  }

  return 0 ;
}

/**
@fn int Camera::write_cam_file( const char *filename, struct v4l2_capability *cap )
@brief Write the control list to camera config file.

  - Version V1.0

  - Description:
      -# Write the id, value and name of link list to camera config file.
      -# At last,move link list pointer to header.

  - Parameter List:
      -# filename   # Config file name.
      -# cap        # v4l2 structor contains the details of camera,for example name,bus info.

  - Return Value:
      -# 0  If OK.
      -# -1 If filename is NULL or open failed.
      -# -2 If link list pointer is NULL.

  - Dependencies:
      -# .

  - Notice:
      -# .

  - Modify Record:
      -# V1.1 May25,2013 Vince Add stardard file description to camera config file.
*/
int Camera::write_cam_file( const char *filename, struct v4l2_capability *cap )
{
  if( filename == NULL )
    return -1 ;
  if( cam_table == NULL )
    return -2 ;

// test wether the file exists
  FILE *fd = NULL ;
  fd = fopen( filename, "a+" );
  fclose( fd );

  ofstream writefile;
  writefile.open( filename );

  writefile << "#" << " " << cap->card << " " << "config file." << endl;
  writefile << "#" << " " << "Location " << cap->bus_info << endl;
  writefile << "#" << " " << "Tips: Set mode first,then set absolete value." << endl;
  writefile << "#" << " " << "Tips: If absolute setting before mode setting,reload twice,it will works." << endl;
  writefile << "#" << " " << "Tips: Don't write empty line to this file." << endl;
  // TO DO Add time ???

  while( cam_table->next != NULL )  // protect the link pointer not as NULL
  {
    if( (cam_table->queryctrl.id >= V4L2_CID_BRIGHTNESS) &&
       (cam_table->queryctrl.id < V4L2_CID_LASTP1) )
      writefile << cam_table->queryctrl.id << " " << cam_table->control.value  << " "
                  << cam_table->queryctrl.name << endl;
    else
      writefile << cam_table->queryctrl.id << " " << cam_table->ext_ctrl.value << " "
                  << cam_table->queryctrl.name << endl;
    cam_table = cam_table->next ;
  }
  if( (cam_table->queryctrl.id >= V4L2_CID_BRIGHTNESS) &&
       (cam_table->queryctrl.id < V4L2_CID_LASTP1) )
    writefile << cam_table->queryctrl.id << " " << cam_table->control.value << " "
                 << cam_table->queryctrl.name << endl;
  else
    writefile << cam_table->queryctrl.id << " " << cam_table->ext_ctrl.value << " "
                 << cam_table->queryctrl.name << endl;

  link_to_begin();
  writefile.close();
  return 0 ;
}

/**
@fn int Camera::write_config_file( void )
@brief Generate config file.

  - Version V1.0

  - Description:
      -# Write the config file if it doesn't exists.

  - Parameter List:
      -# void

  - Return Value:
      -# 0  If OK.

  - Dependencies:
      -# .

  - Notice:
      -# .

  - Modify Record:
      -# V1.0 May25,2013 Vince Create.
*/
int Camera::write_config_file( void )
{
  FILE *fd = NULL ;
  fd = fopen( CONFIG_FILE, "a+" );  // if it doesn't exists, create.
  fclose( fd );

  ofstream writefile;
  writefile.open( CONFIG_FILE );

  writefile << "# Camera class configuration file"<< endl;
  writefile << "# Rev 1.0 2013-5-15 Vince create."<< endl;
  writefile << "# Rev 1.1 2013-5-15 Vince Add some config setting for SMI camera."<< endl;
  writefile << endl;
  writefile << "## camera setting detail file"<< endl;
  writefile << "#camera_cfg_file = SMI.cfg"<< endl;
  writefile << endl;
  writefile << "# Camera will be used"<< endl;
  writefile << "## cam_cnt = 2"<< endl;
  writefile << endl;
  writefile << "# index which camera on current system"<< endl;
  writefile << "index = 0"<< endl;
  writefile << "##index1 = 0"<< endl;
  writefile << "##index2 = 1"<< endl;
  writefile << endl;
  writefile << "# index which resolution"<< endl;
  writefile << "resol_index = 1"<< endl;
  writefile << "resol_cnt = 12"<< endl;
  writefile << endl;
  writefile << "# resolution group, end with 0, must be 12 values"<< endl;
  writefile << "resolution_w = 2592 2048 1920 1600 1280 1280 1024 800 640 320 0 0"<< endl;
  writefile << "resolution_h = 1944 1536 1080 1200 960  720  768  600 480 240 0 0"<< endl;
  writefile << endl;
  writefile << "# the photo name num start"<< endl;
  writefile << "pic_num_start = 1"<< endl;
  writefile << endl;
  writefile << "# the name array index"<< endl;
  writefile << "pic_cnt = 0"<< endl;
  writefile << endl;
  writefile << "# the quantily of photo name"<< endl;
  writefile << "photo_amount = 1000"<< endl;
  writefile << "photo_name_length = 14"<< endl;
  writefile << endl;
  writefile << "# the prifex of photo name"<< endl;
  writefile << "image_name_prefix = image_"<< endl;
  writefile << endl;
  writefile << "# the name type index 1 mean bmp"<< endl;
  writefile << "img_typ_index = 1"<< endl;
  writefile << endl;
  writefile << "# type of photo"<< endl;
  writefile << "photo_type = png bmp jpg raw 0"<< endl;
  writefile << endl;
  writefile << "# setting view size and captured photo size"<< endl;
  writefile << "view_size_width = 800"<< endl;
  writefile << "view_size_height = 600"<< endl;
  writefile << "photo_size_width = 2048"<< endl;
  writefile << "photo_size_height = 1536"<< endl;
  writefile << endl;

  writefile.close();
  return 0 ;
}

/**
@fn int Camera::load_cam_file( void )
@brief Load camera config file.If it isn't exists,create it.

  - Version V1.0

  - Description:
      -# Check wether camera config file exist.If it exists,update the link list from it.
      -# If not,create it with the device name of camera.

  - Parameter List:
      -# void.

  - Return Value:
      -# 0  If OK.
      -# -1 If can't query capabilities.
      -# -2 Camera config file exists,but can't load successary.
      -# -3 Can't refresh the camera control list.
      -# -4 Failed to write the camera config file.

  - Dependencies:
      -# .

  - Notice:
      -# .

  - Modify Record:
      -# V1.0 May25,2013 Vince Create.
*/
int Camera::load_cam_file( void )
{
  struct v4l2_capability cap;

  // Get the detail info of camera.
  if( 0 != ioctl( captureV4L->deviceHandle, VIDIOC_QUERYCAP, &cap) )
    return -1 ;

  cam_config_file.empty();
  cam_config_file += (char *)cap.card ;
  cam_config_file += ".cfg" ;

  FILE *fd = fopen( cam_config_file.c_str(), "r" ); // read only open to check
  if( fd != NULL )
  {
    //cout << "load file..." << endl;
    //cout << cam_file.c_str() << endl;
    if( 0 != load_cam_file(cam_config_file.c_str()) )
      return -2 ;
    if( 0 != refresh_cam_link() )
      return -3 ;
  }
  else
  {
    //cout << "create file ..." << endl;
    if( 0 != write_cam_file(cam_config_file.c_str(),&cap) )
      return -4 ;
  }
  return 0 ;
}

/**
@fn int Camera::load_cam_file( const char *filename )
@brief Update the link list from camera config file.

  - Version V1.0

  - Description:
      -# Read camera config file line by line, and retrieve id and value to link list.
      -# At last,move link list pointer to header.

  - Parameter List:
      -# filename   # Config file name.

  - Return Value:
      -# 0  If OK.
      -# -1 If filename is NULL or open failed.

  - Dependencies:
      -# .

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
int Camera::load_cam_file( const char *filename )
{
  string line, item ;

  if( filename == NULL )
    return -1 ;
  ifstream readfile( filename );

  //readfile.open( filename );
  if( !readfile )
    return -1 ;
  else
  {
    //while( readfile.good() )
    while( getline(readfile,line) )
    {
      istringstream linestream( line );
      getline( linestream, item, ' ' );   //get 2 item every line
      if( item[0] == '#' )
        continue ;
      else
      {
        cam_table->queryctrl.id =(unsigned int) atoi( item.c_str() );
        //cout << cam_table->queryctrl.id << " " << flush ;
      }
      getline( linestream, item, ' ' );
      if( item[0] == '#' )
        continue ;
      else
      {
        cam_table->user_value = atoi( item.c_str() );
        //cout << cam_table->user_value << endl ;
      }
      //str >> cam_table->queryctrl.id >> cam_table->user_value ; // >> cam_table->queryctrl.name ;
      //cout << cam_table->queryctrl.id << " " << cam_table->user_value << " " << cam_table->queryctrl.name << endl;
      if( cam_table->next == NULL )
      {
        cam_table->next = new cam_val ;
        cam_table->next->last = cam_table ;
        cam_table->next->next = NULL ;
        //cout << "empty link list " << endl;
        //return -2 ;
      }
      cam_table = cam_table->next ;
      //cout << str << endl;
    }
  }
  cam_table = cam_table->last ;
  delete cam_table->next ;
  cam_table->next = NULL ;
  link_to_begin();
  readfile.close();
  return 0 ;
}

/**
@fn int Camera::link_to_begin( void )
@brief Put the link list pointer to the link list start node.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# NULL.

  - Return Value:
      -# 0  If OK.
      -# -1 If camera link list header is empty.

  - Dependencies:
      -# .

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
int Camera::link_to_begin( void )
{
  if( !cam_table )
  {
    return -1 ;
  }
  while( cam_table->last != NULL )
  {
    cam_table = cam_table->last ;
  }
  return 0 ;
}

/**
@fn int Camera::link_to_end( void )
@brief Put the link list pointer to the link list end node.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# NULL.

  - Return Value:
      -# 0  If OK.
      -# -1 If camera link list header is empty.

  - Dependencies:
      -# .

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
int Camera::link_to_end( void )
{
  if( !cam_table )
  {
    return -1 ;
  }
  while( cam_table->next != NULL )
  {
    cam_table = cam_table->next ;
  }
  return 0 ;
}

/**
@fn int Camera::refresh_cam_link( void )
@brief Refresh control list with values from camera config file.

  - Version V1.0

  - Description:
      -# Push link list contents into camera.

  - Parameter List:
      -# NULL.

  - Return Value:
      -# 0  If OK.
      -# -1 If camera link list header is empty.

  - Dependencies:
      -# .

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
int Camera::refresh_cam_link( void )
{
  if( cam_table == NULL )
  {
    //cout << "refresh failed .."<< endl;
    return -1 ;
  }
  while( cam_table->next != NULL )
  {
    set_cam_ctrl( cam_table->queryctrl.id, cam_table->user_value );
    cam_table = cam_table->next ;
  }
  set_cam_ctrl( cam_table->queryctrl.id, cam_table->user_value );
  link_to_begin();
  return 0 ;
}

/**
@fn Camera::Camera( void )
@brief Camera constructor function.

  - Version V1.2

  - Description:
      -# Constructor.
      -# Initialize the data member.
      -# Load config parameter from CONFIG_FILE.
      -# Generate photo name array.
      -# Search and open desire camera in current system.
      -# Set resolution.
      -# Get control list with link list method.
      -# Load camera config file.If it isn't exist,write it with link list contents.
      -# Refresh control list.
      -# Print control list and help message.

  - Parameter List:
      -# NULL.

  - Return Value:
      -# void.

  - Dependencies:
      -# OpenCV

  - Notice:
      -# If you want to use a new camera,you can set camera config-file name
         in CONFIG_FILE.Such as xxx.cfg.Because it isn't exist, will be created
         and written in.
      -# You can modify values of camera config file,use 'r' command to update
          if you use User_ctrl function.

  - Modify Record:
      -# V1.1 May25,2013 Vince Add cout the details of concurrent camera.
      -# V1.2 May27,2013 Vince Add check the config file wether exists.If not, create it.
*/
Camera::Camera( void )
{
  if( NULL == fopen( CONFIG_FILE, "r" ))
    write_config_file();
  load_config_parameter();
  cam_ctrl_cnt = 1;
  key = 0;
  stop = 0;
  cam_table = NULL;

#if DEBUG_EXT_SYNC
  ext_sync = 0 ;
#endif

  n_buffers = 0;
  numCameras = 0;
  indexList = 0;

  char *j = new char[5];
	for (int i = pic_num_start;i < ( photo_amount + pic_num_start) ; i++)
	{
	  sprintf(j,"%d",i);
		strcpy( save_image_name[i-pic_num_start],img_name_prefix);
		strcat( save_image_name[i-pic_num_start],j);
		strcat( save_image_name[i-pic_num_start],".");
		strcat( save_image_name[i-pic_num_start],img_name_suffix[img_typ_index]);
	}
	delete j;
	j = NULL ;

#if Display_output
	cout << "Photograph name is from " << save_image_name[0] << " to " << save_image_name[photo_amount-1]  << endl;
	cout << endl;

	cout << "Searching for cameras... " << flush;
#endif

	icvInitCapture_V4L();

#if Display_output
	cout << numCameras << " camera(s) found:" << endl;

	for (int i=0, j=1; i<MAX_CAMERAS; i++)
	{
		if (indexList & 0x1)
		{
			cout << j << "  " << "/dev/video" << i << "  " << endl;
			j++;
		}
		indexList >>= 1;
	}
	numCameras = 0;
#endif

	/* Create the image and the window */
	Image = cvCreateImage(cvSize(photo_size_width, photo_size_height), IPL_DEPTH_8U, 3);
	Image->imageData = (char *) cvAlloc(Image->imageSize);

	view_window = cvCreateImage(cvSize(view_size_width, view_size_height), IPL_DEPTH_8U, 3);
	view_window->imageData = (char *) cvAlloc(view_window->imageSize);

#if Display_output
	gen_window_name();
	cvNamedWindow( Win_Name.c_str() );
#endif
	/* Initialize the webcam */
	//cout << " Open Device ..."<< endl;
	captureV4L = icvCaptureFromCAM_V4L( index );

	/* Set up the width and the height of the image */
	//cout << " Setting the resolution ... " << endl;
	icvSetPropertyCAM_V4L(captureV4L, CV_CAP_PROP_FRAME_WIDTH, photo_size_width); //view_size_width);
	icvSetPropertyCAM_V4L(captureV4L, CV_CAP_PROP_FRAME_HEIGHT, photo_size_height); //view_size_height);

	get_control_list();
#if 0
  cout << captureV4L->cap.driver << endl;
  cout << captureV4L->cap.card << endl;
  cout << captureV4L->cap.bus_info << endl;
  cout << captureV4L->cap.version << endl;


#endif
  if( 0 != load_cam_file() )
    exit(0);

  get_control_list();
#if Display_output
	list_ctrls( cam_table );
	help_menu();
#endif
}

/**
@fn Camera::Camera( const unsigned int cam_index )
@brief Camera constructor function with cam_index.

  - Version V1.2

  - Description:
      -# Constructor.
      -# Initialize the data member.
      -# Load config parameter from CONFIG_FILE.
      -# Generate photo name array.
      -# Search and open desire camera in current system.
      -# Set resolution.
      -# Get control list with link list method.
      -# Load camera config file.If it isn't exist,write it with link list contents.
      -# Refresh control list.
      -# Print control list and help message.
      -# index will be update with cam_index.

  - Parameter List:
      -# NULL.

  - Return Value:
      -# void.

  - Dependencies:
      -# OpenCV

  - Notice:
      -# If you want to use a new camera,you can set camera config-file name
         in CONFIG_FILE.Such as xxx.cfg.Because it isn't exist, will be created
         and written in.
      -# You can modify values of camera config file,use 'r' command to update
          if you use User_ctrl function.

  - Modify Record:
      -# V1.2 May27,2013 Vince Create it.
*/
Camera::Camera( const unsigned int cam_index )
{
  if( NULL == fopen( CONFIG_FILE, "r" ))
    write_config_file();
  load_config_parameter();
  cam_ctrl_cnt = 1;
  key = 0;
  stop = 0;
  cam_table = NULL;

#if DEBUG_EXT_SYNC
  ext_sync = 0 ;
#endif

  n_buffers = 0;
  numCameras = 0;
  indexList = 0;

  char *j = new char[5];
	for (int i = pic_num_start;i < ( photo_amount + pic_num_start) ; i++)
	{
	  sprintf(j,"%d",i);
		strcpy( save_image_name[i-pic_num_start],img_name_prefix);
		strcat( save_image_name[i-pic_num_start],j);
		strcat( save_image_name[i-pic_num_start],".");
		strcat( save_image_name[i-pic_num_start],img_name_suffix[img_typ_index]);
	}
	delete j;
	j = NULL ;

#if Display_output
	cout << "Photograph name is from " << save_image_name[0] << " to " << save_image_name[photo_amount-1]  << endl;
	cout << endl;

	cout << "Searching for cameras... " << flush;
#endif

	icvInitCapture_V4L();

#if Display_output
	cout << numCameras << " camera(s) found:" << endl;

	for (int i=0, j=1; i<MAX_CAMERAS; i++)
	{
		if (indexList & 0x1)
		{
			cout << j << "  " << "/dev/video" << i << "  " << endl;
			j++;
		}
		indexList >>= 1;
	}
	numCameras = 0;
#endif

	/* Create the image and the window */
	Image = cvCreateImage(cvSize(photo_size_width, photo_size_height), IPL_DEPTH_8U, 3);
	Image->imageData = (char *) cvAlloc(Image->imageSize);

	view_window = cvCreateImage(cvSize(view_size_width, view_size_height), IPL_DEPTH_8U, 3);
	view_window->imageData = (char *) cvAlloc(view_window->imageSize);

	index = cam_index ;

#if Display_output
	gen_window_name();
	cvNamedWindow( Win_Name.c_str() );
#endif
	/* Initialize the webcam */
	//cout << " Open Device ..."<< endl;

	captureV4L = icvCaptureFromCAM_V4L( index );

	/* Set up the width and the height of the image */
	//cout << " Setting the resolution ... " << endl;
	icvSetPropertyCAM_V4L(captureV4L, CV_CAP_PROP_FRAME_WIDTH, photo_size_width); //view_size_width);
	icvSetPropertyCAM_V4L(captureV4L, CV_CAP_PROP_FRAME_HEIGHT, photo_size_height); //view_size_height);

	get_control_list();
#if 0
  cout << captureV4L->cap.driver << endl;
  cout << captureV4L->cap.card << endl;
  cout << captureV4L->cap.bus_info << endl;
  cout << captureV4L->cap.version << endl;


#endif
  if( 0 != load_cam_file() )
    exit(0);

  get_control_list();
#if Display_output
	list_ctrls( cam_table );
	help_menu();
#endif
}

/**
@fn Camera::Camera( const unsigned int cam_index )
@brief Camera constructor function with cam_index.

  - Version V1.2

  - Description:
      -# Constructor.
      -# Initialize the data member.
      -# Load config parameter from CONFIG_FILE.
      -# Generate photo name array.
      -# Search and open desire camera in current system.
      -# Set resolution.
      -# Get control list with link list method.
      -# Load camera config file.If it isn't exist,write it with link list contents.
      -# Refresh control list.
      -# Print control list and help message.
      -# index will be update with cam_index.

  - Parameter List:
      -# NULL.

  - Return Value:
      -# void.

  - Dependencies:
      -# OpenCV

  - Notice:
      -# If you want to use a new camera,you can set camera config-file name
         in CONFIG_FILE.Such as xxx.cfg.Because it isn't exist, will be created
         and written in.
      -# You can modify values of camera config file,use 'r' command to update
          if you use User_ctrl function.

  - Modify Record:
      -# V1.2 May27,2013 Vince Create it.
*/
Camera::Camera( const unsigned int cam_index, char *configfile,const char *camfile )
{
  if( NULL == fopen( configfile, "r" ))
    write_config_file();
  load_config_parameter( configfile );
  
  cam_config_file.empty();
  cam_config_file = camfile ;
  
  cam_ctrl_cnt = 1;
  key = 0;
  stop = 0;
  cam_table = NULL;

#if DEBUG_EXT_SYNC
  ext_sync = 0 ;
#endif

  n_buffers = 0;
  numCameras = 0;
  indexList = 0;

  char *j = new char[5];
	for (int i = pic_num_start;i < ( photo_amount + pic_num_start) ; i++)
	{
	  sprintf(j,"%d",i);
		strcpy( save_image_name[i-pic_num_start],img_name_prefix);
		strcat( save_image_name[i-pic_num_start],j);
		strcat( save_image_name[i-pic_num_start],".");
		strcat( save_image_name[i-pic_num_start],img_name_suffix[img_typ_index]);
	}
	delete j;
	j = NULL ;

#if Display_output
	cout << "Photograph name is from " << save_image_name[0] << " to " << save_image_name[photo_amount-1]  << endl;
	cout << endl;

	cout << "Searching for cameras... " << flush;
#endif

	icvInitCapture_V4L();

#if Display_output
	cout << numCameras << " camera(s) found:" << endl;

	for (int i=0, j=1; i<MAX_CAMERAS; i++)
	{
		if (indexList & 0x1)
		{
			cout << j << "  " << "/dev/video" << i << "  " << endl;
			j++;
		}
		indexList >>= 1;
	}
	numCameras = 0;
#endif

	/* Create the image and the window */
	Image = cvCreateImage(cvSize(photo_size_width, photo_size_height), IPL_DEPTH_8U, 3);
	Image->imageData = (char *) cvAlloc(Image->imageSize);

	view_window = cvCreateImage(cvSize(view_size_width, view_size_height), IPL_DEPTH_8U, 3);
	view_window->imageData = (char *) cvAlloc(view_window->imageSize);

	index = cam_index ;

#if Display_output
	gen_window_name();
	cvNamedWindow( Win_Name.c_str() );
#endif
	/* Initialize the webcam */
	//cout << " Open Device ..."<< endl;

	captureV4L = icvCaptureFromCAM_V4L( index );

	/* Set up the width and the height of the image */
	//cout << " Setting the resolution ... " << endl;
	icvSetPropertyCAM_V4L(captureV4L, CV_CAP_PROP_FRAME_WIDTH, photo_size_width); //view_size_width);
	icvSetPropertyCAM_V4L(captureV4L, CV_CAP_PROP_FRAME_HEIGHT, photo_size_height); //view_size_height);

	get_control_list();
#if 0
  cout << captureV4L->cap.driver << endl;
  cout << captureV4L->cap.card << endl;
  cout << captureV4L->cap.bus_info << endl;
  cout << captureV4L->cap.version << endl;


#endif
  //if( 0 != load_cam_file() )
  //  exit(0);
  load_cam_file( camfile );
  refresh_cam_link();
  refresh_cam_link();

  get_control_list();
#if Display_output
	list_ctrls( cam_table );
	help_menu();
#endif
}

/// Camera deconstructor function.
Camera::~Camera()
{
  // class can deconstruct its memory ?
  //delete img_name_prefix;

  /* Uninit the webcam */
	icvCloseCAM_V4L(captureV4L);

	/* Free memory */
	cvFree(&captureV4L);

  /* Release the image and close the window */
  cvReleaseImage(&Image);
  cvDestroyWindow( Win_Name.c_str() );
}

/**
@fn void Camera::display()
@brief Grab a frame and fill to the view window.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# NULL.

  - Return Value:
      -# void.

  - Dependencies:
      -# OpenCV

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
#if Display_output
void Camera::display()
{
  icvGrabFrameCAM_V4L(captureV4L);
  Image = icvRetrieveFrameCAM_V4L(captureV4L, 0);
  cvResize( Image, view_window );
  cvShowImage( Win_Name.c_str(), view_window);
}
#endif

/**
@fn Mat Camera::take_photo()
@brief A function can grab a frame and save as Mat object.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# NULL.

  - Return Value:
      -# A Mat object.

  - Dependencies:
      -# OpenCV

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
Mat Camera::take_photo()
{
  icvGrabFrameCAM_V4L(captureV4L);
  Image_photo = icvRetrieveFrameCAM_V4L(captureV4L, 0);
  Mat ret_mat( Image_photo );
  return ret_mat;
}

/**
@fn void Camera::take_photo( cv::Mat &var )
@brief A function can grab a frame and fill a Mat with the frame.

  - Version V1.0

  - Description:
      -# .

  - Parameter List:
      -# var # A reference to a mat object. Will be fill by this function.

  - Return Value:
      -# Void.

  - Dependencies:
      -# OpenCV

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void Camera::take_photo( cv::Mat &var )
{
  //User_ctrl( ' ', NULL );
  icvGrabFrameCAM_V4L(captureV4L);
  Image_photo = icvRetrieveFrameCAM_V4L(captureV4L, 0);
  Mat ret_mat( Image_photo, 1 );
  var = ret_mat;
}

/**
@fn void Camera::take_photo( char *photo_name )
@brief A function take a photo with photo_name.

  - Version V1.0

  - Description:
      -# This function call User_ctrl(' ',photo_name) to do it easy.

  - Parameter List:
      -# photo_name # A char point point to a photo name array.

  - Return Value:
      -# Void.

  - Dependencies:
      -# OpenCV

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void Camera::take_photo( char *photo_name )
{
  User_ctrl( ' ', photo_name );
}

/**
@fn void validate( boost::any &v, const vector<string> &values, cfg_resoltype_Arr*,int)
@brief A validate used by boost::program_option.

  - Version V1.0

  - Description:
      -# To compatiable with cfg_resoltype_Arr.

  - Parameter List:
      -# v                  # boost::any      Don't care.
      -# values             # A string vector Don't care.
      -# cfg_resoltype_Arr* # A user define data type.
      -# int                # Syetem define   Don't care.

  - Return Value:
      -# Void.

  - Dependencies:
      -# boost::program_option

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void validate( boost::any &v, const vector<string> &values, cfg_resoltype_Arr*,int)
{
  cfg_resoltype_Arr arr;
  vector<int> val;

  if(values.size() == 0)
  {
    throw po::validation_error(po::validation_error::invalid_option_value,
                               "invalid cfgArr specification");
  }

//#if 1
  for( vector<string>::const_iterator it = values.begin();
        it != values.end(); ++it )
  {
    stringstream ss(*it);
    copy( istream_iterator<int>(ss), istream_iterator<int>(),back_inserter(val));
    if( !ss.eof() )
    {
      throw po::validation_error( po::validation_error::invalid_option_value,
                                "invalid array ");
    }
  }
//#endif

  //for( vector<string>::const_iterator i = val.begin(), int j = 0; i != val.end(); (++i,j++) )
  //for( int i = val.size(); i >= 0; i)
  //cout << val[9] ;
  for( int i=0; i < Resolution_cnt; ++i )
  {
    //arr.arr[0] = boost::lexical_cast<int>(values.at(1));// val[i];
    //arr.arr[1] = boost::lexical_cast<int>(values.at(2));// val[i];
    arr.arr[i] = val[i];
  }
  v = arr;
}

/**
@fn void validate( boost::any &v, const vector<string> &values, cfg_ptype_Arr*,int)
@brief A validate used by boost::program_option.

  - Version V1.0

  - Description:
      -# To compatiable with cfg_ptype_Arr.

  - Parameter List:
      -# v                  # boost::any      Don't care.
      -# values             # A string vector Don't care.
      -# cfg_ptype_Arr*     # A user define data type.
      -# int                # Syetem define   Don't care.

  - Return Value:
      -# Void.

  - Dependencies:
      -# boost::program_option

  - Notice:
      -# .

  - Modify Record:
      -# .
*/
void validate( boost::any &v, const vector<string> &values, cfg_ptype_Arr*,int)
{
  cfg_ptype_Arr arr;
  vector<string> val;

  if(values.size() == 0)
  {
    throw po::validation_error(po::validation_error::invalid_option_value,
                               "invalid cfgArr specification");
  }

//#if 1
  for( vector<string>::const_iterator it = values.begin();
        it != values.end(); ++it )
  {
    stringstream ss(*it);
    copy( istream_iterator<string>(ss), istream_iterator<string>(),back_inserter(val));
    if( !ss.eof() )
    {
      throw po::validation_error( po::validation_error::invalid_option_value,
                                "invalid array ");
    }
  }
//#endif

  //for( vector<string>::const_iterator i = val.begin(), int j = 0; i != val.end(); (++i,j++) )
  //for( int i = val.size(); i >= 0; i)
  //cout << val[9] ;
  for( int i=0; i < Photo_name_type_cnt; ++i )
  {
    //arr.arr[0] = boost::lexical_cast<int>(values.at(1));// val[i];
    //arr.arr[1] = boost::lexical_cast<int>(values.at(2));// val[i];
    arr.arr[i] = val[i];
  }
  v = arr;
}

/**
@fn int Camera::load_config_parameter( void )
@brief Load config parameter from config file( CONFIG_FILE ).

  - Version V1.0

  - Description:
      -# Load the config parameter need by program.Can be run more one time to update parameters.

  - Parameter List:
      -# NULL.

  - Return Value:
      -# 0 if OK.

  - Dependencies:
      -# boost::program_option

  - Notice:
      -# If you want to use array and other types ,you need define it and validate.

  - Modify Record:
      -# V1.1 May25,2013 Vince Remove the fix camera config file.Change to use dynamic name.
                               Changes 3 places,not define in hidden object, assigment,config.cfg.
*/
int Camera::load_config_parameter( void )
{
  //namespace po = boost::program_options;

  int var_tmp;
  string cfg_file;

  cfg_resoltype_Arr w;
  cfg_resoltype_Arr h;

  cfg_ptype_Arr ptyp;

/// single group options
  po::options_description generic("Generic options");
  generic.add_options()
    ("help,h", "print help message")
//    ("tmp,t",po::value<int>()->default_value(555),"for test")
    ("version,v","to do ")
    ("config,c",po::value<string>(&cfg_file)->default_value(
      CONFIG_FILE),"config file")
    ;

  po::options_description config("Configuration");
  config.add_options()
    ("optimization", po::value<int>(&var_tmp)->default_value(10),
            "optimization level")
    ("include-path,I",
           po::value< vector<string> >()->composing(),
           "include path")
    ;

  po::options_description hidden("Hidden options");
  hidden.add_options()
    //("input-file", po::value< vector<string> >(), "input file")
    ("view_size_width", po::value<int>(), "view_size_width")
    ("view_size_height", po::value<int>(), "view_size_height")
    ("photo_size_width", po::value<int>(), "photo_size_width")
    ("photo_size_height", po::value<int>(), "photo_size_height")
    ("index", po::value<int>(), "index which camera")
    ("pic_cnt", po::value<int>(), "index which camera")
    ("img_typ_index", po::value<int>(), "index which camera")
    ("resol_index", po::value<int>(), "index which camera")
    ("resol_cnt", po::value<int>(), "index which camera")
    ("pic_num_start", po::value<int>(), "index which camera")
    ("photo_amount", po::value<int>(), "index which camera")
    ("photo_name_length", po::value<int>(), "index which camera")
    ("resolution_w", po::value<cfg_resoltype_Arr>(&w)->multitoken(), "resolution_w")
    ("resolution_h", po::value<cfg_resoltype_Arr>(&h)->multitoken(), "resolution_h")
    ("photo_type", po::value<cfg_ptype_Arr>(&ptyp)->multitoken(), "img_name_suffix")
    ("image_name_prefix", po::value<string>(), "image_name_prefix")
    //("camera_cfg_file", po::value<string>(), "camera setting store file")
    ;

// further group orginazition
#if 1
  po::options_description cmdline_options;
  //cmdline_options.add(generic).add(config).add(hidden);
  cmdline_options.add(generic);
#endif
  po::options_description config_file_options;
//  config_file_options.add(config).add(hidden);
  config_file_options.add(hidden);

#if 0
  po::options_description visible("Allowed options");
  visible.add(generic).add(config);
#endif
// load para from fix location
#if 0
//  po::positional_options_description p;
//  p.add("input-file", -1);
#endif

// declair value-option map object and fill it
  po::variables_map vmap;
#if 1
  //store(po::command_line_parser(argc, argv).options(cmdline_options).
        //  positional(p).run(), vmap);
  // store(po::command_line_parser(0, NULL).options(cmdline_options).
      //  positional(p).run(), vmap);
  store(po::command_line_parser(0, NULL).options(cmdline_options).run(), vmap);
  notify(vmap);
#endif
  //po::store(parse_command_line(argc,argv,load_var),vmap);

  ifstream cfg_f( cfg_file.c_str() );
  if( !cfg_f )
  {
    cout << "can't open config file " << cfg_file << endl;
  }
  else
  {
    po::store(parse_config_file(cfg_f,config_file_options),vmap);
    po::notify(vmap);
  }

#if 0
// check which option used
  if(vmap.count("help"))
  {
    cout << visible << endl;  // print the options details
    return 0;
  }

  if (vmap.count("version"))
  {
    cout << "Current version is: " << endl;
    return 0;
  }
#endif

// get value from multi sources
  //index = vmap["tmp"].as<int>();
  view_size_width = vmap["view_size_width"].as<int>();
  view_size_height = vmap["view_size_height"].as<int>();
  photo_size_width = vmap["photo_size_width"].as<int>();
  photo_size_height = vmap["photo_size_height"].as<int>();
  index = vmap["index"].as<int>();
  img_typ_index = vmap["img_typ_index"].as<int>();
  pic_cnt = vmap["pic_cnt"].as<int>();
  pic_num_start = vmap["pic_num_start"].as<int>();
  photo_amount = vmap["photo_amount"].as<int>();
  resol_index = vmap["resol_index"].as<int>();
  int resol_cnt = vmap["resol_cnt"].as<int>();

  for( int i=0; i < Resolution_cnt; i++ )
  {
    resol[i][0] = w.arr[i];
    resol[i][1] = h.arr[i];
    //cout << resol[i][0] << " x " << resol[i][1] << endl;
  }

  //*img_name_suffix = new char(Photo_name_type_cnt*P_type_leng);
  for(int i=0;i<Photo_name_type_cnt;i++)
  {
    img_name_suffix[i] = (char *)malloc(sizeof(char)*P_type_leng);
  }

  for( int i=0; i < Photo_name_type_cnt; i++ )
  {
    //img_name_suffix[i] =(char *) ptyp.arr[i].c_str();
    strcpy( img_name_suffix[i], ptyp.arr[i].c_str());
    //cout << img_name_suffix[i] << endl;
    //cout << ptyp.arr[i].c_str() << endl;
  }

  string img_prx( vmap["image_name_prefix"].as<string>() );
  //img_name_prefix =(char *) img_prx.c_str();
  img_name_prefix =(char *) new char(6);
  strcpy( img_name_prefix, img_prx.c_str());

  int photo_name_length = vmap["photo_name_length"].as<int>();

  /// allocate memory to photo name array
  //p2D2arr( save_image_name, photo_amount, photo_name_length );

  //cam_file = vmap["camera_cfg_file"].as<string>() ;

  return 0;
}

/**
@fn int Camera::load_config_parameter( void )
@brief Load config parameter from config file( CONFIG_FILE ).

  - Version V1.0

  - Description:
      -# Load the config parameter need by program.Can be run more one time to update parameters.

  - Parameter List:
      -# NULL.

  - Return Value:
      -# 0 if OK.

  - Dependencies:
      -# boost::program_option

  - Notice:
      -# If you want to use array and other types ,you need define it and validate.

  - Modify Record:
      -# V1.1 May25,2013 Vince Remove the fix camera config file.Change to use dynamic name.
                               Changes 3 places,not define in hidden object, assigment,config.cfg.
*/
int Camera::load_config_parameter( const char *configfile )
{
  //namespace po = boost::program_options;

  int var_tmp;
  string cfg_file;

  cfg_resoltype_Arr w;
  cfg_resoltype_Arr h;

  cfg_ptype_Arr ptyp;

/// single group options
  po::options_description generic("Generic options");
  generic.add_options()
    ("help,h", "print help message")
//    ("tmp,t",po::value<int>()->default_value(555),"for test")
    ("version,v","to do ")
    ("config,c",po::value<string>(&cfg_file)->default_value(
     configfile),"config file")
    ;

  po::options_description config("Configuration");
  config.add_options()
    ("optimization", po::value<int>(&var_tmp)->default_value(10),
            "optimization level")
    ("include-path,I",
           po::value< vector<string> >()->composing(),
           "include path")
    ;

  po::options_description hidden("Hidden options");
  hidden.add_options()
    //("input-file", po::value< vector<string> >(), "input file")
    ("view_size_width", po::value<int>(), "view_size_width")
    ("view_size_height", po::value<int>(), "view_size_height")
    ("photo_size_width", po::value<int>(), "photo_size_width")
    ("photo_size_height", po::value<int>(), "photo_size_height")
    ("index", po::value<int>(), "index which camera")
    ("pic_cnt", po::value<int>(), "index which camera")
    ("img_typ_index", po::value<int>(), "index which camera")
    ("resol_index", po::value<int>(), "index which camera")
    ("resol_cnt", po::value<int>(), "index which camera")
    ("pic_num_start", po::value<int>(), "index which camera")
    ("photo_amount", po::value<int>(), "index which camera")
    ("photo_name_length", po::value<int>(), "index which camera")
    ("resolution_w", po::value<cfg_resoltype_Arr>(&w)->multitoken(), "resolution_w")
    ("resolution_h", po::value<cfg_resoltype_Arr>(&h)->multitoken(), "resolution_h")
    ("photo_type", po::value<cfg_ptype_Arr>(&ptyp)->multitoken(), "img_name_suffix")
    ("image_name_prefix", po::value<string>(), "image_name_prefix")
    //("camera_cfg_file", po::value<string>(), "camera setting store file")
    ;

// further group orginazition
#if 1
  po::options_description cmdline_options;
  //cmdline_options.add(generic).add(config).add(hidden);
  cmdline_options.add(generic);
#endif
  po::options_description config_file_options;
//  config_file_options.add(config).add(hidden);
  config_file_options.add(hidden);

#if 0
  po::options_description visible("Allowed options");
  visible.add(generic).add(config);
#endif
// load para from fix location
#if 0
//  po::positional_options_description p;
//  p.add("input-file", -1);
#endif

// declair value-option map object and fill it
  po::variables_map vmap;
#if 1
  //store(po::command_line_parser(argc, argv).options(cmdline_options).
        //  positional(p).run(), vmap);
  // store(po::command_line_parser(0, NULL).options(cmdline_options).
      //  positional(p).run(), vmap);
  store(po::command_line_parser(0, NULL).options(cmdline_options).run(), vmap);
  notify(vmap);
#endif
  //po::store(parse_command_line(argc,argv,load_var),vmap);

  ifstream cfg_f( cfg_file.c_str() );
  if( !cfg_f )
  {
    cout << "can't open config file " << cfg_file << endl;
  }
  else
  {
    po::store(parse_config_file(cfg_f,config_file_options),vmap);
    po::notify(vmap);
  }

#if 0
// check which option used
  if(vmap.count("help"))
  {
    cout << visible << endl;  // print the options details
    return 0;
  }

  if (vmap.count("version"))
  {
    cout << "Current version is: " << endl;
    return 0;
  }
#endif

// get value from multi sources
  //index = vmap["tmp"].as<int>();
  view_size_width = vmap["view_size_width"].as<int>();
  view_size_height = vmap["view_size_height"].as<int>();
  photo_size_width = vmap["photo_size_width"].as<int>();
  photo_size_height = vmap["photo_size_height"].as<int>();
  index = vmap["index"].as<int>();
  img_typ_index = vmap["img_typ_index"].as<int>();
  pic_cnt = vmap["pic_cnt"].as<int>();
  pic_num_start = vmap["pic_num_start"].as<int>();
  photo_amount = vmap["photo_amount"].as<int>();
  resol_index = vmap["resol_index"].as<int>();
  int resol_cnt = vmap["resol_cnt"].as<int>();

  for( int i=0; i < Resolution_cnt; i++ )
  {
    resol[i][0] = w.arr[i];
    resol[i][1] = h.arr[i];
    //cout << resol[i][0] << " x " << resol[i][1] << endl;
  }

  //*img_name_suffix = new char(Photo_name_type_cnt*P_type_leng);
  for(int i=0;i<Photo_name_type_cnt;i++)
  {
    img_name_suffix[i] = (char *)malloc(sizeof(char)*P_type_leng);
  }

  for( int i=0; i < Photo_name_type_cnt; i++ )
  {
    //img_name_suffix[i] =(char *) ptyp.arr[i].c_str();
    strcpy( img_name_suffix[i], ptyp.arr[i].c_str());
    //cout << img_name_suffix[i] << endl;
    //cout << ptyp.arr[i].c_str() << endl;
  }

  string img_prx( vmap["image_name_prefix"].as<string>() );
  //img_name_prefix =(char *) img_prx.c_str();
  img_name_prefix =(char *) new char(6);
  strcpy( img_name_prefix, img_prx.c_str());

  int photo_name_length = vmap["photo_name_length"].as<int>();

  /// allocate memory to photo name array
  //p2D2arr( save_image_name, photo_amount, photo_name_length );

  //cam_file = vmap["camera_cfg_file"].as<string>() ;

  return 0;
}

/**
@fn void Camera::c_2D_arr( int **p, int row, int coln)
@brief Create a int 2D array.

  - Version V1.0

  - Description:
      -# The 2D char array with row and coln.

  - Parameter List:
      -# **p   # A 2D int point.
      -# row   # Row , 1st demension counter.
      -# coln  # Coln , 2nd demension counter.

  - Return Value:
      -# void.

  - Dependencies:
      -# .

  - Notice:
       -# .

  - Modify Record:
       -# .
*/
void Camera::c_2D_arr( int **p, int row, int coln)
{
  //p = (int **) malloc(sizeof(int *)*row);
  p = (int **) new int*[row];
  if( p == NULL )
    cout << "error" << endl;
  for( int i=0; i<row; i++)
  {
    //p[i] = (int *) malloc(sizeof(int)*coln);
    p[i] = (int *) new int[coln];
    #if DEBUG
    cout << i << endl;
    for(int j=0;j<2;j++)
    {
      p[i][j] = 55;
      cout << p[i][j] << endl;
    }
    #endif
  }
}

/**
@fn void Camera::c_2D_arr( char **p, int row, int coln)
@brief Create a char 2D array.

  - Version V1.0

  - Description:
      -# The 2D char array with row and coln.

  - Parameter List:
      -# **p   # A 2D char point.
      -# row   # Row , 1st demension counter.
      -# coln  # Coln , 2nd demension counter.

  - Return Value:
      -# void.

  - Dependencies:
      -# .

  - Notice:
       -# .

  - Modify Record:
       -# .
*/
void Camera::c_2D_arr( char **p, int row, int coln)
{
  //p = (char **) malloc(sizeof(char*)*row);
  p = new char*[row];
  for( int i=0; i<row; i++)
  {
    //p[i] = (char *) malloc(sizeof(char)*coln);
    p[i] = new char[coln];
  }
}

