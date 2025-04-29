#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;
uniform mat4 u_ViewProjection;
uniform mat4 u_ModelMatrix;
out vec2 Texcoord;
void main()
{
	gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position, 1.0);
	Texcoord = a_Texcoord;
}

#type fragment
#version 330 core

in vec2 Texcoord;

uniform sampler2D u_Texture;
out vec4 outColor;
void main()
{
	outColor = texture(u_Texture, Texcoord);
}