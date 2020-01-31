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

	uniform sampler2D shadowMap;

	float shadowFunc(vec4 pos)
	{
	    vec3 projCoords = lightFragPos.xyz / lightFragPos.w;

	    projCoords = projCoords * 0.5 + 0.5; //[0, 1]
	    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	    float closestDepth = texture(shadowMap, projCoords.xy).r; 
	    // get depth of current fragment from light's perspective
	    float currentDepth = projCoords.z;
	    // check whether current frag pos is in shadow

	    float bias = 0.003;
	    float shadow = ((currentDepth - bias) > closestDepth)  ? 1.0 : 0.0;

	    //fix parts outside the frustum
		if(projCoords.z > 1.0)
		    shadow = 0.0;

	    return shadow;
	}


	void main() 
	{
		float shadowValue = 0.4 + 0.6 * (1.0 - shadowFunc(lightFragPos));
		vec4 color  = texture(texturemap, textureCoordinate) * shadowValue;
		color.a = 1.0;

		finalColor = color;
	}	
}