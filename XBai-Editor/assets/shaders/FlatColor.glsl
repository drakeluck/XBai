#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;
out vec2 Texcoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelMatrix;

void main()
{
	Texcoord = a_Texcoord;
	gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

in vec2 Texcoord;

uniform vec4 u_Color;
uniform float u_TillingFactor;
uniform sampler2D u_Texture;

layout(location = 0) out vec4 outColor;
void main()
{
	outColor = (texture(u_Texture, Texcoord * u_TillingFactor) + vec4(0.7, 0.7, 0.7, 0.0))* u_Color;
}