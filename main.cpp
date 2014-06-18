/**
@file main.cpp
@brief A test program for demo.
      The test program is a demo for user. And user can use mostly all of
the functions with the console.

  - Version: V1.2

  - Function List:
     -# main(), the main function to show the API and functions.

  - Dependencies:
     -# V4L2
     -# OpenCV
     -# Boost::program_option

  - Notice:
     -# Only for demo.
     -# You can change "#define TEST" value to select the test segmentation.
     -# Add initialize a static member of Camera.It be used to count the photo number.

  - Modify Record:
     -# V1.0 Mar27,2013. Vince create.
     -# V1.1 May21,2013. Vince complete.
     -# V1.2 May27,2013. Vince Add initialize a static member before main();
*/

#include <iostream>
#include <ctime>
#include "camera.h"

#define TEST 4   ///< To choose which segmantation will enable.
#define Delay_snapshot 5  ///< delay time for capture
#define Delay_Upper 6  ///< delay time before capture
#define Delay_Lower 5  ///< delay time after capture

#define Delay_cap 1  ///< wether delay capture

using namespace std;
using namespace cv;

//unsigned int Camera::pic_cnt = 0 ;   ///< Init a counter of photo number(static member)

/**
@fn int main( int argc, char **argv )
@brief A main function call the camera class to show the usage of class and program.

  - Version: V1.0

  - <b>Description:</b>
      -# A main function

  - Parameter List:
      -# argc The command line counter.
      -# argv A 2D array to contain parameters by user.

  - Return Value:
      -# 0 if OK.

  - Dependencies:
      -# V4L2
      -# OpenCV
      -# Boost::program_option

  - Notice:
      -# You can set the different value of "TEST" to enable variable segmentation.

  - Modify Record:
      -# 2013-5-17 Vince Add a method Camera.set_cam_ctrl( id, value ) to demo.
*/
int main( int argc, char *argv[] )
{
  time_t start_1 = 0 , end_1 = 0 ;
  time_t start_2 = 0 , end_2 = 0 ;
  bool Cap_Flag = 0 ;
  int tmp1 = 0 , tmp2 = 0 ;
  cout << "Iput camera_A num:" << endl;
  cin >> tmp1 ;
  cout << "Iput camera_B num:" << endl;
  cin >> tmp2 ;

  //define a camera object
	//Camera Camera_A( 0 );
	//Camera Camera_B( 1 );
	//Camera Camera_A;

	Camera Camera_A( tmp1, "config_a.cfg", "SMI_a.cfg");
	Camera Camera_B( tmp2, "config_b.cfg", "SMI_b.cfg");

#if (TEST == 0)
  char *p_name = new char(14);
  cout << "input photo name, no or your name:";
  cin >> p_name;

  if( p_name[0] == 'n' && p_name[1] == 'o' )
	  Camera_A.User_ctrl( ' ', NULL );
  else
  {
    cout << p_name << endl;
    Camera_A.User_ctrl( ' ', p_name );
    //Camera_A.
  }
  delete p_name;
  p_name = NULL;

#elif (TEST == 1)
  // camera return Mat , and write it to file.
  Mat getmat = Camera_A.take_photo();
  FileStorage fs("getmatfile.xml", FileStorage::WRITE);
  fs << "gematfile" << getmat;
  fs.release();

#elif (TEST == 2)
  Mat mat2;
  Camera_A.take_photo( mat2 );
  FileStorage fs("getmatfile2.xml", FileStorage::WRITE);
  fs << "gematfile2" << mat2;
  fs.release();
#elif (TEST == 3)

#else
  Camera_A.set_cam_ctrl( BRIGHTNESS, 3 );
	while( Camera_A.stop != 1 )
	{
	  Camera_A.display();

	  //sleep( 1 );
	  Camera_B.display();
	  //Camera_B.display();
#if Delay_cap
    if( Camera_A.ext_sync == 1 )
    {

      // sleep( 6 );
      if( start_1 == 0 )
        start_1 = time( NULL );
      if( Cap_Flag == 0 )
        end_1 = time( NULL );
      //cout << start_1 << endl;
      if( (end_1 - start_1) >= Delay_Upper )
      {
        Camera_A.User_ctrl( ' ', NULL );
        Cap_Flag = 1 ;
        end_1 = 0 ;
      }

      if( (Cap_Flag == 1) && (start_2 == 0) )
        start_2 = time( NULL );
      if(Cap_Flag == 1)
        end_2 = time( NULL );

      if( (end_2 - start_2) >= Delay_Lower )
      {
        start_1 = 0 ;
        start_2 = 0 ;
        Cap_Flag = 0 ;
        end_2 = 0 ;
      }
      // sleep( 5 );

    }
#endif
	  Camera_B.key = Camera_A.key = cvWaitKey(10);
	  //Camera_B.key = Camera_A.key = cvWaitKey(10);
	  Camera_A.User_ctrl( Camera_A.key, NULL );
	  Camera_B.User_ctrl( Camera_B.key, NULL );
	  //Camera_B.User_ctrl( Camera_B.key );
	}
#endif

  cout << "exit..." << endl;
	return 0;
}
