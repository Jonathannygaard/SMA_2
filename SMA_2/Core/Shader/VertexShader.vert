#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 Color;
out vec3 VertexColor;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelMatrix;
void main()
{
    gl_Position = projection*view*modelMatrix*vec4(aPos.x,aPos.y,aPos.z, 1.0);
    VertexColor = Color;
}