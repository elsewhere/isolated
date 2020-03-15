VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

	out vec2 textureCoordinate;

	void main() 
	{
	    textureCoordinate = vertexTextureCoordinate;
	    textureCoordinate.y = 1.0 - textureCoordinate.y;

	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D tex;
	uniform float alpha;
	out vec4 finalColor;
	in vec2 textureCoordinate;

	void main() 
	{
		vec4 texel = texture(tex, textureCoordinate);
		finalColor = clamp(texel, 0.0, 1.0);
		finalColor.a = alpha;

	}	
}