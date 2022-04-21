#version 300 es
precision highp float;
in vec2 v_texCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D s_texture_map_;
uniform vec2 u_texSize;
void main() { outColor = texture(s_texture_map_, v_texCoord); }