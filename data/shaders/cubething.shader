VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	out vec3 fragPos;

	void main() 
	{
	    vec4 pos = modelMatrix * vec4(vertexPosition, 1.0);
	    fragPos = vec3(pos);
	    gl_Position = cameraMatrix * pos;
	}
}

FRAGMENT_SHADER
{
	#version 330

	out vec4 finalColor;
	in vec3 fragPos;

	uniform vec4 color;

	#include "cubeshadow.fragment"

	void main() 
	{
		vec3 col = color.xyz * (1.0 - 0.5 * shadowFunc(fragPos));
		finalColor = vec4(col, 1.0);
	}	
}