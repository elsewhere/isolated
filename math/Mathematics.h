#pragma once

#include "../Globals.h"

/*
    A class that contains mathematics-related static helper methods. 

*/


//tetrahedron
extern const int tetra_vertex_count;
extern const int tetra_triangle_count;
extern const glm::vec3 tetra_vertices[];
extern const int tetra_triangles[];

//hexahedron (cube)
extern const int hexa_vertex_count;
extern const int hexa_triangle_count;
extern const int hexa_face_count;
extern const glm::vec3 hexa_vertices[];
extern const int hexa_triangles[];

//octahedron
extern const int octa_vertex_count;
extern const int octa_triangle_count;
extern const glm::vec3 octa_vertices[];
extern const int octa_triangles[];

//icosahedron
extern const int icosa_vertex_count;
extern const int icosa_triangle_count;
extern const glm::vec3 icosa_vertices[];
extern const int icosa_triangles[];

//dodecahedron
extern const int dodeca_vertex_count;
extern const int dodeca_triangle_count;
extern const int dodeca_face_count;
extern const glm::vec3 dodeca_vertices[];
extern const int dodeca_triangles[];



class Math
{
public:
	Math(unsigned long s = 5489UL) {}; 
	~Math() {};

	static void init();

	//some ugly template functions for stuff
	template <class T> static T lerp(const float t, const T &a, const T &b)
	{
		return a + (b - a) * t;
	};
	template <class T> static T clamp(const T &t, const T &a, const T &b)
	{
		if (t < a)
			return a;
		if (t > b)
			return b;
		else return t;
	};
	template <class T>
	static float calcPos(T value, T limit1, T limit2)
	{
		if (value <= limit1)
			return 0;
		if (value >= limit2)
			return 1;
		else
			return (float)(value-limit1)/(limit2-limit1);
	};

	class Random
	{
	public:


		// Mersenne twister
		// initializes state[n] with a seed
		void srand(unsigned long s);

		// palauttaa [0 - max]
		float randFloat(float max = 1.0f);
		// palauttaa [0 - 0xFFFFFFFF]
		int randInt(int max = 0xFFFFFFFF);

		// palauttaa [0 - 0xFFFFFFFF]
		int rand();
		// palauttaa [min, max]
		float randBetween(float min, float max);
		// palauttaa [min, max]
		int randIntBetween(int min, int max);
		float randBetween(demomath::Range& range);
		float randBetween(float max);

		bool randBool(float probability = 0.5f);

	private:
		// Mersenne twister 
		enum MT_ENUM
		{
			N = 624
		};
		// generates a random number on [0,0xffffffff]-interval 
		unsigned long genrand_int32();

		// generates a random number on [0,0xffffffff]-interval 
		long genrand_int31();

		// generates a random number on [0,1]-real-interval 
		float genrand_real1();

		// generates a random number on [0,1)-real-interval 
		float genrand_real2();

		// generates a random number on (0,1)-real-interval
		float genrand_real3();

		// generates a random number on [0,1) with 53-bit resolution
		float genrand_res53();


		void next_state();
		unsigned long state[N];
		int left;
		unsigned long *next;


	};


	//phi = [0, pii], theta = [0, 2*pii], radius = [0, inf]
	static glm::vec3 sphereToCartesian(float radius, float phi, float theta);
	static glm::vec3 cartesianToSphere(float x, float y, float z);

	static void srand(int seed);
	// palauttaa [0 - max]
	static float randFloat(float max = 1.0f);
	// palauttaa [0 - 0xFFFFFFFF]
	static int randInt(int max = 0xFFFFFFFF);

	// palauttaa [min - max]
	static float randFloat(float min, float max);
	// palauttaa [max - 0xFFFFFFFF]
	static int randInt(int min, int max);

	// palauttaa [0 - 0xFFFFFFFF]
	static int rand();
	// palauttaa [min, max]
	static float randBetween(float min, float max);
	// palauttaa [min, max]
	static int randIntBetween(int min, int max);
	static float randBetween(demomath::Range& range);
	static float randBetween(float max);
	static bool randBool(float probability = 0.5f);

	static float smoothStep(float value, float limit1, float limit2);

	// Vector( [-0.5f, 0.5f]*kerroin
	static glm::vec3 randVector(float dx = 1.0f, float dy = 1.0f, float dz = 1.0f);
    static glm::vec3 randVectSphere(float radius = 1.0f);
	static glm::vec3 randVectHemisphere(glm::vec3& normal, float radius = 1.0f);
	static glm::vec3 randVectCircle(float radius = 1.0f);

	static glm::vec3 transform(const glm::vec3& v, const glm::mat4& trans);
	static glm::vec3 transform(const glm::vec4& v, const glm::mat4& trans);
	static std::string toString(glm::vec3 &v);
	static std::string toString(glm::vec4 &v);
	static std::string toString(glm::mat4 &m);

	template<typename T>
	static T evaluateBezier(const T& p0, const T& p1, const T& p2, const T& p3, float t)
	{
		const float t2 = t * t;
		const float t3 = t2 * t;
		return p0 * (1.f - t)*(1.f - t)*(1.f - t) + p1 * 3.f*t*(1.f - t)*(1.f - t) + p2 * 3.f*t2*(1.f - t) + p3 * t3;
	}
private:
	static Random sm_random;

};

