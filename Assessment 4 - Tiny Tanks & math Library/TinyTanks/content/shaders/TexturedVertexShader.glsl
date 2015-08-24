#version 400

in vec4 colour;
in vec2 UV;

out vec4 outColour;

uniform sampler2D diffuse;

void main()
{
	outColour = texture(diffuse, UV).rgba * colour;
}