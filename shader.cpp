
#include "shader.h"
GLint iStatus;
GLint InfoLogLength;

/* ������ɫ��(shader)�ļ������룻������ɫ���������� */
void CShaderProgram::loadShaders(const char* vertexFilePath,const char* fragmentFilePath)
{
	// ������ɫ��
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// ��ȡ���㣨Vertex����ɫ���ļ�
	string VertexShaderCode;
	ifstream VertexShaderStream(vertexFilePath, ios::in);
	if (VertexShaderStream.is_open()) {
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	} else {
		fprintf(stderr, "Can not open vertex shader file: %s \n", vertexFilePath);
		exit(1);
	}

	// ��ȡƬ�Σ�Fragment����ɫ���ļ�
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragmentFilePath, ios::in);
	if (FragmentShaderStream.is_open()) {
		string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	} else {
		fprintf(stderr, "Can not open fragment shader file: %s \n", fragmentFilePath);
		exit(1);
	}

	// ���붥����ɫ��
	printf("Compiling Vertex Shader: %s\n", vertexFilePath);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// ��鶥����ɫ���������
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &iStatus);
	if (!iStatus) {
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stderr, "Vertex Shader compiling error: %s\n", &VertexShaderErrorMessage[0]);
		exit(1);
	}

	// ����Ƭ����ɫ��
	printf("Compiling Fragment Shader: %s\n", fragmentFilePath);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// ���Ƭ����ɫ���������
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &iStatus);
	if (!iStatus){
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stderr, "Fragment Shader compiling error: %s\n", &FragmentShaderErrorMessage[0]);
		exit(1);
	}

	// ������������ɫ������
	printf("Linking shader program\n");
	m_uiProgramID = glCreateProgram();
	glAttachShader(m_uiProgramID, VertexShaderID);
	glAttachShader(m_uiProgramID, FragmentShaderID);
	glLinkProgram(m_uiProgramID);

	// �����ɫ����������״��
	glGetProgramiv(m_uiProgramID, GL_LINK_STATUS, &iStatus);
	if (!iStatus){
		glGetProgramiv(m_uiProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(m_uiProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stderr, "shader program linking error: %s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

GLuint CShaderProgram::getProgramID()
{
	return m_uiProgramID;
}

void CShaderProgram::useProgram()
{
	glValidateProgram(m_uiProgramID);
    glGetProgramiv(m_uiProgramID, GL_VALIDATE_STATUS, &iStatus);
    if (!iStatus) {
		glGetProgramiv(m_uiProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(m_uiProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stderr, "Invalid shader program: '%s'\n", &ProgramErrorMessage[0]);
        exit(1);
    }
	glUseProgram(m_uiProgramID);
}

CShaderProgram::~CShaderProgram()
{
	glDeleteProgram(m_uiProgramID);
}

/* ������ɫ��(shader)�е�ͳһ����(uniform) */
// ����ʸ��
void CShaderProgram::setUniform(string sUniformName, glm::vec2* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
}

void CShaderProgram::setUniform(string sUniformName, const glm::vec2 vVector)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}

void CShaderProgram::setUniform(string sUniformName, vec3* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}

void CShaderProgram::setUniform(string sUniformName, const vec3 vVector)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}

void CShaderProgram::setUniform(string sUniformName, glm::vec4* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform4fv(iLoc, iCount, (GLfloat*)vVectors);
}

void CShaderProgram::setUniform(string sUniformName, const glm::vec4 vVector)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform4fv(iLoc, 1, (GLfloat*)&vVector);
}

// ���ø�����
void CShaderProgram::setUniform(string sUniformName, float* fValues, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform1fv(iLoc, iCount, fValues);
}

void CShaderProgram::setUniform(string sUniformName, const float fValue)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform1fv(iLoc, 1, &fValue);
}

// ���� 4x4 ����
void CShaderProgram::setUniform(string sUniformName, glm::mat4* mMatrices, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniformMatrix4fv(iLoc, iCount, false, (GLfloat*)mMatrices);
}

void CShaderProgram::setUniform(string sUniformName, const glm::mat4 mMatrix)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniformMatrix4fv(iLoc, 1, false, (GLfloat*)&mMatrix);
}

// ��������
void CShaderProgram::setUniform(string sUniformName, int* iValues, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform1iv(iLoc, iCount, iValues);
}

void CShaderProgram::setUniform(string sUniformName, const int iValue)
{
	int iLoc = glGetUniformLocation(m_uiProgramID, sUniformName.c_str());
	glUniform1i(iLoc, iValue);
}