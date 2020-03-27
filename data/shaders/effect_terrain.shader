VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	uniform sampler2D heightmap;
	uniform float time;

	uniform float terrainScale;
	uniform float terrainHeightScale;

	out vec2 textureCoordinate;
	out vec2 textureCoordinate2;

	out float height;

	void main() 
	{
	    textureCoordinate = vertexTextureCoordinate;

	    textureCoordinate *= terrainScale;
	    textureCoordinate2 = textureCoordinate * 1.0;

	    float heightVal = texture(heightmap, textureCoordinate).r;

	    vec3 vertex = vertexPosition;

		height = heightVal * terrainHeightScale;
	    vertex.y = height;

	    gl_Position = cameraMatrix * modelMatrix * vec4(vertex, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

//	uniform sampler2D tex;

	uniform sampler2D heightmap;
	uniform sampler2D stone;

	out vec4 finalColor;
	in vec2 textureCoordinate;
	in vec2 textureCoordinate2;
	in float height;

	vec3 getNormal(vec2 coord)
	{
		/*
			[ a b c ]
			[ d x e ]
			[ f g h ]
		*/

		const float offset = 0.001;

		float a = texture(heightmap, coord + vec2(-offset, -offset)).r;
		float b = texture(heightmap, coord + vec2(0.0, -offset)).r;
		float c = texture(heightmap, coord + vec2(offset, -offset)).r;
		float d = texture(heightmap, coord + vec2(-offset, 0.0)).r;
		float e = texture(heightmap, coord + vec2( offset, 0.0)).r;
		float f = texture(heightmap, coord + vec2(-offset, offset)).r;
		float g = texture(heightmap, coord + vec2(0.0, offset)).r;
		float h = texture(heightmap, coord + vec2(offset, offset)).r;

		float x = (c + 2.0 * e + h) - (a + 2.0 * d + f);
		float y = (a + 2.0 * b + c) - (f + 2.0 * g + h);
		float z = 0.2;

		return normalize(vec3(x, y, z));
	}


	void main() 
	{
		vec4 heightValue = texture(heightmap, textureCoordinate);
		vec4 col = mix(heightValue, texture(stone, textureCoordinate2), 0.4);

		vec3 normal = getNormal(textureCoordinate);

		vec3 lightDir = normalize(vec3(0.2, -0.0, 0.4)) ;

		float light = max(0.0, dot(normal, lightDir));

		float dim = max(0.0, pow(1.0 - heightValue.r, 15.0));
//		if (dim > 0.0)
//		{
			col.xyz -= vec3(dim);
//			col.xyz += vec3(0.7f, 0.3f, 0.2f) * dim * 6.2;
//		}

		finalColor = vec4(col.xyz * light, 1.0) * 0.7;

//		finalColor = vec4(getNormal(textureCoordinate), 1.0);
//		finalColor = vec4(col.xyz, 1.0);//vec4(textureCoordinate.x, textureCoordinate.y, 0.f, 1.f);//clamp(texel, 0.0, 1.0);

	}	
}