VERTEX_SHADER
{
	#version 330

	uniform mat4 camera;
	uniform mat4 model;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

	out vec2 textureCoordinate;
	out vec3 worldpos;

	void main() 
	{
	    textureCoordinate = vertexTextureCoordinate;
	    vec3 vertex = vertexPosition;

	    vec4 pos = model * vec4(vertex, 1);
	    worldpos = pos.xyz;
	    gl_Position = camera * pos;
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D texturemap;

	out vec4 finalColor;

	in vec2 textureCoordinate;
	in vec3 worldpos;


	void main() 
	{
		vec4 color  = texture(texturemap, textureCoordinate);
		color.a = 1.0;

		finalColor = color;
	}	
}