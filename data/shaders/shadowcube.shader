VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

	out vec2 textureCoordinate;
	out vec3 fragPos;

	void main() 
	{
	    textureCoordinate = vertexTextureCoordinate;

	    vec4 pos = modelMatrix * vec4(vertexPosition, 1); 
	    fragPos = vec3(pos);
	    gl_Position = cameraMatrix * pos;
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D texturemap;
	uniform samplerCube shadowMap;

	out vec4 finalColor;

	in vec2 textureCoordinate;

	in vec3 fragPos;

	#include "cubeshadow.fragment"

	void main() 
	{
		float shadowValue = 0.5 + 0.5 * (1.0 - shadowFunc(fragPos));
		vec4 color = texture(texturemap, textureCoordinate) * shadowValue;
		color.a = 1.0;

		finalColor = color;
	}	
}