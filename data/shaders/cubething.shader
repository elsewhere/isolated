VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	uniform mat4 lightMatrix;

	in vec3 vertexPosition;

	out vec4 lightFragPos;

	void main() {
	    // Apply all matrix transformations to vert

	    lightFragPos = lightMatrix * vec4(vertexPosition, 1.0);
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	}
}

FRAGMENT_SHADER
{
	#version 330

	out vec4 finalColor;

	in vec4 lightFragPos;

	uniform vec4 color;
	uniform sampler2D shadowMap;

	float shadowFunc(vec4 pos)
	{
	    vec3 projCoords = lightFragPos.xyz / lightFragPos.w;
	    // transform to [0,1] range
	    projCoords = projCoords * 0.5 + 0.5;
	    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	    float closestDepth = texture(shadowMap, projCoords.xy).r; 
	    // get depth of current fragment from light's perspective
	    float currentDepth = projCoords.z;
	    // check whether current frag pos is in shadow
	    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

	    return shadow;
	}

	void main() 
	{
		vec3 col = color.xyz * (1.0 - 0.4 * shadowFunc(lightFragPos));
		finalColor = vec4(col, 1.0);
	}	
}