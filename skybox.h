#pragma once

#include "texture.h"
#include "vertexBufferObject.h"

class CSkybox
{
public:
	void initSkyBox();
	// 生成一次纹理，然后进行纹理替换
	void loadSkyboxFirst(string sDirectory, 
		string sFront, string sBack, string sLeft, string sRight, string sTop, string sBottom);
	void loadSkybox(string sDirectory, 
		string sFront, string sBack, string sLeft, string sRight, string sTop, string sBottom);
	void renderSkybox();

	~CSkybox();

private:
	GLuint m_uiVAO;
	CVertexBufferObject m_vboRenderData;
	CTexture m_tTextures[6];
};