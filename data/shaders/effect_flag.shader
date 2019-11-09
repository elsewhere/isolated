VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec4 vertexColor;
	in vec3 vertexNormal;
	in vec2 vertexUv;
//	in vec2 vertexTextureCoordinate;
///	in vec3 vertexNormal;

//	out vec2 fragmentTextureCoordinate;
//	out vec3 fragmentVertexPosition;
//	out vec3 fragmentNormal;

	out vec4 color;
	out vec2 uv;
	out vec3 normal;

	void main() 
	{
	    // Pass the tex coord straight through to the fragment shader
//	    fragmentTextureCoordinate = vertexTextureCoordinate;
//	    fragmentVertexPosition = vertexPosition;
//	    fragmentNormal = vertexNormal;

		color = vertexColor;
		uv = vertexUv;
		normal = vertexNormal;
	    
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform mat4 modelMatrix;
//	uniform mat3 normalMatrix;

	in vec4 color;
	in vec3 normal;
	in vec2 uv;
	out vec4 finalColor;

	uniform sampler2D surfaceTexture;

	void main() 
	{

		vec3 directionalLight = normalize(vec3(0.7, 0.5, 0.1));
		float light = clamp(dot(normal, directionalLight), 0.0, 1.0);

		light = mix(light, 1.0, 0.8);
		vec4 texel = texture(surfaceTexture, uv * 2.0);
		finalColor = vec4(texel.xyz * color.xyz * light, 1.0);// * (uv.x + uv.y) * 0.5;//vec4(uv.x, uv.y, 0.0, 1.0);;
/*
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
*/		
	}
}