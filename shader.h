#pragma once

#include "common_header.h"

/* ������ɫ��(shader)�ļ������룻������ɫ���������� */
class CShaderProgram {
public:
	void loadShaders(const char* vertexFilePath,const char* fragmentFilePath);
	void useProgram();
	GLuint getProgramID();
	~CShaderProgram();

	/* ������ɫ��(shader)�е�ͳһ����(uniform) */
	// ����ʸ��
	void setUniform(string sUniformName, vec2* vVectors, int iCount = 1);
	void setUniform(string sUniformName, const vec2 vVector);
	void setUniform(string sUniformName, vec3* vVectors, int iCount = 1);
	void setUniform(string sUniformName, const vec3 vVector);
	void setUniform(string sUniformName, vec4* vVectors, int iCount = 1);
	void setUniform(string sUniformName, const vec4 vVector);
	// ���ø�����
	void setUniform(string sUniformName, float* fValues, int iCount = 1);
	void setUniform(string sUniformName, const float fValue);
	// ���� 4x4 ����
	void setUniform(string sUniformName, mat4* mMatrices, int iCount = 1);
	void setUniform(string sUniformName, const mat4 mMatrix);
	// ��������
	void setUniform(string sUniformName, int* iValues, int iCount = 1);
	void setUniform(string sUniformName, const int iValue);

private:
	GLuint m_uiProgramID; // ��ɫ�������ID
};