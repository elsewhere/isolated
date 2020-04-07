VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;

	void main() {
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	}
}

FRAGMENT_SHADER
{
	#version 330

	void main() 
	{
	}	
}