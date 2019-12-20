#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 binormal;

out DATA
{
	vec2 texCoord;
} vs_out;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

uniform float textureTiling = 1.0;

void main()
{
	vs_out.texCoord = texCoords / textureTiling;
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;

in DATA
{
	vec2 texCoord;
} fs_in;

uniform sampler2D colorTex;

void main()
{
	fragColor = texture(colorTex, fs_in.texCoord);
}