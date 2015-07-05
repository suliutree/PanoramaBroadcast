#pragma once

#include "texture.h"
#include "vertexBufferObject.h"

class CSkybox
{
public:
	void initSkyBox();
	// ����һ������Ȼ����������滻
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