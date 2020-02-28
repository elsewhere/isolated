VERTEX_SHADER
{
	#version 330

	uniform mat4 camera;
	uniform mat4 model;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

	void main() {
	    // Apply all matrix transformations to vert
	    gl_Position = camera * model * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	out vec4 finalColor;

	uniform vec3 color;

	void main() 
	{
		finalColor = vec4(color, 1.0);

	}	
}