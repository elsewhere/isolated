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

	uniform sampler2D colorTex;
	uniform sampler2D depthTex;
	uniform float nearPlane;
	uniform float farPlane;
	uniform float focus;

	out vec4 finalColor;
	in vec2 textureCoordinate;

	const float infiniteFocus = 1.0; 
	const float farBlur = 2.0;

	float getCoC(vec2 pos)
	{
		float coc = 0.0;
		float depth = texture(depthTex, textureCoordinate).r;
		float linearDepth  = (2.0 * nearPlane) / (farPlane + nearPlane - depth  * (farPlane - nearPlane));

		float focusValue = mix(0.0, infiniteFocus, focus); 
		float depthValue = mix(0.0, infiniteFocus, linearDepth);

		float farBlurDepth = focusValue * pow(4.0, farBlur);
		if (depthValue < focusValue)
		{
			coc = (depthValue - focusValue) / focusValue;
		}
		else
		{
			coc = (depthValue - focusValue) / (farBlurDepth - focusValue);
			coc = clamp(coc, 0.0, 1.0);
		}

		coc = clamp(coc * 0.5 + 0.5, 0.0, 1.0);
		return coc;
	}

	void main() 
	{
		vec4 color =  texture(colorTex, textureCoordinate);
		finalColor = vec4(color.xyz, getCoC(textureCoordinate));
	}	
}