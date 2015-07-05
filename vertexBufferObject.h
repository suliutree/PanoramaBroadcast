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
	GLuint m_uiBuffer;			// 缓存对象的名称
	GLenum m_iBufferType;		// 缓存结合点（如GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER）
	vector<GLubyte> m_data;		// 缓存中存放的数据
};