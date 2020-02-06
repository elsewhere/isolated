VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;
	uniform mat4 lightMatrix;

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
	uniform vec3 lightPos;
	uniform float farplane;

	out vec4 finalColor;

	in vec2 textureCoordinate;
	in vec3 fragPos;

	float shadowFunc(vec3 pos)
	{
	   // get vector between fragment position and light position
	    vec3 fragToLight = pos - lightPos;
	    // use the light to fragment vector to sample from the depth map    
	    float closestDepth = texture(shadowMap, fragToLight).r;
	    // it is currently in linear range between [0,1]. Re-transform back to original value
	    closestDepth *= farplane;
	    // now get current linear depth as the length between the fragment and light position
	    float currentDepth = length(fragToLight);
	    // now test for shadows
	    float bias = 0.05; 
	    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

	    return shadow;
	}


	void main() 
	{
//		vec4 value = texture(shadowMap, lightPos - fragPos);
//		vec4 color = vec4(value.xxx, 1.0);
		float shadowValue = 0.4 + 0.6 * (1.0 - shadowFunc(fragPos));
		vec4 color = texture(texturemap, textureCoordinate) * shadowValue;
		color.a = 1.0;

		finalColor = color;
	}	
}