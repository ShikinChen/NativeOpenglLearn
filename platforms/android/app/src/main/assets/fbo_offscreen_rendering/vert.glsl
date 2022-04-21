#version 300 es                          
layout(location = 0) in vec4 v_Position;
layout(location = 1) in vec2 f_Position;
out vec2 v_texCoord;
uniform mat4 u_matrix;
void main()
{
    gl_Position = v_Position*u_matrix;
    v_texCoord = f_Position;
}