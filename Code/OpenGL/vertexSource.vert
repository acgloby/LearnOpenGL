#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // (乘MVP矩阵)注意乘法要从右向左读
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vertexColor = vec4(aColor.x,aColor.y,aColor.z,1.0f);
	TexCoord = aTexCoord;
}