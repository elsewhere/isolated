VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	uniform mat4 lightMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

	void main() {
	    // Apply all matrix transformations to vert

	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	}
}

FRAGMENT_SHADER
{
	#version 330

	out vec4 finalColor;

	uniform vec4 color;

	void main() 
	{
		vec3 col = color.xyz;
		finalColor = vec4(col, 1.0);
	}	
}