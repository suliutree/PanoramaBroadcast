#pragma once

#include "common_header.h"

class CTexture {
public:
	CTexture();
	~CTexture();

	
	void createTexStorage2D(GLubyte* bData, 
		GLenum format, int iWidth, int iHeight, bool bGenerateMipMaps);//Ϊ���������滻�����
	bool initTexture2D(std::string sPath, bool bGenerateMipMaps = false);//
	void createFromData(GLubyte* bData, 
		int iWidth, int iHeight, int iBPP, GLenum format, bool bGenerateMipMaps = false);
	bool loadTexture2D(std::string sPath, bool bGenerateMipMaps = false);
	void bindTexture(int iTextureUnit = 0);

	void setFiltering(GLint tfMagnification, GLint tfMinification);

	void setSamplerParameter(GLenum parameter, GLenum value);

private:
	GLuint m_uiTexture;
	GLuint m_uiSampler;
	bool m_bMipMapsGenerated;

	string m_sTexturePath;
};

