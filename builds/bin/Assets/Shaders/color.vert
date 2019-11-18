#version 330

in vec4 color0;
in vec3 normal0;
in vec3 position0;
in vec2 texcoord0;

uniform mat4 mvp;

out vec4 passColor0;
out vec2 passTexcoord0;

void main()
{
  passColor0 = color0;
  passTexcoord0 = texcoord0;
  gl_Position = mvp * vec4(position0, 1);
}
