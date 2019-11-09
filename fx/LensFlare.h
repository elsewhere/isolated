#pragma once

#include <vector>

#include "../Globals.h"
#include "../render/LineRenderer.h"

namespace demofx
{
	class LensFlare
	{
		struct PatentFormat 
		{
			float r; //radius
			float d;
			float n; //index of refraction
			bool  f; //flat 
			float w;
			float h;
			float c;
		};

		//taken from https://github.com/greje656/PhysicallyBasedLensFlare

		struct LensDescription
		{
			static constexpr int aperture_opening = 7;

			// Nikon Lens
			const float d6 = 53.142f;
			const float d10 = 7.063f;
			const float d14 = 1.532f;
			const float dAp = 2.800f;
			const float d20 = 16.889f;
			const float Bf = 39.683f;
			const int nikon_aperture_id = 14;

			std::vector<PatentFormat> nikon_28_75mm = {
				{    72.747f,  2.300f, 1.60300f, false, 0.2f, 29.0f, 530 },
				{    37.000f, 13.000f, 1.00000f, false, 0.2f, 29.0f, 600 },

				{  -172.809f,  2.100f, 1.58913f, false, 2.7f, 26.2f, 570 },
				{    39.894f,  1.000f, 1.00000f, false, 2.7f, 26.2f, 660 },

				{    49.820f,  4.400f, 1.86074f, false, 0.5f, 20.0f, 330 },
				{    74.750f,      d6, 1.00000f, false, 0.5f, 20.0f, 544 },

				{    63.402f,  1.600f, 1.86074f, false, 0.5f, 16.1f, 740 },
				{    37.530f,  8.600f, 1.51680f, false, 0.5f, 16.1f, 411 },

				{   -75.887f,  1.600f, 1.80458f, false, 0.5f, 16.0f, 580 },
				{   -97.792f,     d10, 1.00000f, false, 0.5f, 16.5f, 730 },

				{    96.034f,  3.600f, 1.62041f, false, 0.5f, 18.0f, 700 },
				{   261.743f,  0.100f, 1.00000f, false, 0.5f, 18.0f, 440 },

				{    54.262f,  6.000f, 1.69680f, false, 0.5f, 18.0f, 800 },
				{ -5995.277f,     d14, 1.00000f, false, 0.5f, 18.0f, 300 },

				{       0.0f,     dAp, 1.00000f, true,  18.f, aperture_opening, 440 },

				{   -74.414f,  2.200f, 1.90265f, false, 0.5f, 13.0f, 500 },

				{   -62.929f,  1.450f, 1.51680f, false, 0.1f, 13.0f, 770 },
				{   121.380f,  2.500f, 1.00000f, false, 4.0f, 13.1f, 820 },

				{   -85.723f,  1.400f, 1.49782f, false, 4.0f, 13.0f, 200 },

				{    31.093f,  2.600f, 1.80458f, false, 4.0f, 13.1f, 540 },
				{    84.758f,     d20, 1.00000f, false, 0.5f, 13.0f, 580 },

				{   459.690f,  1.400f, 1.86074f, false, 1.0f, 15.0f, 533 },

				{    40.240f,  7.300f, 1.49782f, false, 1.0f, 15.0f, 666 },
				{   -49.771f,  0.100f, 1.00000f, false, 1.0f, 15.2f, 500 },

				{    62.369f,  7.000f, 1.67025f, false, 1.0f, 16.0f, 487 },
				{   -76.454f,  5.200f, 1.00000f, false, 1.0f, 16.0f, 671 },

				{   -32.524f,  2.000f, 1.80454f, false, 0.5f, 17.0f, 487 },
				{   -50.194f,      Bf, 1.00000f, false, 0.5f, 17.0f, 732 },

				{        0.f,     5.f, 1.00000f,  true, 10.f,  10.f, 500 }
			};

			// Angenieux Lens
			const int angenieux_aperture_id = 7;

			std::vector<PatentFormat> angenieux = {
				{ 164.13f,     10.99f, 1.67510f, false, 0.5f, 52.0f, 432 },
				{ 559.20f,      0.23f, 1.00000f, false, 0.5f, 52.0f, 532 },

				{ 100.12f,     11.45f, 1.66890f, false, 0.5f, 48.0f, 382 },
				{ 213.54f,      0.23f, 1.00000f, false, 0.5f, 48.0f, 422 },

				{ 58.04f,      22.95f, 1.69131f, false, 0.5f, 36.0f, 572 },

				{ 2551.10f,     2.58f, 1.67510f, false, 0.5f, 42.0f, 612 },
				{ 32.39f,      30.66f, 1.00000f, false, 0.3f, 36.0f, 732 },

				{ 0.0f,        10.00f, 1.00000f, true,  25.f,aperture_opening, 440 },

				{ -40.42f,      2.74f, 1.69920f, false, 1.5f, 13.0f, 602 },

				{ 192.98f,     27.92f, 1.62040f, false, 4.0f, 36.0f, 482 },
				{ -55.53f,      0.23f, 1.00000f, false, 0.5f, 36.0f, 662 },

				{ 192.98f,      7.98f, 1.69131f, false, 0.5f, 35.0f, 332 },
				{ -225.30f,     0.23f, 1.00000f, false, 0.5f, 35.0f, 412 },

				{ 175.09f,      8.48f, 1.69130f, false, 0.5f, 35.0f, 532 },
				{ -203.55f,      40.f, 1.00000f, false, 0.5f, 35.0f, 632 },

				{ 0.f,            5.f, 1.00000f,  true, 10.f,   5.f, 500 }
			};
		};


		struct LensInterface
		{
			glm::vec3 center;
			float radius; 
			glm::vec3 n; 

			float sa;

			float d1;
			float flat;
			float pos;
			float w;

		};

	public:
		LensFlare();
		~LensFlare();

		void debugDraw();

	private:

		void parseSystem(const std::vector<PatentFormat>& data);
		std::vector<LensInterface> m_system;

		float min_ior = 100000.f;
		float max_ior = 0.f;

		void debugDraw(const LensInterface &i);

		demorender::Camera m_debugCamera;
		std::unique_ptr<demorender::LineRenderer> m_lines;
	};

}