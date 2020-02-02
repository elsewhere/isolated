VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;
	uniform mat4 lightMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

	out vec2 textureCoordinate;
	out vec4 lightFragPos;

	void main() 
	{
	    textureCoordinate = vertexTextureCoordinate;

	    vec4 pos = modelMatrix * vec4(vertexPosition, 1);
	    lightFragPos = lightMatrix * vec4(vertexPosition, 1.0);
	    gl_Position = cameraMatrix * pos;
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D texturemap;

	out vec4 finalColor;

	in vec2 textureCoordinate;
	in vec4 lightFragPos;

	uniform sampler2DShadow shadowMap;

	float shadowFunc(vec4 pos)
	{
	    vec3 projCoords = lightFragPos.xyz / lightFragPos.w;
	    projCoords = projCoords * 0.5 + 0.5; //[0, 1]

	    vec2 size = 1.0 / textureSize(shadowMap, 0);
	    float shadow = 0.0;

	    float bias = 0.003;
	    for (int y = -1; y < 1; y++)
	    {
	    	for (int x = -1; x < 1; x++)
	    	{
	    		vec2 offs = vec2(x * size.x, y * size.y);
	    		vec3 coord = vec3(projCoords.xy + offs, projCoords.z + bias);
	    		shadow += texture(shadowMap, coord);
	    	}
	    }
	    return shadow / 9.0;
	}


	void main() 
	{
		float shadowValue = 0.4 + 0.6 * (shadowFunc(lightFragPos));
		vec4 color  = texture(texturemap, textureCoordinate) * shadowValue;
		color.a = 1.0;

		finalColor = color;
	}	
}