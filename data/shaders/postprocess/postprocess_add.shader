VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

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
	uniform sampler2D tex2;

	uniform float amount;

	out vec4 finalColor;
	in vec2 textureCoordinate;

	void main() 
	{
	    finalColor = texture(tex, textureCoordinate) + 
	    			 texture(tex2, textureCoordinate) * amount;//vec4(1.0, 1.0, 1.0, 1.0);
//	    finalColor = texture(tex2, textureCoordinate);
//	    			 texture(tex2, textureCoordinate) * amount;//vec4(1.0, 1.0, 1.0, 1.0);
	}	
}