#include "renderScene.h"
#include "shader.h"
#include "texture.h"
#include "4PI.h"
#include "skybox.h"

extern INIINFO iniInfo;

CShaderProgram sp4PI;
C4PI c4PI;

CShaderProgram spSkyBox;
CSkybox cSkybox;


// ======================================================================================

void initScene(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	if (iniInfo.Mode == 1) 
	{
		string filePath = iniInfo.FilePath4PI;

		// 读取并编译着色器（shader）, 创建着色器程序，添加并链接着色器
		sp4PI.loadShaders("shader.vert", "shader.frag");

		c4PI.init4PI();
		c4PI.load4PIFirst(filePath + "FS00000000.jpg");
		sp4PI.useProgram();
	} 
	else if (iniInfo.Mode == 2) 
	{
		string filePath = iniInfo.FilePathBox;

		spSkyBox.loadShaders("shader.vert", "shader.frag");

		cSkybox.initSkyBox();
		char pic_path[6][128];
		sprintf(pic_path[0], "FT2\\FT%08d.jpg", 0);
		sprintf(pic_path[1], "BK2\\BK%08d.jpg", 0);
		sprintf(pic_path[2], "LT2\\LT%08d.jpg", 0);
		sprintf(pic_path[3], "RT2\\RT%08d.jpg", 0);
		sprintf(pic_path[4], "TP2\\TP%08d.jpg", 0);
		sprintf(pic_path[5], "BM2\\BM%08d.jpg", 0);

		cSkybox.loadSkyboxFirst(filePath, pic_path[0], pic_path[1], pic_path[2], pic_path[3], pic_path[4], pic_path[5]);
		spSkyBox.useProgram();
	}
}

// ======================================================================================

void renderScene(mat4& mModelViewProjectionMatrix, int frameSequence, bool isLoaded)
{
	if (isLoaded) 
	{
		if (iniInfo.Mode == 1)
		{
			string filePath = iniInfo.FilePath4PI;
			char pic_path[128];
			sprintf(pic_path, "FS%08d.jpg", frameSequence);
			c4PI.load4PI(filePath, pic_path);
		}
		else if (iniInfo.Mode == 2)
		{
			string filePath = iniInfo.FilePathBox;
			char pic_path[6][128];
			sprintf(pic_path[0], "FT2\\FT%08d.jpg", frameSequence);
			sprintf(pic_path[1], "BK2\\BK%08d.jpg", frameSequence);
			sprintf(pic_path[2], "LT2\\LT%08d.jpg", frameSequence);
			sprintf(pic_path[3], "RT2\\RT%08d.jpg", frameSequence);
			sprintf(pic_path[4], "TP2\\TP%08d.jpg", frameSequence);
			sprintf(pic_path[5], "BM2\\BM%08d.jpg", frameSequence);

			cSkybox.loadSkybox(filePath, pic_path[0], pic_path[1], pic_path[2], pic_path[3], pic_path[4], pic_path[5]);
		}
	}

	if (iniInfo.Mode == 1)
	{
		sp4PI.setUniform("gSampler", 0);
		sp4PI.setUniform("mMVP", mModelViewProjectionMatrix);
		c4PI.render4PI();
	}
	else if (iniInfo.Mode == 2)
	{
		spSkyBox.setUniform("gSampler", 0);
		spSkyBox.setUniform("mMVP", mModelViewProjectionMatrix);
		cSkybox.renderSkybox();
	}
}

// ======================================================================================

void releaseScene(void)
{
	//TODO
}