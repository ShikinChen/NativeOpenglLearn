#version 300 es
precision mediump float;
in vec2 v_texCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D y_Texture;
uniform sampler2D uv_Texture;
void main() {
  vec3 yuv;
  yuv.x = texture(y_Texture, v_texCoord).r;
  yuv.y = texture(uv_Texture, v_texCoord).a - 0.5;
  yuv.z = texture(uv_Texture, v_texCoord).r - 0.5;
  highp vec3 rgb = mat3(1, 1, 1, 0, -0.344, 1.770, 1.403, -0.714, 0) * yuv;
  outColor = vec4(rgb, 1);
}