#version 330 core

uniform mat4 mvp;

in vec3 POSITION;
in vec2 TEXCOORD;
in vec4 COLOR;

out vec2 passTexcoord;
out vec4 passColor;

void main()
{
  passTexcoord = TEXCOORD;
  passColor = COLOR;
  gl_Position = mvp * vec4(POSITION, 1.0);
}
