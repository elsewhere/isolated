VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec3 vertexNormal;
	in vec2 vertexTextureCoordinate;

	out vec3 fragmentNormal;
	out vec3 fragmentPosition;
	out vec2 fragmentTextureCoordinate;

	void main() 
	{
		fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
		fragmentPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
	    
	    fragmentTextureCoordinate = vertexTextureCoordinate;
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D textureMap;
	uniform samplerCube cubeMap;

	uniform vec3 cameraPosition;

	in vec3 fragmentPosition;
	in vec3 fragmentNormal;
	in vec2 fragmentTextureCoordinate;

	out vec4 finalColor;

	void main() 
	{
//		vec3 eye = normalize(fragmentPosition - cameraPosition);
//		vec3 reflection = refract(eye, fragmentNormal, 0.5);

		vec4 reflectionColor = texture(cubeMap, normalize(fragmentPosition));//eflection);
		vec4 baseColor = vec4(0.5, 0.5, 1.0, 1.0);//texture(textureMap, fragmentTextureCoordinate);

		vec3 col = mix(baseColor.xyz, reflectionColor.xyz, 0.9);

	    finalColor = vec4(col, 1.0);
	}	
}
