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
	uniform float near;
	uniform float far;

	out vec4 finalColor;
	in vec2 textureCoordinate;

	void main() 
	{
		float depth = texture(tex, textureCoordinate).r;
		float t =  (2.0 * near) / (far + near - depth * (far - near));
		finalColor = vec4(t, t, t, 1);
	}
}