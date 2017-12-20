#version 410 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord;

out vec2 fragTexCoord;

uniform mat3 viewProj;

void main() {
  gl_Position.xy = (viewProj * vec3(pos, 1.0)).xy;
  gl_Position.zw = vec2(0.0, 1.0);
  fragTexCoord = texCoord;
}
