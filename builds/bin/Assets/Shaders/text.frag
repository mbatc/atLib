#version 330 core

uniform sampler2D tex0;

out vec4 fragColor;

in vec2 passTexcoord;
in vec4 passColor;

void main()
{
  fragColor = passColor * textureLod(tex0, passTexcoord, 0);
}
