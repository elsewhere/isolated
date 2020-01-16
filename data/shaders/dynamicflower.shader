VERTEX_SHADER
{
	#version 330

	uniform vec4 flowerColor;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;

	out VS_OUT
	{
		vec4 color;
	} vs_out;

	void main() {
	    // apply matrix transformations in the geometry shader
	    gl_Position = vec4(vertexPosition, 1);
	    vs_out.color = flowerColor;
	}
}

GEOMETRY_SHADER
{
	#version 330

	uniform mat4 viewMatrix;
	uniform mat4 modelMatrix;
	uniform mat4 projectionMatrix;

	layout (triangles_adjacency) in;
	layout (triangle_strip, max_vertices = 30) out;

	in VS_OUT
	{
		vec4 color;
	} gs_in[];

	out vec4 color;

	vec4 evaluateBezier(vec4 p0, vec4 p1, vec4 p2, vec4 p3, float t)
	{
		float t2 = t * t;
		float t3 = t2 * t;

		return p0 * (1.f - t)*(1.f - t)*(1.f - t) + p1 * 3.f*t*(1.f - t)*(1.f - t) + p2 * 3.f*t2*(1.f - t) + p3 * t3;
	}


	void main() 
	{
		mat4 modelViewProj = projectionMatrix * viewMatrix * modelMatrix;

		//gl_in[0] = startPosition
		//gl_in[1] = startPosition + startDirection1
		//gl_in[2] = startPosition
		//gl_in[3] = startPosition + startDirection2
		//gl_in[4] = endPosition - endDirection
		//gl_in[5] = endPosition

		for (float t = 0.0; t < 1.0; t += 2.0 / 30)
		{
			vec4 leftPoint =  modelViewProj * evaluateBezier(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[4].gl_Position, gl_in[5].gl_Position, t);
			vec4 rightPoint = modelViewProj * evaluateBezier(gl_in[0].gl_Position, gl_in[3].gl_Position, gl_in[4].gl_Position, gl_in[5].gl_Position, t);

			gl_Position = leftPoint;
			color = gs_in[0].color;
			EmitVertex();
			gl_Position = rightPoint;
			color = gs_in[0].color;
			EmitVertex();
		}
/*		
		for (int i = 0; i < 6; i++)
		{
			vec4 pos = modelView * gl_in[i].gl_Position;

			gl_Position = projectionMatrix * pos;
			color = gs_in[i].color;
			EmitVertex();
		}
*/		
	}    	
}

FRAGMENT_SHADER
{
	#version 330

	out vec4 finalColor;
	in vec4 color;

	void main() 
	{
		finalColor = color;

	}	
}