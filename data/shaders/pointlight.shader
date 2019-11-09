VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec2 vertexTextureCoordinate;
	in vec3 vertexNormal;

	out vec2 fragmentTextureCoordinate;
	out vec3 fragmentVertexPosition;
	out vec3 fragmentNormal;

	void main() {
	    // Pass the tex coord straight through to the fragment shader
	    fragmentTextureCoordinate = vertexTextureCoordinate;
	    fragmentVertexPosition = vertexPosition;
	    fragmentNormal = vertexNormal;
	    
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform mat4 modelMatrix;
	uniform mat3 normalMatrix;
	uniform sampler2D tex;

	uniform struct Light
	{
		vec3 position;
		vec3 color;
	} light;

	in vec2 fragmentTextureCoordinate;
	in vec3 fragmentVertexPosition;
	in vec3 fragmentNormal;

	out vec4 finalColor;

	void main() 
	{
		vec3 normal = normalize(normalMatrix * fragmentNormal);

		vec3 fragmentPosition = vec3(modelMatrix * vec4(fragmentVertexPosition, 1));
		vec3 surfaceToLight = light.position - fragmentPosition;

		float lightDistance = length(surfaceToLight);

		float brightness = dot(normal, surfaceToLight) / (lightDistance * length(normal));

		const float minDistance = 3.f;
		const float maxDistance = 20.f;
		float fade = 1.0 - (lightDistance - minDistance) / (maxDistance - minDistance);
		brightness = clamp(brightness * fade, 0, 1);

		vec4 texel = texture(tex, fragmentTextureCoordinate);
		finalColor = vec4(brightness * light.color * texel.rgb, texel.a);    
	}
}