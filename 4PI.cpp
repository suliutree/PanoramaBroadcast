#include "4PI.h"

#define BUFFER_OFFSET(offset)	((GLvoid*)offset)
#define picWidth 2000 
#define picHeight 1000
#define radius 500

#define RIGHT_ANGLE 90

GLuint vboBuffers[3];

int dimX, dimY;

void C4PI::init4PI()
{
	glGenVertexArrays(1, &m_uiVAO);

	dimX = picWidth/radius*RIGHT_ANGLE+1;
	dimY = picHeight/radius*RIGHT_ANGLE+1;

	float (*sphereVerts)[3] = new float[dimX*dimY][3];
	float (*textureUV)[2] = new float[dimX*dimY][2];
	int *sphereIndices = new int[dimX*(dimY-1)*2];

	int leftAngle = RIGHT_ANGLE * (picWidth / 2) / (float)radius;
	int topAngle = RIGHT_ANGLE * (picHeight / 2) / (float)radius;

	for (int i = 0; i < dimY; ++i) {
		float y = sinf((topAngle-i)*PI/180.0f);
		float r = cosf((topAngle-i)*PI/180.0f);
		for (int j = 0; j < dimX; ++j) {
			float x = r * sinf( (j-leftAngle)*PI/180.0f );
			float z = - r * cosf( (j-leftAngle)*PI/180.0f );

			sphereVerts[i*dimX + j][0] = x;
			sphereVerts[i*dimX + j][1] = y;
			sphereVerts[i*dimX + j][2] = z;

			textureUV[i*dimX + j][0] = j/(float)(dimX-1);
			textureUV[i*dimX + j][1] = 1 - i/(float)(dimY-1);
		}
	}

	int index=0;
	for (int i=0; i<dimY-1;i++)
	{
		for (int j=0; j<dimX; j++)
		{
			sphereIndices[index++] = j+dimX*(i+1);
			sphereIndices[index++] = j+dimX*i;
		}
	}

	glBindVertexArray(m_uiVAO);

	// 启用顶点数组
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(3, vboBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vboBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, 3*dimX*dimY*sizeof(float), sphereVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)0);


	glBindBuffer(GL_ARRAY_BUFFER, vboBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, 2*dimX*dimY*sizeof(float), textureUV, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)0);

	// GL_ELEMENT_ARRAY_BUFFER（表示索引数据），用索引数据初始化缓冲区对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboBuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dimX*(dimY-1)*2*sizeof(unsigned int), sphereIndices, GL_STATIC_DRAW);

	delete[] sphereVerts;
	delete[] textureUV;
	delete[] sphereIndices;
}

void C4PI::load4PIFirst(string sDirectory)// 先加载一张纹理，之后做纹理替换
{
	m_tTexture.initTexture2D(sDirectory);

	m_tTexture.setFiltering(GL_LINEAR, GL_LINEAR);
	m_tTexture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_tTexture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void C4PI::load4PI(string sDirectory, string sPath)
{
	m_tTexture.loadTexture2D(sDirectory+sPath);

	m_tTexture.setFiltering(GL_LINEAR, GL_LINEAR);
	m_tTexture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_tTexture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void C4PI::render4PI()
{
	glDepthMask(0);
	glBindVertexArray(m_uiVAO);
	m_tTexture.bindTexture();
	glDrawElements(GL_TRIANGLE_STRIP, dimX*(dimY-1)*2, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glDepthMask(1);
}

C4PI::~C4PI()
{
	glDeleteVertexArrays(1, &m_uiVAO);
}