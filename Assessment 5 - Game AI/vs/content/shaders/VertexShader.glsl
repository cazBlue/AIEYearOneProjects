#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 vertexUV;

uniform mat4 MVP;

smooth out vec4 vertColour;
out vec2 UV;

void main()
{
	vertColour = colour;	
    UV = vertexUV;
	//vertColour = vec4(UV.x, UV.y, 0, 1);

    vec4 scaledPosition = MVP * position;

	gl_Position = scaledPosition;
}