VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec3 vertexNormal;

	out vec3 fragmentNormal;
	out vec3 fragmentPosition;

	void main() 
	{
		fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
		fragmentPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
	    
	    // Apply all matrix transformations to vert
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform samplerCube tex;
	uniform vec3 cameraPosition;

	in vec3 fragmentPosition;
	in vec3 fragmentNormal;

	out vec4 finalColor;

	void main() 
	{
		vec3 eye = normalize(fragmentPosition - cameraPosition);
		vec3 reflection = refract(eye, fragmentNormal, 0.5);

		vec4 col = texture(tex, reflection);
	    finalColor = col;
	}	
}
