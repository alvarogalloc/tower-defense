#version 330 core

out vec4 FragColor;

uniform float time;

void main() {
  // black to white and back
  float col =(sin(time)/100)+0.1;
  FragColor = vec4(col, col, col, 1.0);
}
