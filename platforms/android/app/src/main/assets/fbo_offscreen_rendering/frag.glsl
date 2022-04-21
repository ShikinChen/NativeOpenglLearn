#version 300 es                              
precision mediump float;
in vec2 v_texCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D s_texture_map_;
void main()
{
    outColor = texture(s_texture_map_, v_texCoord);
}