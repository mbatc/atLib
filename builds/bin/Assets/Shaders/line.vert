#version 330

in vec4 COLOR;
in vec3 POSITION;

uniform mat4 mvp;

out vec4 passColor0;

void main()
{
  passColor0 = COLOR;
  gl_Position = mvp * vec4(POSITION, 1);
}
