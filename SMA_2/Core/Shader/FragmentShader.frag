#version 330 core
out vec3 FragColor;
in vec3 VertexColor;
void main()
{
    FragColor = VertexColor;
}