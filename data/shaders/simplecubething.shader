VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;

	void main() 
	{
	    vec4 pos = modelMatrix * vec4(vertexPosition, 1.0);
	    gl_Position = cameraMatrix * pos;
	}
}

FRAGMENT_SHADER
{
	#version 330

	out vec4 finalColor;

	uniform vec4 color;

	void main() 
	{
		finalColor = vec4(color.xyz, 1.0);
	}	
}