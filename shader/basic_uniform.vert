#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;


//out vec3 LightIntensity;
out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform struct viewInfo {
    mat4 ModelViewMatrix;
    mat3 NormalMatrix;
    mat4 MVP;
    vec3 View; // added to know the camera's position
} itemView;

void main()
{
    TexCoord = VertexTexCoord;
    Normal = normalize( itemView.NormalMatrix * VertexNormal);
    Position = ( itemView.ModelViewMatrix * vec4(VertexPosition,1.0) ).xyz;
    gl_Position = itemView.MVP * vec4(VertexPosition,1.0);
}