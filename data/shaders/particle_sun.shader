VERTEX_SHADER
{
	#version 330

//	#include "testi.test"

	layout(location = 0) in vec3 vertexPosition;
	layout(location = 1) in float vertexSize;
	layout(location = 2) in vec4 vertexColor;

	out VS_OUT
	{
		vec4 color;
	} vs_out;

	void main() 
	{
	   gl_Position = vec4(vertexPosition, 1.0);
	   gl_PointSize = vertexSize;
	   vs_out.color = vertexColor;
	}
}

GEOMETRY_SHADER
{
	#version 330

	uniform mat4 viewMatrix;
	uniform mat4 modelMatrix;
	uniform mat4 projectionMatrix;

	layout (points) in;
	layout (triangle_strip, max_vertices = 4) out;

	in VS_OUT
	{
		vec4 color;
	} gs_in[];

	out vec2 textureCoordinate;
	out vec4 color;

	void main() 
	{
	   vec4 pos = viewMatrix * modelMatrix  * gl_in[0].gl_Position;
	   float size = gl_in[0].gl_PointSize;
	   color = gs_in[0].color;

	   textureCoordinate = vec2(0, 0);
	   gl_Position = projectionMatrix * (pos + size * vec4(-1, -1, 0, 0));
	   EmitVertex();
	   textureCoordinate = vec2(1, 0);
	   gl_Position = projectionMatrix * (pos + size * vec4(1, -1, 0, 0));
	   EmitVertex();
	   textureCoordinate = vec2(0, 1);
	   gl_Position = projectionMatrix * (pos + size * vec4(-1, 1, 0, 0));
	   EmitVertex();
	   textureCoordinate = vec2(1, 1);
	   gl_Position = projectionMatrix * (pos + size  * vec4(1, 1, 0, 0));
	   EmitVertex();
	}    	
}

FRAGMENT_SHADER
{
	#version 330

	uniform sampler2D tex;
	in vec2 textureCoordinate;
	in vec4 color;

	out vec4 finalColor;

	void main() 
	{
		vec4 texel = texture(tex, textureCoordinate);
		finalColor  = texel * color;
	}
}