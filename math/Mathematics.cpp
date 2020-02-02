#include "Mathematics.h"
#include <mmsystem.h> //timeGetTime
#include <iostream>


Math::Random Math::sm_random;

// Mersenne twister defines
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UMASK 0x80000000UL /* most significant w-r bits */
#define LMASK 0x7fffffffUL /* least significant r bits */
#define MIXBITS(u,v) ( ((u) & UMASK) | ((v) & LMASK) )
#define TWIST(u,v) ((MIXBITS(u,v) >> 1) ^ ((v)&1UL ? MATRIX_A : 0UL))

using namespace demomath;
using glm::vec2;
using glm::vec3;

//tetrahedron
const int tetra_vertex_count = 4;
const int tetra_triangle_count = 4;

//hexahedron (cube)
const int hexa_vertex_count = 8;
const int hexa_triangle_count = 12;
const int hexa_face_count = 6;

//octahedron
const int octa_vertex_count = 6;
const int octa_triangle_count = 8;

//icosahedron
const int icosa_vertex_count = 12;
const int icosa_triangle_count = 20;

//dodecahedron
const int dodeca_vertex_count = 20;
const int dodeca_triangle_count = 36;
const int dodeca_face_count = 12;

const glm::vec3 tetra_vertices[tetra_vertex_count] = 
{
	vec3(-1, -1, -1),
	vec3(1, 1, -1),
	vec3(1, -1, 1),
	vec3(-1, 1, 1)
};

const int tetra_triangles[tetra_triangle_count * 3] = 
{
	1,2,3,
	1,0,2,
	3,2,0,
	0,1,3
};


const vec3 hexa_vertices[8] = 
{
	vec3(-1, -1, -1),
	vec3( 1, -1, -1),
	vec3( 1,  1, -1),
	vec3(-1,  1, -1),
	vec3(-1, -1,  1),
	vec3( 1, -1,  1),
	vec3( 1,  1,  1),
	vec3(-1,  1,  1),
};
static int hexa_faces[hexa_face_count][4] = 
{
	{0, 1, 2, 3},
	{5, 4, 7, 6},
	{6, 2, 1, 5},
	{3, 7, 4, 0},
	{7, 3, 2, 6},
	{5, 1, 0, 4},
};

const int hexa_triangles[hexa_triangle_count * 3] = 
{
	0, 1, 3,
	1, 2, 3,
	5, 4, 6,
	4, 7, 6,
	6, 2, 5,
	2, 1, 5,
	3, 7, 0,
	7, 4, 0,
	7, 3, 6,
	3, 2, 6,
	5, 1, 4,
	1, 0, 4,
};


const vec3 octa_vertices[octa_vertex_count] = 
{
	vec3(1,  0,  0),
	vec3(0, -1,  0),
	vec3(-1,  0,  0),
	vec3(0,  1,  0),
	vec3(0,  0,  1),
	vec3(0,  0, -1)
};

const int octa_triangles[octa_triangle_count * 3] = 
{
	4, 0, 1,
	4, 1, 2,
	4, 2, 3,
	4, 3, 0,
	5, 1, 0,
	5, 2, 1,
	5, 3, 2,
	5, 0, 3,
};

const vec3 icosa_vertices[icosa_vertex_count] = 
{
	vec3(0, -0.52573f,  0.85065f),
	vec3(0.85065f,  0,  0.52573f),
	vec3(0.85065f,  0, -0.52573f),
	vec3(-0.85065f,  0, -0.52573f),
	vec3(-0.85065f,  0,  0.52573f),
	vec3(-0.52573f,  0.85065f,  0),
	vec3(0.52573f,  0.85065f,  0),
	vec3(0.52573f, -0.85065f,  0),
	vec3(-0.52573f, -0.85065f,  0),
	vec3(0, -0.52573f, -0.85065f),
	vec3(0,  0.52573f, -0.85065f),
	vec3(0,  0.52573f,  0.85065f)
};


const int icosa_triangles[icosa_triangle_count * 3] = 
{
	6, 2, 1, 
	2, 7, 1, 
	5, 4, 3, 
	8, 3, 4, 
	11, 5, 6, 
	10, 6, 5, 
	2, 10, 9, 
	3, 9, 10, 
	9, 8, 7, 
	0, 7, 8, 
	1, 0, 11, 
	4, 11, 0, 
	10, 2, 6, 
	11, 6, 1, 
	10, 5, 3, 
	11, 4, 5, 
	9, 7, 2, 
	0, 1, 7, 
	8, 9, 3, 
	0, 8, 4, 
};
const vec3 dodeca_vertices[dodeca_vertex_count] = 
{
	vec3(-0.57735f, -0.57735f, 0.57735f),
	vec3(0.93417f, 0.35682f, 0),
	vec3(0.93417f, -0.35682f, 0),
	vec3(-0.93417f, 0.35682f, 0),
	vec3(-0.93417f, -0.35682f, 0),
	vec3(0, 0.93417f, 0.35682f),
	vec3(0, 0.93417f, -0.35682f),
	vec3(0.35682f, 0, -0.93417f),
	vec3(-0.35682f, 0, -0.93417f),
	vec3(0, -0.93417f, -0.35682f),
	vec3(0, -0.93417f, 0.35682f),
	vec3(0.35682f, 0, 0.93417f),
	vec3(-0.35682f, 0, 0.93417f),
	vec3(0.57735f, 0.57735f, -0.57735f),
	vec3(0.57735f,0.57735f,0.57735f),
	vec3(-0.57735f, 0.57735f, -0.57735f),
	vec3(-0.57735f, 0.57735f, 0.57735f),
	vec3(0.57735f, -0.57735f, -0.57735f),
	vec3(0.57735f, -0.57735f, 0.57735f),
	vec3(-0.57735f, -0.57735f, -0.57735f)
};

