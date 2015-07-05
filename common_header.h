#pragma once

/*
* ͨ��ͷ�ļ�
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;


#include <GL/glew.h>
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <OVR.h>
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib") 
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "libovr.lib")
#pragma comment(lib, "libovrd.lib")
#pragma comment(lib, "winmm" )

// gl��ѧ�⣬���ھ���������ֱ任
#include "./glm-0.9.4.0/glm/glm.hpp"
#include "./glm-0.9.4.0/glm/gtc/matrix_transform.hpp"
#include "./glm-0.9.4.0/glm/gtc/type_ptr.hpp"
#include "./glm-0.9.4.0/glm/gtx/rotate_vector.hpp"
using namespace glm;

const float PI = atan(1.0f)*4.0f;

#include "./FreeImage/Dist/FreeImage.h"
#pragma comment(lib, "./FreeImage/Dist/FreeImage.lib")

// ��ʼ���ļ�
typedef struct  
{
	int Mode;
	char* FilePath4PI;
	char* FilePathBox;
	int BeginSequence;
	int EndSequence;
	int FrameRate;
} INIINFO;

