VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	in vec3 vertexNormal;

	out vec2 textureCoordinate;

	flat out uint seed;
	uniform float time;

	void main() {
	    // Pass the tex coord straight through to the fragment shader
	    textureCoordinate = vertexTextureCoordinate;
	    seed = uint(time * 100.0 + gl_VertexID);

	    // Apply all matrix transformations to vert
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D tex;
//	uniform float time;
	out vec4 finalColor;
	in vec2 textureCoordinate;

	uniform float power;
	flat in uint seed;

	float randhash(uint seed, float b)
	{
	    uint i=(seed^12345391u)*2654435769u;
	    i^=(i<<6u)^(i>>26u);
	    i*=2654435769u;
	    i+=(i<<5u)^(i>>12u);
	    return float(b * i) * (1.0 / 4294967295.0);
	}


	void main() 
	{
		uint s  = uint(seed + textureCoordinate.x * 510501.5 + textureCoordinate.y * 5419591.4);
	    float r = mix(1.0, 0.4 + randhash(s, 0.6), power);

		vec4 original = texture(tex, textureCoordinate);
//		original.x += 0.3;
		original.xyz *= vec3(r);
	    finalColor = original;
	}
}