const int dodeca_faces[12][5] = 
{
	{1, 2, 18, 11, 14}, 
	{1, 13, 7, 17, 2}, 
	{3, 4, 19, 8, 15}, 
	{3, 16, 12, 0, 4}, 
	{3, 15, 6, 5, 16}, 
	{1, 14, 5, 6, 13}, 
	{2, 17, 9, 10, 18}, 
	{4, 0, 10, 9, 19}, 
	{7, 8, 19, 9, 17}, 
	{6, 15, 8, 7, 13}, 
	{5, 14, 11, 12, 16}, 
	{10, 0, 12, 11, 18}, 
};

const int dodeca_triangles[dodeca_triangle_count * 3] = 
{
//	1, 2, 18, 11, 14, 
	1, 2, 14,
	2, 18, 14,
	18, 11, 14,
//	1, 13, 7, 17, 2, 
	1, 13, 2,
	13, 7, 2,
	7, 17, 2,
//	3, 4, 19, 8, 15, 
	3, 4, 15,
	4, 19, 15,
	19, 8, 15,

//	3, 16, 12, 0, 4, 
	3, 16, 4,
	16, 12, 4,
	12, 0, 4,
//	3, 15, 6, 5, 16, 
	3, 15, 16,
	15, 6, 16,
	6, 5, 16,
//	1, 14, 5, 6, 13, 
	1, 14, 13,
	14, 5, 13,
	5, 6, 13,
//	2, 17, 9, 10, 18, 
	2, 17, 18,
	17, 9, 18,
	9, 10, 18,
//	4, 0, 10, 9, 19, 
	4, 0, 19,
	0, 10, 19,
	10, 9, 19,
//	7, 8, 19, 9, 17, 
	7, 8, 17,
	8, 19, 17,
	19, 9, 17,
//	6, 15, 8, 7, 13, 
	6, 15, 13,
	15, 8, 13,
	8, 7, 13,
//	5, 14, 11, 12, 16, 
	5, 14, 16,
	14, 11, 16,
	11, 12, 16,
//	10, 0, 12, 11, 18, 
	10, 0, 18,
	0, 12, 18,
	12, 11, 18,

};





void Math::init()
{
	srand(timeGetTime());
}

vec3 Math::sphereToCartesian(float radius, float phi, float theta)
{
	float sp = (float)sin(phi);
	float cp = (float)cos(phi);
	float st = (float)sin(theta);
	float ct = (float)cos(theta);
	vec3 value = vec3(sp*ct, sp*st, cp) * radius;
	return value;
}
vec3 Math::cartesianToSphere(float x, float y, float z)
{
	float rho = (float)sqrt(x*x+y*y+z*z);
	if (fabsf(rho) < 0.00001f) 
		return vec3(0,0,0);
/*
    float phi = (float)acos(z/rho);
	float theta = (float)acos(x/(rho*sin(phi)));
*/

    float phi = atan2f(y, x);
    float theta = acosf(z / rho);
	vec3 value = vec3(rho, theta, phi);
	return value;
}

float Math::smoothStep(float value, float limit1, float limit2)
{ 
  if (value < limit1) return 0;
  if (value >= limit2) return 1; 
  float x = clamp<float>((value - limit1)/(limit2 - limit1),0,1); 
  return (x*x * (3 - 2*x)); 
}
             
float Math::Random::randFloat(float max)
{
	return genrand_real1() * max;
}

int Math::Random::randInt(int max)
{
	return genrand_int32() % max;
}

int Math::Random::rand()
{
	return genrand_int32();
}

bool Math::Random::randBool(float probability)
{
	return genrand_real1() < probability;
}

float Math::Random::randBetween(float min, float max)
{
    return min + (max - min) * genrand_real1();
}
float Math::Random::randBetween(float max)
{
    return (2 * max * genrand_real1()) - max;
}

float Math::Random::randBetween(Range &r)
{
	return randBetween(r.getMinimum(), r.getMaximum());
}
int Math::Random::randIntBetween(int min, int max)
{
	if (min > max)
	{
		return max + (genrand_int31() & (min - max));
	}
	else if (min == max)
	{
		return min;
	}
	else
	{
		return min + (genrand_int31() & (max - min));
	}

}


void Math::srand(int seed)
{
	sm_random.srand(seed);
}

