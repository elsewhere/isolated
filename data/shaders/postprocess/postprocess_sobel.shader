VERTEX_SHADER
{
	#version 330

	uniform mat4 camera;
	uniform mat4 model;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	in vec3 vertexNormal;

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

	uniform float spacing;
	uniform float exponent;

	vec3 sample(vec2 offset, vec2 fragmentPos)
	{
	    return texture(tex, fragmentPos + offset).rgb;
	}

	// -1 -2 -1
	//  0  0  0
	//  1  2  1

	// -1  0  1
	// -2  0  2
	// -1  0  1

	float lum(vec3 color)
	{
	    return dot(color, vec3(.2126, .7152, .0722));
	}

	vec3 sobel(vec2 pos)
	{
	    vec3 horizontal = sample(vec2(-spacing, -spacing), pos) * -1.0 +
	                      sample(vec2(0.0, -spacing), pos) * -2.0 + 
	                      sample(vec2(spacing, -spacing), pos) * -1.0 + 
	                      sample(vec2(-spacing,  spacing), pos) * 1.0 +
	                      sample(vec2(0.0,  spacing), pos) * 2.0 + 
	                      sample(vec2(spacing, spacing), pos) * 1.0;

	    vec3 vertical = sample(vec2(-spacing, -spacing), pos) * -1.0 + 
	                    sample(vec2(-spacing, 0.0), pos) * -2.0 + 
	                    sample(vec2(-spacing, spacing), pos) * -1.0 + 
	                    sample(vec2( spacing, -spacing), pos) *  1.0 + 
	                    sample(vec2( spacing, 0.0), pos) *  2.0 + 
	                    sample(vec2( spacing, spacing), pos) * 1.0;

	//    return sample(vec2(0.0), pos) + pow(lum(horizontal*horizontal + vertical*vertical), 1.0);
	    return vec3(pow(lum(horizontal*horizontal + vertical*vertical), exponent));

	}

	void main() 
	{
	    finalColor = vec4(sobel(textureCoordinate), 1.0);
	}
}

