#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelMatrix;
out vec4 v_Color;
void main()
{
	v_Color = a_Color;
	gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

in vec4 v_Color;
layout(location = 0) out vec4 outColor;
void main()
{
	outColor = v_Color;
}