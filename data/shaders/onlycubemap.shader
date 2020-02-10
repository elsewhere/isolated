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
	    
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform samplerCube cubeMap;

	in vec3 fragmentPosition;
	in vec3 fragmentNormal;

	out vec4 finalColor;

	void main() 
	{
		vec4 reflectionColor = texture(cubeMap, normalize(fragmentPosition));//eflection);
	    finalColor = vec4(reflectionColor.xyz, 1.0);
	}	
}
