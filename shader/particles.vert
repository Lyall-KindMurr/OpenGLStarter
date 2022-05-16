#version 460

layout (location = 0) in vec3 VertexPosition;

uniform struct viewInfo {
    mat4 ModelViewMatrix;
    mat3 NormalMatrix;
    mat4 MVP;
    vec3 View; // added to know the camera's position
} itemView;

void main()
{
    gl_Position = itemView.ModelViewMatrix * vec4(VertexPosition,1.0);
}