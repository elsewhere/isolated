VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	in vec3 vertexNormal;

	out vec2 textureCoordinate;

	void main() {
	    // Pass the tex coord straight through to the fragment shader
	    textureCoordinate = vertexTextureCoordinate;

	    // Apply all matrix transformations to vert
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D tex;
	uniform float blurscale;
	uniform bool horizontal;
	out vec4 finalColor;
	in vec2 textureCoordinate;

	uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

	void main() 
	{
//	    vec2 tex_offset = 1.0 / textureSize(tex, 0); // gets size of single texel

//	    tex_offset *= blurscale;
	    vec3 result = texture(tex, textureCoordinate).rgb * weight[0]; // current fragment's contribution

	    vec2 offset = vec2(0.0);
	    vec2 direction = normalize(textureCoordinate - vec2(0.5, 0.5));

	    for (int i = 1; i < 5; i++)
	    {
	    	float t = 0.2 / i;
	        result += texture(tex, textureCoordinate + offset).rgb * t;//weight[i];
	        result += texture(tex, textureCoordinate - offset).rgb * t;//weight[i];

	        offset += direction * blurscale;
	    }
/*

	    if (horizontal)
	    {
		    for(int i = 1; i < 5; ++i)
		    {
		        result += texture(tex, textureCoordinate + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		        result += texture(tex, textureCoordinate - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		    }
		}
		else
		{
		    for(int i = 1; i < 5; ++i)
		    {
		        result += texture(tex, textureCoordinate + vec2(0.0, tex_offset.x * i)).rgb * weight[i];
		        result += texture(tex, textureCoordinate - vec2(0.0, tex_offset.x * i)).rgb * weight[i];
		    }

		}
*/
		finalColor = vec4(result, 1.0);//vec4(1.0, 0.0, 0.0, 1.0);//vec4(texel.rgb, 1.0);
	}
}