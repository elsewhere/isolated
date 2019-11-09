VERTEX_SHADER
{
	#version 330

	uniform mat4 camera;
	uniform mat4 model;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

	out vec2 textureCoordinate;

	void main() {
	    // Pass the tex coord straight through to the fragment shader
	    textureCoordinate = vertexTextureCoordinate;
	    
	    // Apply all matrix transformations to vert
	    gl_Position = camera * model * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D tex;
	out vec4 finalColor;
	in vec2 textureCoordinate;

	void main() 
	{
		vec4 texel = texture(tex, textureCoordinate);
/*
		if (texel.r < 0.0)
			finalColor = vec4(0.5, 0.0, 0.0, 1.0);
		else if (texel.r > 1.0)
			finalColor = vec4(1.0, 0.0, 0.0, 1.0);
		else if (texel.g < 0.0)
			finalColor = vec4(0.0, 0.5, 0.0, 1.0);
		else if (texel.g > 1.0)
			finalColor = vec4(0.0, 1.0, 0.0, 1.0);
		else if (texel.b < 0.0)
			finalColor = vec4(0.0, 0.0, 0.5, 1.0);
		else if (texel.b > 1.0)
			finalColor = vec4(0.0, 0.0, 1.0, 1.0);
		else if (texel.a < 0.0)
			finalColor = vec4(0.5, 0.5, 0.5, 1.0);
		else if (texel.a > 1.0)
			finalColor = vec4(1.0, 1.0, 1.0, 1.0);
		else
		{
			finalColor = texel;
		}
		*/
		finalColor = clamp(texel, 0.0, 1.0);

	}	
}