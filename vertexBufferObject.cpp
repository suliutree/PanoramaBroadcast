
#include "vertexBufferObject.h"

void CVertexBufferObject::createVBO(GLenum iBufferType)
{
	glGenBuffers(1, &m_uiBuffer);
	glBindBuffer(iBufferType, m_uiBuffer);
	m_iBufferType = iBufferType;
}

CVertexBufferObject::~CVertexBufferObject()
{
	glDeleteBuffers(1, &m_uiBuffer);
	m_data.clear();
}

/*
* �����Դ�
* iUsageHint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...
*/
void CVertexBufferObject::uploadDataToGPU(int iDrawingHint)
{
	glBufferData(m_iBufferType, m_data.size(), &m_data[0], iDrawingHint);
	m_data.clear();
}

/* ��VBO��������������� */
void CVertexBufferObject::addData(void* ptrData, GLuint uiDataSize)
{
	// ��m_data.end()ǰ��������[ptrData, ptrData+uiDataSize]������Ԫ��
	m_data.insert(m_data.end(), (GLubyte*)ptrData, (GLubyte*)ptrData+uiDataSize);
}