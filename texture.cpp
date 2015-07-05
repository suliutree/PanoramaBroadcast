
#include "texture.h"

#include <time.h>

CTexture::CTexture()
{
	m_bMipMapsGenerated = false;
}

int index = 0;

void CTexture::createTexStorage2D(GLubyte* bData, GLenum format, int iWidth, int iHeight, bool bGenerateMipMaps)
{
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);

	if(format == GL_RGB || format == GL_BGR) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
		if(bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, format, iWidth, iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
		if(bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	}

	glGenSamplers(1, &m_uiSampler);

	m_sTexturePath = "";
	m_bMipMapsGenerated = bGenerateMipMaps;
}

bool CTexture::initTexture2D(std::string sTexturePath, bool bGenerateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(sTexturePath.c_str(), 0); // ����ļ�ǩ�����Ƶ����ʽ
	if(fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(sTexturePath.c_str()); 	// ����չ���²��ʽ
	if(fif == FIF_UNKNOWN) return false;

	if(FreeImage_FIFSupportsReading(fif)) dib = FreeImage_Load(fif, sTexturePath.c_str());
	if(!dib) return false;


	GLubyte* bDataPointer = FreeImage_GetBits(dib);
	if(bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
		return false;

	GLenum format = FreeImage_GetBPP(dib) == 24 ? GL_BGR : FreeImage_GetBPP(dib) == 8 ? GL_LUMINANCE : 0;

	createTexStorage2D(bDataPointer, format, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), bGenerateMipMaps);

	FreeImage_Unload(dib);

	m_sTexturePath = sTexturePath;
	return true;
}

void CTexture::createFromData(GLubyte* bData, int iWidth, int iHeight, int iBPP, GLenum format, bool bGenerateMipMaps)
{
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	if(format == GL_RGB || format == GL_BGR) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, iWidth, iHeight, GL_BGR, GL_UNSIGNED_BYTE, bData);
		if(bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, iWidth, iHeight, format, GL_UNSIGNED_BYTE, bData);
		if(bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	}

	glGenSamplers(1, &m_uiSampler);

	m_sTexturePath = "";
	m_bMipMapsGenerated = bGenerateMipMaps;
}

bool CTexture::loadTexture2D(std::string sTexturePath, bool bGenerateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(sTexturePath.c_str(), 0); // ����ļ�ǩ�����Ƶ����ʽ
	if(fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(sTexturePath.c_str()); 	// ����չ���²��ʽ
	if(fif == FIF_UNKNOWN) return false;

	//clock_t begin = clock();
	if(FreeImage_FIFSupportsReading(fif)) dib = FreeImage_Load(fif, sTexturePath.c_str());
	if(!dib) return false;
	//clock_t end = clock();
	//cout << end - begin << endl;
		
	GLubyte* bDataPointer = FreeImage_GetBits(dib);
	if(bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
		return false;

	GLenum format = FreeImage_GetBPP(dib) == 24 ? GL_BGR : FreeImage_GetBPP(dib) == 8 ? GL_LUMINANCE : 0;
	createFromData(bDataPointer, 
		FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, bGenerateMipMaps);

	FreeImage_Unload(dib);

	m_sTexturePath = sTexturePath;
	return true;
}

void CTexture::setSamplerParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(m_uiSampler, parameter, value);
}

void CTexture::setFiltering(int tfMagnification, int tfMinification)
{
	glSamplerParameteri(m_uiSampler, GL_TEXTURE_MAG_FILTER, tfMagnification); // ���÷Ŵ��˲���
	glSamplerParameteri(m_uiSampler, GL_TEXTURE_MIN_FILTER, tfMinification); // ������С�˲���
}

void CTexture::bindTexture(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	glBindSampler(iTextureUnit, m_uiSampler);
}


CTexture::~CTexture()
{
	glDeleteSamplers(1, &m_uiSampler);
	glDeleteTextures(1, &m_uiTexture);
}