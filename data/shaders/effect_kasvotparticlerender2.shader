VERTEX_SHADER
{
	#version 330

	in vec3 vertexPosition;
	in vec4 vertexColor;
	in float vertexEnergy;
	in float vertexMaxEnergy;

	out VS_OUT
	{
		vec4 color;
		float energy;
		float maxEnergy;
	} vs_out;

	void main() 
	{
	   gl_Position = vec4(vertexPosition, 1.0);
	   gl_PointSize = 0.015;
	   vs_out.color = vertexColor;
	   vs_out.energy = vertexEnergy;
	   vs_out.maxEnergy = vertexMaxEnergy;
	}
}

GEOMETRY_SHADER
{
	#version 330

	uniform mat4 viewMatrix;
	uniform mat4 modelMatrix;
	uniform mat4 projectionMatrix;

	uniform vec3 cameraPosition;
	uniform float focusDistance;

	uniform float fade;

	layout (points) in;
	layout (triangle_strip, max_vertices = 4) out;

	in VS_OUT
	{
		vec4 color;
		float energy;
		float maxEnergy;
	} gs_in[];

	out vec2 textureCoordinate;
	out vec4 color;

	void main() 
	{
	   	vec4 pos = viewMatrix * modelMatrix  * gl_in[0].gl_Position;

		float dist = length(pos- vec4(cameraPosition, 1.0));
		float bokehDist = abs(focusDistance - dist);
		float bokeh = clamp(bokehDist * 0.1, 0.0, 1.0);
		float bokehSize = pow(bokeh, 2.5) * 25.0;

		float t = 1.0 - gs_in[0].energy / gs_in[0].maxEnergy;
		float alpha = sin(t * 3.1415) * fade;
		alpha -= bokeh * 0.97;

		if (alpha > 0.01)
		{
		   float size = gl_in[0].gl_PointSize * (1.0 + bokehSize);
		   color = gs_in[0].color;
		   color.w *= pow(alpha, 4.0);

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