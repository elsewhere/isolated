#pragma once

#include <string>
#include "../glm/glm.hpp"
#include "PostProcessManager.h"

namespace demorender
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// base
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class PostProcessEffect
	{
	public:
		PostProcessEffect();
		virtual ~PostProcessEffect();

		virtual void apply(const std::string& source, const std::string& target, PostProcessParameters& params);
		virtual std::string getName();
	private:
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// the rest
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class TestEffect : public PostProcessEffect
	{
	public:
		TestEffect();
		virtual ~TestEffect();

		virtual void apply(const std::string& source, const std::string& target, PostProcessParameters& params);
		std::string getName();
	};

	class Invert : public PostProcessEffect
	{
	public:
		Invert();
		virtual ~Invert();

		virtual void apply(const std::string& source, const std::string& target, PostProcessParameters& params);
		std::string getName();
	};

	class Sobel : public PostProcessEffect
	{
	public:
		Sobel();
		virtual ~Sobel();

		virtual void apply(const std::string& source, const std::string& target, PostProcessParameters& params);
		std::string getName();
	};

	class Glow : public PostProcessEffect
	{
	public:
		Glow();
		virtual ~Glow();

		virtual void apply(const std::string& source, const std::string& target, PostProcessParameters& params);
		std::string getName();
	};

	class SSAO : public PostProcessEffect
	{
	public:
		SSAO();
		virtual ~SSAO();
		
		virtual void apply(const std::string& source, const std::string& target, PostProcessParameters& params);
		std::string getName();
	};

	class DOF : public PostProcessEffect
	{
	public:
		DOF();
		virtual ~DOF();

		virtual void apply(const std::string& source, const std::string& target, PostProcessParameters& params);
		std::string getName();
	};

	class Lens : public PostProcessEffect
	{
	public:
		Lens();
		virtual ~Lens();

		virtual void apply(const std::string& source, const std::string& target, PostProcessParameters& params);
		std::string getName();
	};

}