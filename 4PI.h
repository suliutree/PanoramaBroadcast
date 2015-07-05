#pragma once

#include "texture.h"

class C4PI
{
public:
	void init4PI();
	void load4PIFirst(string sDirectory);
	void load4PI(string sDirectory, string sPath);
	void render4PI();

	~C4PI();

private:
	GLuint m_uiVAO;
	//CVertexBufferObject m_vboRenderData;
	CTexture m_tTexture;
};