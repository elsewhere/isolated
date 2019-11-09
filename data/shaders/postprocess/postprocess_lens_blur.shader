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

	uniform sampler2D cocTexture;

	out vec4 finalColor;
	in vec2 textureCoordinate;

	const float fADOF_ShapeRadius = 12.0;
	const float fADOF_BokehCurve = 1.0;

	vec3 bokeh(sampler2D tex, vec2 pos, float coc, float centerDepth)
	{
		vec4 res = vec4(texture(tex, pos).xyz, 1.0);

		float discRadius = coc * 1.7;

		int rings = 6;
		for (int j =1 ; j <= rings; j++)
		{
			int samples = j * 2;
			float radius = j * (1.0 / rings) * discRadius;
			for (int i = 0; i < samples; i++)
			{
				float t = i * (1.0 / samples);

				vec2 offs = pos + vec2(sin(t * 6.28), cos(t * 6.28)) * radius;

				vec4 tap = texture(tex, offs);
				tap.w = (tap.w >= centerDepth*0.99) ? 1.0 : pow(abs(tap.w * 2.0 - 1.0),4.0); 

				res.xyz += pow(tap.xyz,vec3(fADOF_BokehCurve)) * tap.w;
				res.w += tap.w;
			}
		}
		res.xyz = max(res.xyz/res.w,0.0);
		return vec3(pow(res.xyz, vec3(1.0 / fADOF_BokehCurve)));

	}

	void main() 
	{
		vec4 textureValue = texture(cocTexture, textureCoordinate);//vec4(1.0, 1.0, 1.0, 1.0);

		float centerDepth = textureValue.w;

		float blurAmount = abs(centerDepth * 2.0 - 1.0);
		float discRadius = blurAmount * fADOF_ShapeRadius * 0.002;

		vec3 blurred = discRadius < 0.001 ? textureValue.xyz : bokeh(cocTexture, textureCoordinate, discRadius, centerDepth);
//		finalColor = vec4(blurred.xyz, 1.0);//vec4(vec3(blurAmount), 1.0);
		finalColor = vec4(blurred.xyz, 1.0);//vec4(vec3(blurAmount), 1.0);

//		finalColor = vec4(vec3(blurAmount), 1.0);
	}	
}