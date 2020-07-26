#version 330

uniform sampler2D texture0;

uniform vec4 diffuseColour0;

in vec4 passColor0;
in vec2 passTexcoord0;

out vec4 fragColor;

void main()
{
  fragColor = texture(texture0, passTexcoord0) * passColor0 * diffuseColour0;
}
