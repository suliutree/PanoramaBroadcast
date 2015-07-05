#pragma once

#include "common_header.h"

/* VBO: vertex buffer object */
class CVertexBufferObject {
public:
	void createVBO(GLenum iBufferType = GL_ARRAY_BUFFER);
	~CVertexBufferObject();

	void uploadDataToGPU(int iUsageHint);
	
	void addData(void* ptrData, GLuint uiDataSize);

private:
	GLuint m_uiBuffer;			// ������������
	GLenum m_iBufferType;		// �����ϵ㣨��GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER��
	vector<GLubyte> m_data;		// �����д�ŵ�����
};