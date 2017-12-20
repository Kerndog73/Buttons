#version 410 core

in vec2 fragTexCoord;

uniform sampler2D tex;

out vec4 color;

void main() {
  color = texture(tex, fragTexCoord);
  color = vec4(1.0, 0.0, 0.0, 1.0);
  if (color.a == 0.0) {
    gl_FragDepth = 1.0;
  } else {
    gl_FragDepth = gl_FragCoord.z;
  }
}
