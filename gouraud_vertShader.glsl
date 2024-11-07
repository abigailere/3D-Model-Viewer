#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
uniform mat4 transform;
uniform mat4 MP;

out vec3 ourColor;
out vec3 Normal;
out vec3 FragPos;

uniform bool gouraud;
void main()
{
	FragPos = vec3(transform * vec4(aPos.x, aPos.y, aPos.z, 1.0));
	gl_Position =  transform * vec4(FragPos, 1.0);
	
	Normal = mat3(transpose(inverse(transform))) * aNormal;
	//Normal = aNormal;
	ourColor = aColor;
	
};