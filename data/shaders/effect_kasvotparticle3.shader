FEEDBACK_VARYINGS
{
	particlePositionOut
	particleColorOut
	particleEnergyOut
	particleMaxEnergyOut

}
VERTEX_SHADER
{
	#version 330

	uniform float time;
	in vec3 particlePosition;
	in vec4 particleColor;
	in float particleEnergy;
	in float particleMaxEnergy;

	out vec3 particlePositionOut;
	out vec4 particleColorOut;
	out float particleEnergyOut;
	out float particleMaxEnergyOut;

	uniform sampler2D tex;

	float randhash(uint seed, float b)
	{
	    uint i=(seed^12345391u)*2654435769u;
	    i^=(i<<6u)^(i>>26u);
	    i*=2654435769u;
	    i+=(i<<5u)^(i>>12u);
	    return float(b * i) * (1.0 / 4294967295.0);
	}

	vec3 getDirection(vec3 position)
	{
		float s = time * 0.4;
		position *= 2.3;
		float x = sin(position.x * 1.2 + 1.2 * s) + cos(position.y * 0.5 + 1.1 * s);
		float y = cos(position.x * 2.5 + 0.7 * s) + cos(position.z * 2.1 - 1.4 * s);
		float z = sin(position.y * 1.0 + 1.1 * s) + cos(position.z * 3.3 + 0.5 * s)  - 3.0;

		return vec3(x, y, z) * 0.0007;
	}

	void main() 
	{
		particleEnergyOut = particleEnergy - 0.01;
		particleMaxEnergyOut = particleMaxEnergy;

		if (particleEnergyOut <= 0.0)
		{
			//create new particle  
		   uint seed = uint(time * 100.0) + uint(gl_VertexID);

		   float a = randhash(seed++, 2*3.141592);
		   float b = randhash(seed++, 2*3.141592);

		   float u = randhash(seed++, 1.0);
		   float v = randhash(seed++, 1.0);

		   vec3 generatedPos = vec3((u - 0.5), (v - 0.5), 0.0) * 4;
//		   vec3 generatedPos = vec3(sin(a) * sin(b), sin(a) * cos(b), cos(a));
		   particlePositionOut = generatedPos;

		   float c = randhash(seed++, 2*3.141592);
		   float d = randhash(seed++, 2*3.141592);

		   float energy = 0.5 + randhash(seed++, 1.4);
		   particleEnergyOut = energy;
		   particleMaxEnergyOut = energy;

		   particleColorOut = texture2D(tex, vec2(u, 1.0 - v));
		}
		else
		{
			particlePositionOut = particlePosition + getDirection(particlePosition);
			particleColorOut = particleColor;

		}
	}
}
