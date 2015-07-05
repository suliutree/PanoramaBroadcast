
#include "skybox.h"

void CSkybox::initSkyBox()
{
	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);

	m_vboRenderData.createVBO();

	vec3 vSkyBoxVertices[24] = { // 顶点位置
		vec3(100.0f, 100.0f, -100.0f), vec3(100.0f, -100.0f, -100.0f), vec3(-100.0f, 100.0f, -100.0f), vec3(-100.0f, -100.0f, -100.0f),

		vec3(100.0f, -100.0f, 100.0f), vec3(100.0f, 100.0f, 100.0f), vec3(-100.0f, -100.0f, 100.0f), vec3(-100.0f, 100.0f, 100.0f),

		vec3(100.0f, 100.0f, 100.0f), vec3(100.0f, -100.0f, 100.0f), vec3(100.0f, 100.0f, -100.0f), vec3(100.0f, -100.0f, -100.0f),

		vec3(-100.0f, 100.0f, -100.0f), vec3(-100.0f, -100.0f, -100.0f), vec3(-100.0f, 100.0f, 100.0f), vec3(-100.0f, -100.0f, 100.0f),

		vec3(100.0f, 100.0f, 100.0f),  vec3(100.0f, 100.0f, -100.0f), vec3(-100.0f, 100.0f, 100.0f), vec3(-100.0f, 100.0f, -100.0f),

		vec3(100.0f, -100.0f, -100.0f),  vec3(100.0f, -100.0f, 100.0f), vec3(-100.0f, -100.0f, -100.0f), vec3(-100.0f, -100.0f, 100.0f)
	};
	vec2 vSkyBoxTexCoords[4] = { // 纹理坐标
		vec2(0.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), vec2(1.0f, 0.0f)
	};

	for(int i = 0; i < 24; ++i) {
		m_vboRenderData.addData(&vSkyBoxVertices[i], sizeof(vec3));
		m_vboRenderData.addData(&vSkyBoxTexCoords[i%4], sizeof(vec2));
	}

	m_vboRenderData.uploadDataToGPU(GL_STATIC_DRAW); // 载入显存

	// 顶点位置属性设置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3)+sizeof(vec2), 0);
	// 纹理坐标属性设置
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec3)+sizeof(vec2), (void*)sizeof(vec3));
}


void CSkybox::loadSkyboxFirst(string sDirectory, 
	string sFront, string sBack, string sLeft, string sRight, string sTop, string sBottom)
{
	m_tTextures[0].initTexture2D(sDirectory+sFront);
	m_tTextures[1].initTexture2D(sDirectory+sBack);
	m_tTextures[2].initTexture2D(sDirectory+sLeft);
	m_tTextures[3].initTexture2D(sDirectory+sRight);
	m_tTextures[4].initTexture2D(sDirectory+sTop);
	m_tTextures[5].initTexture2D(sDirectory+sBottom);

	for (int i = 0; i < 6; ++i) {
		m_tTextures[i].setFiltering(GL_LINEAR, GL_LINEAR);
		m_tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

}


void CSkybox::loadSkybox(string sDirectory, 
	string sFront, string sBack, string sLeft, string sRight, string sTop, string sBottom)
{
	m_tTextures[0].loadTexture2D(sDirectory+sFront);
	m_tTextures[1].loadTexture2D(sDirectory+sBack);
	m_tTextures[2].loadTexture2D(sDirectory+sLeft);
	m_tTextures[3].loadTexture2D(sDirectory+sRight);
	m_tTextures[4].loadTexture2D(sDirectory+sTop);
	m_tTextures[5].loadTexture2D(sDirectory+sBottom);

	for (int i = 0; i < 6; ++i) {
		m_tTextures[i].setFiltering(GL_LINEAR, GL_LINEAR);
		m_tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

void CSkybox::renderSkybox()
{
	glDepthMask(0);
	glBindVertexArray(m_uiVAO);
	for (int i = 0; i < 6; ++i) {
		m_tTextures[i].bindTexture();
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}
	glDepthMask(1);
}

CSkybox::~CSkybox()
{
	glDeleteVertexArrays(1, &m_uiVAO);
}