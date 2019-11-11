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

	void main() 
	{
		vec4 color =  texture(tex, textureCoordinate);//vec4(1.0, 1.0, 1.0, 1.0);
		finalColor = vec4(vec3(1.0) - color.xyz, 1.0);
	}	
}