#include "common_header.h"
#include "texture.h"
#include "4PI.h"
#include "renderScene.h"
#include <time.h>

const bool l_FullScreen = true;
const bool l_MultiSampling = false;

ovrHmd l_Hmd;
ovrHmdDesc l_HmdDesc;
ovrFovPort l_EyeFov[2];
ovrGLConfig l_Cfg;
ovrEyeRenderDesc l_EyeRenderDesc[2];

bool isLoaded = true; // 控制同一帧渲染不要加载两次
INIINFO iniInfo;

// =============================================================================

void readINI()
{
	char ini_path[128];
	sprintf(ini_path, "./play.ini");
	iniInfo.FilePath4PI = new char[128];
	GetPrivateProfileStringA("IN", "FilePath4PI", "", iniInfo.FilePath4PI, 128, ini_path);
	iniInfo.FilePathBox = new char[128];
	GetPrivateProfileStringA("IN", "FilePathBox", "", iniInfo.FilePathBox, 128, ini_path);
	iniInfo.Mode = GetPrivateProfileIntA("IN", "Mode", 1, ini_path);
	iniInfo.BeginSequence = GetPrivateProfileIntA("IN", "BeginSequence", 1, ini_path);
	iniInfo.EndSequence = GetPrivateProfileIntA("IN", "EndSequence", 100, ini_path);
	iniInfo.FrameRate = GetPrivateProfileIntA("IN", "FrameRate", 25, ini_path);
}

// =============================================================================

static void ErrorCallback(int p_Error, const char* p_Description)
{
	fputs(p_Description, stderr);
}

// =============================================================================

static void KeyCallback(GLFWwindow* p_Window, int p_Key, int p_Scancode, int p_Action, int p_Mods)
{
	if (p_Key == GLFW_KEY_ESCAPE && p_Action == GLFW_PRESS) 
		glfwSetWindowShouldClose(p_Window, GL_TRUE);
}

// =============================================================================

static void WindowSizeCallback(GLFWwindow* p_Window, int p_Width, int p_Height)
{
	l_Cfg.OGL.Header.RTSize.w = p_Width; 
	l_Cfg.OGL.Header.RTSize.h = p_Height;

	int l_DistortionCaps = ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp;
	ovrHmd_ConfigureRendering(l_Hmd, &l_Cfg.Config, l_DistortionCaps, l_EyeFov, l_EyeRenderDesc);
}


// =============================================================================

