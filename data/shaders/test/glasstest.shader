VERTEX_SHADER
{
	#version 330

	uniform mat4 cameraMatrix;
	uniform mat4 modelMatrix;

	in vec3 vertexPosition;
	in vec3 vertexNormal;

	out vec3 fragmentNormal;
	out vec3 fragmentPosition;

	void main() 
	{
		fragmentNormal = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
		fragmentPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));

	    // Apply all matrix transformations to vert
	    gl_Position = cameraMatrix * modelMatrix * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

	uniform samplerCube tex;
	uniform vec3 cameraPosition;

	in vec3 fragmentPosition;
	in vec3 fragmentNormal;

	out vec4 finalColor;

	void main() 
	{
		const float coeff = 0.65;
		const float dispersion = 0.01;
		vec3 eye = normalize(fragmentPosition - cameraPosition);
		vec3 reflection = reflect(eye, fragmentNormal); //faaaakeee
		vec3 refractionR  = refract(eye, fragmentNormal, coeff);
		vec3 refractionG  = refract(eye, fragmentNormal, coeff + dispersion);
		vec3 refractionB  = refract(eye, fragmentNormal, coeff + dispersion * 2.0);

		vec4 col = texture(tex, reflection);
		vec4 colR = vec4(texture(tex, refractionR).r, 0.0, 0.0, 0.0);
		vec4 colG = vec4(0.0, texture(tex, refractionG).g, 0.0, 0.0);
		vec4 colB = vec4(0.0, 0.0, texture(tex, refractionB).b, 0.0);

		vec4 refractedColor = (colR + colG + colB) * 0.333;

		float ratio = 0.9;

		vec3 glassColor = vec3(0.95, 0.95, 1.0) * 3.0;

		refractedColor.xyz *= glassColor;

	    finalColor = col + (refractedColor - col) * ratio;//vec4(1.0, 1.0, 1.0, 1.0);
	}	
}
