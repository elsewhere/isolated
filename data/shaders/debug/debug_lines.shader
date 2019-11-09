VERTEX_SHADER
{
	#version 330

	uniform mat4 camera;
	uniform mat4 model;

	in vec3 vertexPosition;
	in vec4 vertexColor;

	out vec4 color;

	void main() 
	{
		color = vertexColor;
	    vec3 vertex = vertexPosition;

	    vec4 pos = model * vec4(vertex, 1);
	    gl_Position = camera * pos;
	}
}

FRAGMENT_SHADER
{
	#version 330

	in vec4 color;
	out vec4 finalColor;

	void main() 
	{
		finalColor = color;
	}	
}