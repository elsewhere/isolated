VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	in vec3 vertexNormal;

	out vec2 textureCoordinate;
	out vec2 radialDirection;

	void main() {
	    // Pass the tex coord straight through to the fragment shader
	    textureCoordinate = vertexTextureCoordinate;
	    radialDirection = (vec2(0.0, 0.0) - (textureCoordinate - vec2(0.5))) / 256.0;
	    
	    // Apply all matrix transformations to vert
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D tex;
	out vec4 finalColor;
	in vec2 textureCoordinate;
	in vec2 radialDirection;

	void main() 
	{
		vec2 pos = textureCoordinate;
		vec4 accum = vec4(0.0);

		float weight = 1.0;

		for (int i = 0; i < 64; i++)
		{
			vec4 value = texture(tex, pos);

			accum += weight * value;
			weight *= 0.985;

			pos += radialDirection;

		}
		vec4 color =  accum;//vec4(1.0, 1.0, 1.0, 1.0);
		finalColor = color;
	}	
}