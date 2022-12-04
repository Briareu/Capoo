#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 mv_matrix;
uniform samplerCube skybox;

void main()
{
    color = texture(skybox, TexCoords);
}