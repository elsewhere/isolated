VERTEX_SHADER
{
	#version 330

	uniform mat4 camera;
	uniform mat4 model;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	uniform sampler2D heightmap;
	uniform float time;

	uniform float terrainScale;
	uniform float terrainHeightScale;

	out vec2 textureCoordinate;
	out vec2 textureCoordinate2;

	out vec3 worldpos;

	out float height;

	void main() 
	{
	    // Pass the tex coord straight through to the fragment shader
	    textureCoordinate = vertexTextureCoordinate;

	    textureCoordinate *= terrainScale;
	    textureCoordinate2 = textureCoordinate * 1.0;

	    float heightVal = texture(heightmap, textureCoordinate).r;

	    vec3 vertex = vertexPosition;

/*
	float base = texture(heightmap, textureCoordinate).r;
	float octave1 = texture(heightmap2, textureCoordinate * 2.0).r;
	float octave2 = texture(heightmap3, textureCoordinate * 4.0).r;

	vertex.y = base * 24.0 + octave1 * 4.0 + octave2 * 2.0;


*/
/*
	    const float freq = 3.0;
	    float plasma = sin(textureCoordinate.x * 5.6 * freq + time * 51.4) * cos(textureCoordinate.y * 4.4 * freq - time * 43.4) + 
	    			   cos(textureCoordinate.x * 3.0 * freq + time * 33.5) - sin(textureCoordinate.y * 6.2 * freq - time * 22.3);
	    vertex.y = heightVal * 24.0 + plasma * 6.0;
*/
		height = heightVal * terrainHeightScale;
//	    vertex.y = height;

//	    vertex.xz *= 2.0;
	    
	    // Apply all matrix transformations to vert

	    vec4 pos = model * vec4(vertex, 1);
	    worldpos = pos.xyz;
	    gl_Position = camera * pos;
	}
}

FRAGMENT_SHADER
{
	#version 330

//	uniform sampler2D tex;

	uniform sampler2D heightmap;
	uniform sampler2D stone;

	uniform float lightvalue;
	uniform vec3 lightpos;
	uniform float lightradius;

	out vec4 finalColor;
	in vec2 textureCoordinate;
	in vec2 textureCoordinate2;
	in float height;

	in vec3 worldpos;

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

		vec3 lightDir = normalize(vec3(0.2, -0.0, 0.4));

		float lightness = clamp(1.0 - length(worldpos - lightpos) / lightradius, 0.0, 1.0) * lightvalue;
		float light = max(0.0, dot(normal, lightDir)) * lightness;

		float dim = max(0.0, pow(1.0 - heightValue.r, 15.0)) * 0.2;
//		if (dim > 0.0)
//		{
			col.xyz -= vec3(dim);
//		}

		finalColor = vec4(col.xyz * light, 1.0);

//		finalColor = vec4(getNormal(textureCoordinate), 1.0);
//		finalColor = vec4(col.xyz, 1.0);//vec4(textureCoordinate.x, textureCoordinate.y, 0.f, 1.f);//clamp(texel, 0.0, 1.0);

	}	
}