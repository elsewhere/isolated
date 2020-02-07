VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;
	uniform mat4 lightMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

	out vec2 textureCoordinate;
	out vec4 lightFragPos;

	void main() 
	{
	    textureCoordinate = vertexTextureCoordinate;

	    vec4 pos = modelMatrix * vec4(vertexPosition, 1);
	    lightFragPos = lightMatrix * vec4(vertexPosition, 1.0);
	    gl_Position = cameraMatrix * pos;
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D texturemap;

	out vec4 finalColor;

	in vec2 textureCoordinate;

#include "shadowmap.fragment"

	void main() 
	{
		float shadowValue = 0.4 + 0.6 * (shadowFunc(lightFragPos));
		vec4 color  = texture(texturemap, textureCoordinate) * shadowValue;
		color.a = 1.0;

		finalColor = color;
	}	
}