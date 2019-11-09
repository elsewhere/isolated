VERTEX_SHADER
{
	#version 330

	uniform mat4 camera;
	uniform mat4 model;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	in vec3 vertexNormal;

	out vec2 textureCoordinate;

	void main() 
	{
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
	uniform sampler2D depthtex;
	uniform float near;
	uniform float far;
	uniform float focus;

	out vec4 finalColor;
	in vec2 textureCoordinate;

	void main() 
	{
		vec4 color =  texture(tex, textureCoordinate);//vec4(1.0, 1.0, 1.0, 1.0);
		float depth = texture(depthtex, textureCoordinate).r;
		float scaledDepth = (2.0 * near) / (far + near - depth * (far - near));

		float blurAmount = min(pow(abs(scaledDepth - focus), 0.5) * 0.7, 1.0) * 0.01;

		vec4 acc = vec4(0.0);
		acc += 1.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 2.0, -blurAmount * 2.0));
		acc += 4.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 1.0, -blurAmount *2.0));
		acc += 7.0 * texture(tex, textureCoordinate + vec2( 0.0, -blurAmount *2.0));
		acc += 4.0 * texture(tex, textureCoordinate + vec2( blurAmount * 1.0, -blurAmount *2.0));
		acc += 1.0 * texture(tex, textureCoordinate + vec2( blurAmount * 2.0, -blurAmount *2.0));
		acc += 4.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 2.0, -blurAmount * 1.0));
		acc += 16.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 1.0, -blurAmount *1.0));
		acc += 26.0 * texture(tex, textureCoordinate + vec2( 0.0, -blurAmount * 1.0));
		acc += 16.0 * texture(tex, textureCoordinate + vec2( blurAmount * 1.0, -blurAmount *1.0));
		acc += 4.0 * texture(tex, textureCoordinate + vec2( blurAmount * 2.0, -blurAmount *1.0));
		acc += 7.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 2.0, 0.0));
		acc += 26.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 1.0, 0.0));
		acc += 41.0 * texture(tex, textureCoordinate + vec2( 0.0, 0.0));
		acc += 26.0 * texture(tex, textureCoordinate + vec2( blurAmount * 1.0, 0.0));
		acc += 7.0 * texture(tex, textureCoordinate + vec2( blurAmount * 2.0, 0.0));
		acc += 4.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 2.0,  blurAmount * 1.0));
		acc += 16.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 1.0,  blurAmount *1.0));
		acc += 26.0 * texture(tex, textureCoordinate + vec2( 0.0, blurAmount * 1.0));
		acc += 16.0 * texture(tex, textureCoordinate + vec2( blurAmount * 1.0, blurAmount *1.0));
		acc += 4.0 * texture(tex, textureCoordinate + vec2( blurAmount * 2.0, blurAmount *1.0));
		acc += 1.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 2.0, blurAmount * 2.0));
		acc += 4.0 * texture(tex, textureCoordinate + vec2(-blurAmount * 1.0, blurAmount *2.0));
		acc += 7.0 * texture(tex, textureCoordinate + vec2( 0.0, blurAmount *2.0));
		acc += 4.0 * texture(tex, textureCoordinate + vec2( blurAmount * 1.0, blurAmount *2.0));
		acc += 1.0 * texture(tex, textureCoordinate + vec2( blurAmount * 2.0, blurAmount *2.0));
		acc *= 1.0 / 273.0;
/*
		for (int y = 0; y < 5; y++)
		{
			float yt = (y / 5.0 - 0.5) * 2;
			for (int x = 0; x < 5; x++)
			{
				float xt = (x / 5.0 - 0.5) * 2;
				acc += texture(tex, textureCoordinate + vec2(xt, yt) * blurAmount);
			}
		}
		acc *= 1.0 / 25.0;
*/
		finalColor = vec4(acc.xyz, 1.0);//vec4(vec3(pow(1.0 - scaledDepth, 6.0))*0.4 + color.xyz * 0.001, 1.0);
	}	
}