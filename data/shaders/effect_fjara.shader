VERTEX_SHADER
{
	#version 330

	uniform mat4 camera;
	uniform mat4 model;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	uniform sampler2D heightmap;
	uniform sampler2D watermap;

	uniform float terrainScale;
	uniform float terrainHeightScale;

	out vec2 textureCoordinate;
	out vec2 textureCoordinate2;
	out float waterValue;

	out float height;

	void main() 
	{
	    // Pass the tex coord straight through to the fragment shader
	    textureCoordinate = vertexTextureCoordinate;

	    textureCoordinate *= terrainScale;
	    textureCoordinate2 = textureCoordinate * 4.0;

	    float heightVal = texture(heightmap, textureCoordinate).r;

	    waterValue = texture(watermap, textureCoordinate).r;
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
		height = heightVal * terrainHeightScale * 0.2 * (1.0 - waterValue);
	    vertex.y = height;

//	    vertex.xz *= 2.0;
	    
	    // Apply all matrix transformations to vert
	    gl_Position = camera * model * vec4(vertex, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

//	uniform sampler2D tex;

	uniform sampler2D heightmap;
	uniform sampler2D stonetexture;

	uniform sampler2D waternormals1;
	uniform sampler2D waternormals2;

	uniform samplerCube environment;

	out vec4 finalColor;
	in vec2 textureCoordinate;
	in vec2 textureCoordinate2;
	in float height;
	in float waterValue;

	uniform float time;

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

	vec4 calcWater(vec2 coord, float water)
	{

		const float waterScale = 7.0;
		const float waterSpeed = 0.005;

		vec2 w1 = coord.yx * waterScale * 0.6;
		vec2 w2 = coord.yx * waterScale * 0.7;
		w1.y -= time * 11.0 * waterSpeed;
		w2.y -= time * 5.0 * waterSpeed;
		vec4 water1 = texture(waternormals1, w1) * 1.4; //pienempi amplitudi
		vec4 water2 = texture(waternormals2, w2) * 0.9;

		vec3 normal = normalize(water1.xyz + water2.xyz);
		vec3 color = texture(environment, normal).xyz;

		color.xyz = pow(color.xyz, vec3(5.0));

		return vec4(color, 1.0);

	}


	void main() 
	{
		vec4 heightValue = texture(heightmap, textureCoordinate);
		vec4 col = mix(heightValue, texture(stonetexture, textureCoordinate2), 0.4);
		vec3 normal = getNormal(textureCoordinate);
		vec3 lightDir = normalize(vec3(0.2, -0.0, 0.4));
		float light = max(0.0, dot(normal, lightDir)) * 0.7;

		float dim = waterValue;//max(0.0, pow(1.0 - heightValue.r, 5.0)) * 0.2;
		col.xyz -= vec3(dim);

		vec4 waterColor = calcWater(textureCoordinate, waterValue);
//		finalColor = waterColor;
		finalColor = vec4(mix(col.xyz * light, waterColor.xyz, pow(waterValue, 0.7)), 1.0);
//		finalColor = vec4(mix(col.xyz * light + waterColor.xyz, waterValue), 1.0);
//		finalColor = vec4(vec3(waterValue), 1.0);

//		finalColor = vec4(waterColor.xyz, 1.0);
/*
		if (dim > 0.17)
		{
			finalColor = vec4(vec3(dim), 1.0);
		}
		else
		{
			finalColor = vec4(col.xyz * light, 1.0);
		}
*/	

//		finalColor = vec4(getNormal(textureCoordinate), 1.0);
//		finalColor = vec4(col.xyz, 1.0);//vec4(textureCoordinate.x, textureCoordinate.y, 0.f, 1.f);//clamp(texel, 0.0, 1.0);

	}	
}