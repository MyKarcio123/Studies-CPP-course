#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 viewMatrix;

void main()
{
	// Pozycja w przestrzeni œwiatowej
	vec4 worldPosition = viewMatrix * vec4(position, 1.0);

	// Normalna w przestrzeni œwiatowej
	vec3 worldNormal = normalize(mat3(transpose(inverse(viewMatrix))) * normal);

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * worldPosition;
}