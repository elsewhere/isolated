VERTEX_SHADER
{
	#version 330

	layout(location = 0) in vec3 vertexPosition;
	layout(location = 1) in float vertexSize;
	layout(location = 2) in vec4 vertexColor;
	layout(location = 3) in float vertexEnergy;
	layout(location = 4) in float vertexMaxEnergy;

	out VS_OUT
	{
		vec4 color;
	} vs_out;

	void main() 
	{
	   gl_Position = vec4(vertexPosition, 1.0);
	   gl_PointSize = vertexSize;

	   float a = sin((vertexEnergy / vertexMaxEnergy)*3.1415);

	   vec4 outColor = vertexColor;
	   outColor.a *= a;

	   vs_out.color = outColor;
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

	layout (points) in;
	layout (triangle_strip, max_vertices = 4) out;

	in VS_OUT
	{
		vec4 color;
	} gs_in[];

	out vec2 textureCoordinate;
	out vec4 color;
	out float bokeh;

	void main() 
	{
		vec4 worldPos = modelMatrix * gl_in[0].gl_Position;
		vec4 pos = viewMatrix * worldPos;

		float dist = length(worldPos- vec4(cameraPosition, 1.0));

		float bokehDist = abs(focusDistance - dist);

		bokeh = clamp(bokehDist * 0.05, 0.0, 1.0);

		float bokehSize = pow(bokeh, 2.0) * 4.6f;

		float size = gl_in[0].gl_PointSize * (1.0 + bokehSize);
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
	in float bokeh;

	out vec4 finalColor;

	void main() 
	{
		vec4 texel = texture(tex, textureCoordinate);

//		texel.xyz = mix(texel.xyz, vec3(1.0, 0.0, 0.0), bokeh);

		vec4 col = texel * color;
		col.a *= clamp(1.0 - bokeh, 0.03, 1.0);

		finalColor = col;
	}
}