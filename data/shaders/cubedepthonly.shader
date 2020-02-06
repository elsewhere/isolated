VERTEX_SHADER
{
	#version 330

	uniform mat4 modelMatrix;

	in vec3 vertexPosition;

	void main() 
	{
	    gl_Position = modelMatrix * vec4(vertexPosition, 1.0);
	}
}

GEOMETRY_SHADER
{
	#version 330
	layout (triangles) in;
	layout (triangle_strip, max_vertices = 18) out;

	uniform mat4 shadowTransforms[6];

	out vec4 fragPos;

	void main()
	{
		for (int i = 0; i < 6; i++)
		{
			gl_Layer = i; 
			for (int j = 0; j < 3; j++)
			{
				fragPos  = gl_in[j].gl_Position;
				gl_Position = shadowTransforms[i] * fragPos;
				EmitVertex();
			}
			EndPrimitive();
		}
	}

}

FRAGMENT_SHADER
{
	#version 330

	in vec4 fragPos;
	uniform float farplane;
	uniform vec3 lightPos;

	void main() 
	{
		float d = length(fragPos.xyz - lightPos);
		d /= farplane;
		gl_FragDepth = d; //[0, 1]
	}	
}