VERTEX_SHADER
{

	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;

	out vec3 textureCoordinate;

	void main() {
	    // Pass the tex coord straight through to the fragment shader
	    textureCoordinate = normalize(vertexPosition);
	    
	    // Apply all matrix transformations to vert
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform samplerCube tex;
	uniform float brightness;
	out vec4 finalColor;
	in vec3 textureCoordinate;

	void main() 
	{
	    finalColor = texture(tex, textureCoordinate) * brightness;
	}
}