int main(void)
{
	ovr_Initialize();

	l_Hmd = ovrHmd_Create(0);
	if (!l_Hmd) l_Hmd = ovrHmd_CreateDebug(ovrHmd_DK1);

	ovrHmd_GetDesc(l_Hmd, &l_HmdDesc);

	// Start the sensor(传感器) which provides the Rift’s pose and motion.
	ovrHmd_StartSensor(l_Hmd, ovrSensorCap_Orientation | ovrSensorCap_YawCorrection | ovrSensorCap_Position, ovrSensorCap_Orientation);

	GLFWwindow* l_Window;

	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit()) exit(EXIT_FAILURE);

	if (l_MultiSampling) glfwWindowHint(GLFW_SAMPLES, 4); else glfwWindowHint(GLFW_SAMPLES, 0);

	ovrSizei l_ClientSize;
	if (l_FullScreen)
	{
		l_ClientSize.w = l_HmdDesc.Resolution.w; // 1280 for DK1...
		l_ClientSize.h = l_HmdDesc.Resolution.h; // 800 for DK1...
		l_Window = glfwCreateWindow(l_ClientSize.w, l_ClientSize.h, "GLFW Oculus Rift Test", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		l_ClientSize.w = 640;
		l_ClientSize.h = 480;
		l_Window = glfwCreateWindow(l_ClientSize.w, l_ClientSize.h, "GLFW Oculus Rift Test", NULL, NULL);
	}

	if (!l_Window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(l_Window);

	GLenum l_Result = glewInit();
	if (l_Result!=GLEW_OK)   
	{
		printf("glewInit() error.\n");
		exit(EXIT_FAILURE);
	}


	int l_Major = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MAJOR);
	int l_Minor = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MINOR);
	int l_Profile = glfwGetWindowAttrib(l_Window, GLFW_OPENGL_PROFILE);
	printf("OpenGL: %d.%d ", l_Major, l_Minor);
	if (l_Major>=3) // Profiles introduced in OpenGL 3.0...
	{
		if (l_Profile==GLFW_OPENGL_COMPAT_PROFILE) 
			printf("GLFW_OPENGL_COMPAT_PROFILE\n"); 
		else 
			printf("GLFW_OPENGL_CORE_PROFILE\n");
	}
	printf("Vendor: %s\n", (char*)glGetString(GL_VENDOR));
	printf("Renderer: %s\n", (char*)glGetString(GL_RENDERER));


	ovrSizei l_TextureSizeLeft = ovrHmd_GetFovTextureSize(l_Hmd, ovrEye_Left, l_HmdDesc.DefaultEyeFov[0], 1.0f);
	ovrSizei l_TextureSizeRight = ovrHmd_GetFovTextureSize(l_Hmd, ovrEye_Right, l_HmdDesc.DefaultEyeFov[1], 1.0f);
	ovrSizei l_TextureSize;
	l_TextureSize.w = l_TextureSizeLeft.w + l_TextureSizeRight.w;
	l_TextureSize.h = (l_TextureSizeLeft.h > l_TextureSizeRight.h ? l_TextureSizeLeft.h : l_TextureSizeRight.h);

	GLuint l_FBOId;
	glGenFramebuffers(1, &l_FBOId);
	glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);

	GLuint l_TextureId;
	glGenTextures(1, &l_TextureId);
	glBindTexture(GL_TEXTURE_2D, l_TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, l_TextureSize.w, l_TextureSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint l_DepthBufferId;
	glGenRenderbuffers(1, &l_DepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, l_DepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, l_TextureSize.w, l_TextureSize.h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, l_DepthBufferId);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, l_TextureId, 0);

	GLenum l_GLDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, l_GLDrawBuffers);

	GLenum l_Check = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (l_Check!=GL_FRAMEBUFFER_COMPLETE)
	{
		printf("There is a problem with the FBO.\n");
		exit(EXIT_FAILURE);
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Oculus Rift eye configurations...
	l_EyeFov[0] = l_HmdDesc.DefaultEyeFov[0];
	l_EyeFov[1] = l_HmdDesc.DefaultEyeFov[1];

	l_Cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	l_Cfg.OGL.Header.Multisample = (l_MultiSampling ? 1 : 0);
	l_Cfg.OGL.Header.RTSize.w = l_ClientSize.w;
	l_Cfg.OGL.Header.RTSize.h = l_ClientSize.h;
	l_Cfg.OGL.Window = glfwGetWin32Window(l_Window);

	int l_DistortionCaps = ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp;
	ovrHmd_ConfigureRendering(l_Hmd, &l_Cfg.Config, l_DistortionCaps, l_EyeFov, l_EyeRenderDesc);

	ovrGLTexture l_EyeTexture[2];
	l_EyeTexture[0].OGL.Header.API = ovrRenderAPI_OpenGL;
	l_EyeTexture[0].OGL.Header.TextureSize.w = l_TextureSize.w;
	l_EyeTexture[0].OGL.Header.TextureSize.h = l_TextureSize.h;
	l_EyeTexture[0].OGL.Header.RenderViewport.Pos.x = 0;
	l_EyeTexture[0].OGL.Header.RenderViewport.Pos.y = 0;
	l_EyeTexture[0].OGL.Header.RenderViewport.Size.w = l_TextureSize.w/2;
	l_EyeTexture[0].OGL.Header.RenderViewport.Size.h = l_TextureSize.h;
	l_EyeTexture[0].OGL.TexId = l_TextureId;

	// 右眼与左眼相同，除了x的位置
	l_EyeTexture[1] = l_EyeTexture[0];
	l_EyeTexture[1].OGL.Header.RenderViewport.Pos.x = (l_TextureSize.w+1)/2;
	
	glfwSetKeyCallback(l_Window, KeyCallback);
	glfwSetWindowSizeCallback(l_Window, WindowSizeCallback);

	readINI();

	int frameSequence = iniInfo.BeginSequence;

	initScene();

	clock_t begin = clock();
	clock_t end = clock();

	while (!glfwWindowShouldClose(l_Window) && frameSequence <= iniInfo.EndSequence)
	{
		if ( (end - begin) >= 1000/iniInfo.FrameRate) // 控制帧率
		{
			begin = end;

			ovrFrameTiming m_HmdFrameTiming = ovrHmd_BeginFrame(l_Hmd, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);
			glClearColor(0.5f, 0.4f, 0.3f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (int l_EyeIndex = 0; l_EyeIndex < ovrEye_Count; l_EyeIndex++)
			{
				ovrEyeType l_Eye = l_HmdDesc.EyeRenderOrder[l_EyeIndex];
				ovrPosef l_EyePose = ovrHmd_BeginEyeRender(l_Hmd, l_Eye);

				glViewport(l_EyeTexture[l_Eye].OGL.Header.RenderViewport.Pos.x,
					l_EyeTexture[l_Eye].OGL.Header.RenderViewport.Pos.y,
					l_EyeTexture[l_Eye].OGL.Header.RenderViewport.Size.w,
					l_EyeTexture[l_Eye].OGL.Header.RenderViewport.Size.h
					);


				OVR::Matrix4f l_ProjectionMatrix = OVR::Matrix4f(ovrMatrix4f_Projection(
					l_EyeRenderDesc[l_Eye].Fov, 0.01f, 10000.0f, true));
			

				OVR::Matrix4f l_Translate = OVR::Matrix4f::Translation(l_EyeRenderDesc[l_Eye].ViewAdjust);

				OVR::Quatf l_Orientation = OVR::Quatf(l_EyePose.Orientation);
				OVR::Matrix4f l_ModelViewMatrix = OVR::Matrix4f(l_Orientation.Inverted());


				OVR::Matrix4f l_MVPMatiix = l_ProjectionMatrix * l_Translate * l_ModelViewMatrix;

				mat4 MVPMatrix = transpose(make_mat4(&l_MVPMatiix.M[0][0]));

				if (l_EyeIndex == 0)
					isLoaded = true;
				else
					isLoaded = false;

				renderScene(MVPMatrix, frameSequence, isLoaded);
			
				releaseScene();

				ovrHmd_EndEyeRender(l_Hmd, l_Eye, l_EyePose, &l_EyeTexture[l_Eye].Texture);
			}
			frameSequence++;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			ovrHmd_EndFrame(l_Hmd);

			glfwPollEvents();

		}
		end = clock();
	}

	glfwDestroyWindow(l_Window);

	glfwTerminate();

	ovrHmd_Destroy(l_Hmd);
	ovr_Shutdown();

	exit(EXIT_SUCCESS);
}
