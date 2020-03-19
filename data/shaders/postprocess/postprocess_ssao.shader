VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	in vec3 vertexNormal;

	out vec2 textureCoordinate;

	void main() 
	{
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
	uniform sampler2D depthtex;
	uniform float near;
	uniform float far;

	out vec4 finalColor;
	in vec2 textureCoordinate;


	float rand(vec2 co){
	    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
	}

	void main() 
	{
		vec4 color =  texture(tex, textureCoordinate);//vec4(1.0, 1.0, 1.0, 1.0);
		float depth = texture(depthtex, textureCoordinate).r;
		float scaledDepth = (2.0 * near) / (far + near - depth * (far - near));

		float ao = 0.0;
		for (int i = 0; i < 64; i++)
		{
			vec2 point = textureCoordinate + (rand(textureCoordinate) - vec2(0.5)) * 0.1;
			float diff = depth - texture(depthtex, point).r;

			ao += diff;
		}

//		finalColor = vec4(vec3(ao), 1.0);//vec4(vec3(pow(1.0 - scaledDepth, 6.0))*0.4 + color.xyz * 0.001, 1.0);
		finalColor = vec4(color.xyz + vec3(ao), 1.0);
	}	
}