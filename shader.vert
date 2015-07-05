#version 330

uniform mat4 mMVP;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;

out vec2 texCoord;

void main()
{
	gl_Position = mMVP*vec4(inPosition, 1.0);
	texCoord = inCoord;
}