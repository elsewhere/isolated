FEEDBACK_VARYINGS
{
	particlePositionOut
	particleEnergyOut
	particleMaxEnergyOut

}
VERTEX_SHADER
{
	#version 330

	uniform float time;
	in vec3 particlePosition;
	in float particleEnergy;
	in float particleMaxEnergy;

	out vec3 particlePositionOut;
	out float particleEnergyOut;
	out float particleMaxEnergyOut;

//	uniform sampler2D tex;

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
		return normalize(position) * 0.0011;
	}

	#include "simplexnoise.fragment"

	void main() 
	{
		particleEnergyOut = particleEnergy - 0.01;
		particleMaxEnergyOut = particleMaxEnergy;

		if (particleEnergyOut <= 0.0)
		{
			//create new particle  
		   uint seed = uint(time * 100.0) + uint(gl_VertexID);

		   float x = randhash(seed++, 2.0) - 1.0;
		   float y = randhash(seed++, 2.0) - 1.0;
		   float z = randhash(seed++, 2.0) - 1.0;

		   vec3 pos = normalize(vec3(x, y, z));
//		   vec3 pos = vec3(sin(a), 0.0, cos(b)) * 5.0;
		   particlePositionOut = pos;

		   float c = randhash(seed++, 2*3.141592);
		   float d = randhash(seed++, 2*3.141592);

		   float energy = 0.5 + randhash(seed++, 8.4);
		   particleEnergyOut = energy;
		   particleMaxEnergyOut = energy;

		}
		else
		{
			particlePositionOut = particlePosition + getDirection(particlePosition);

		}
	}
}
