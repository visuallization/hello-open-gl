#version 330

layout (std140) uniform Matrices {
	mat4 pvm;
};

in vec4 position;

out vec4 color;

void main()
{
	color = position;
	gl_Position = pvm * position;
} 