float Math::randFloat(float max)
{
	return sm_random.randFloat(max);
}
// palauttaa [0 - 0xFFFFFFFF]
int Math::randInt(int max)
{
	return sm_random.randInt(max);
}

float Math::randFloat(float min, float max)
{
	return sm_random.randBetween(min, max);
}
// palauttaa [0 - 0xFFFFFFFF]
int Math::randInt(int min, int max)
{
	return sm_random.randIntBetween(min, max);
}

// palauttaa [0 - 0xFFFFFFFF]
int Math::rand()
{
	return sm_random.rand();
}

// palauttaa [min, max]
float Math::randBetween(float min, float max)
{
	return sm_random.randBetween(min, max);
}
// palauttaa [min, max]
int Math::randIntBetween(int min, int max)
{
	return sm_random.randIntBetween(min, max);
}
float Math::randBetween(demomath::Range& range)
{
	return sm_random.randBetween(range);
}
float Math::randBetween(float max)
{
	return sm_random.randBetween(max);
}
bool Math::randBool(float probability)
{
	return sm_random.randBool(probability);
}

vec3 Math::randVector(float dx, float dy, float dz)
{
	return vec3(dx * (randFloat() * 2 - 1.0f), 
				   dy * (randFloat() * 2 - 1.0f), 
				   dz * (randFloat() * 2 - 1.0f));
}

vec3 Math::randVectSphere(float radius)
{
	float s = randFloat() * 3.141592f * 2;
	float t = randFloat() * 2.f - 1.f;

	float u = sqrt(1.0f - t * t);

	return vec3(sinf(s) * u, cosf(s) * u, t) * radius;
}

vec3 Math::randVectHemisphere(vec3& normal, float radius)
{
	vec3 v = randVectSphere();
	return v * glm::sign(glm::dot(v, normal));
}
vec3 Math::randVectCircle(float radius)
{
	float a = randFloat()*2*3.141592f;
	return vec3(sinf(a), cosf(a), 0.0f) * radius;
	
}

// Mersenne twister
void Math::Random::srand(unsigned long s)
{
    int j;
    state[0]= s & 0xffffffffUL;
    for (j=1; j<N; j++) {
        state[j] = (1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array state[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        state[j] &= 0xffffffffUL;  /* for >32 bit machines */
    }
    left = 1; //initf = 1;
}

void Math::Random::next_state()
{
    unsigned long *p=state;
    int j;

    /* if init_genrand() has not been called, */
    /* a default initial seed is used         */
    /* initf is disabled, the class explicitly needs to be initialized
     * via a constructor which leaves the object to a known state.
     * moved the default seed to the header. -- Jetro Lauha 2004-10-07
     */
    //if (initf==0) init_genrand(5489UL);

    left = N;
    next = state;
    
    for (j=N-M+1; --j; p++) 
        *p = p[M] ^ TWIST(p[0], p[1]);

    for (j=M; --j; p++) 
        *p = p[M-N] ^ TWIST(p[0], p[1]);

    *p = p[M-N] ^ TWIST(p[0], state[0]);
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long Math::Random::genrand_int32(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long Math::Random::genrand_int31(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (long)(y>>1);
}

/* generates a random number on [0,1]-real-interval */
float Math::Random::genrand_real1(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (float)y * (1.0f/4294967295.0f); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
float Math::Random::genrand_real2(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (float)y * (1.0f/4294967296.0f);
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
float Math::Random::genrand_real3(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return ((float)y + 0.5f) * (1.0f/4294967296.0f); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
float Math::Random::genrand_res53(void)
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6; 
    return(a*67108864.0f+b)*(1.0f/9007199254740992.f); 
} 


std::string Math::toString(glm::vec3 &v)
{
	std::stringstream ss;
	ss << "[" << v.x << "," << v.y << "," << v.z << "]";
	return ss.str();

}
std::string Math::toString(glm::vec4 &v)
{
	std::stringstream ss;
	ss << "[" << v.x << "," << v.y << "," << v.z << "," << v.w << "]";
	return ss.str();

}
std::string Math::toString(glm::mat4 &m)
{
	std::stringstream ss;
	ss << std::endl;
	ss << "[" << m[0][0] << "," << m[1][0] << "," << m[2][0] << "," << m[3][0] << "]" << std::endl;
	ss << "[" << m[0][1] << "," << m[1][1] << "," << m[2][1] << "," << m[3][1] << "]" << std::endl;
	ss << "[" << m[0][2] << "," << m[1][2] << "," << m[2][2] << "," << m[3][2] << "]" << std::endl;
	ss << "[" << m[0][3] << "," << m[1][3] << "," << m[2][3] << "," << m[3][3] << "]" << std::endl;
	return ss.str();
}


glm::vec3 Math::transform(const glm::vec3& v, const glm::mat4& trans)
{
	glm::vec4 vec = glm::vec4(v, 1.0f);
	vec = trans * vec;
	return glm::vec3(vec.x, vec.y, vec.z);

}
glm::vec3 Math::transform(const glm::vec4& v, const glm::mat4& trans)
{
	glm::vec4 vec = v * trans;
	return glm::vec3(vec.x, vec.y, vec.z);
}
