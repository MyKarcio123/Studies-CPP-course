#version 140 compatibility

attribute vec3 position;
attribute vec2 uv;

uniform mat4 viewMatrix;

out vec2 TexCoord;

void main()
{
    // Pozycja w przestrzeni œwiatowej
    vec4 worldPosition = viewMatrix * vec4(position, 1.0);

    // Normalna w przestrzeni œwiatowej
    // vec3 worldNormal = normalize(mat3(transpose(inverse(viewMatrix))) * normal);

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * worldPosition;
    
    // Przekazanie wspó³rzêdnych tekstury do fragment shadera
    TexCoord = uv;
}