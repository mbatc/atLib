#version 330

in vec4 COLOR;
in vec3 NORMAL;
in vec3 POSITION;
in vec2 TEXCOORD;

uniform mat4 mvp;

out vec4 passColor0;
out vec2 passTexcoord0;

void main()
{
  passColor0 = COLOR;
  passTexcoord0 = TEXCOORD;
  gl_Position = mvp * vec4(POSITION, 1);
}
