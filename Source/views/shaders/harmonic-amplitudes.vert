#version 330 core

in vec4 position;
in vec4 sourceColor;

out vec4 fragColor;

void main() {
  gl_Position = position;

  fragColor = sourceColor;
}
