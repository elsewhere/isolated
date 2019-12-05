#include "PostProcessEffect.h"
#include "Shader.h"

using namespace demomath;

namespace demorender
{
	PostProcessEffect::PostProcessEffect()
	{

	}

	PostProcessEffect::~PostProcessEffect()
	{

	}

	void PostProcessEffect::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		//default behavior is to just blit 
		g_renderTargets->bindTexture(target);
		g_renderUtils->fullscreenQuad(source);
	}

	std::string PostProcessEffect::getName()
	{
		return "<base class>";
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Invert
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Invert::Invert()
	{

	}

	Invert::~Invert()
	{

	}

	void Invert::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		Shader& shader = g_shaders->getShader("postprocess_invert");
		shader.bind();

		glActiveTexture(GL_TEXTURE0);

		shader.setUniform1i("tex", 0);
		g_renderTargets->bindTexture(target);
		g_renderUtils->fullscreenQuad(source, shader);
	}
	std::string Invert::getName()
	{
		return "invert";
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sobel
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Sobel::Sobel()
	{

	}

	Sobel::~Sobel()
	{

	}

	void Sobel::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		Shader& shader = g_shaders->getShader("postprocess_sobel");
		shader.bind();

		shader.setUniform1f("spacing", std::get<float>(params["spacing"]));
		shader.setUniform1f("exponent", std::get<float>(params["exponent"]));

		glActiveTexture(GL_TEXTURE0);

		g_renderTargets->bindTexture(target);
		g_renderUtils->fullscreenQuad(source, shader);
	}
	std::string Sobel::getName()
	{
		return "sobel";
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Glow
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Glow::Glow()
	{

	}

	Glow::~Glow()
	{

	}

	void Glow::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		//take source rendertarget
		//downscale it a few times
		//blur horizontally and vertically
		//add the result on top of the source rendertarget


		const int iterations = std::get<int>(params["iterations"]);
		const float spreadX = std::get<float>(params["spreadX"]);
		const float spreadY = std::get<float>(params["spreadY"]);
		const float exponent = std::get<float>(params["exponent"]);
		const float alpha = std::get<float>(params["alpha"]);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		Shader& simpleCopy = g_shaders->getShader("simpletexture");
		Shader& expShader = g_shaders->getShader("postprocess_exp");
		Shader& blurShader = g_shaders->getShader("postprocess_blur");
		Shader& combine = g_shaders->getShader("postprocess_add");

		//downscale and blit source to downscale1
		g_renderTargets->bindTexture("downscale1");
		simpleCopy.bind();
		simpleCopy.setUniform1i("tex", 0);
		g_textures->bindTexture(source, GL_TEXTURE0); //tästä lähtee
		g_renderUtils->fullscreenQuad(simpleCopy);

		//downscale and use exp filter to downscale 2
		g_renderTargets->bindTexture("downscale2");
		expShader.bind();
		g_textures->bindTexture("downscale1", GL_TEXTURE0);
		expShader.setUniform1i("tex", 0);
		expShader.setUniform1f("expfactor", exponent);
		g_renderUtils->fullscreenQuad(expShader);

		//downscale and blit downcale2 to downscale3
		g_renderTargets->bindTexture("downscale3");
		simpleCopy.bind();
		g_textures->bindTexture("downscale2", GL_TEXTURE0);
		simpleCopy.setUniform1i("tex", 0);
		g_renderUtils->fullscreenQuad(simpleCopy);

		blurShader.bind();
		blurShader.setUniform1i("tex", 0);

		for (int i = 0; i < iterations; i++)
		{
			//now blur horizontally from downscale3 to downscale 3_2
			g_renderTargets->bindTexture("downscale3_2");
			g_textures->bindTexture("downscale3", GL_TEXTURE0);
			blurShader.setUniform1i("horizontal", 1);
			blurShader.setUniform1f("blurscale", spreadX);
			g_renderUtils->fullscreenQuad(blurShader);

			//now blur vertically from downscale3_2 to downscale 3
			g_renderTargets->bindTexture("downscale3");
			g_textures->bindTexture("downscale3_2", GL_TEXTURE0);
			blurShader.setUniform1i("horizontal", 0);
			blurShader.setUniform1f("blurscale", spreadY);
			g_renderUtils->fullscreenQuad(blurShader);
		}

		//finally combine them with the original 
		g_renderTargets->bindTexture(target);
		g_textures->bindTexture(source, GL_TEXTURE0);
		g_textures->bindTexture("downscale3", GL_TEXTURE1);
		combine.bind();
		combine.setUniform1i("tex", 0);
		combine.setUniform1i("tex2", 1);
		combine.setUniform1f("amount", alpha);
		g_renderUtils->fullscreenQuad(source, combine);

		glDepthMask(GL_TRUE);


	}
	std::string Glow::getName()
	{
		return "glow";
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RadialGlow
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RadialGlow::RadialGlow()
	{

	}

	RadialGlow::~RadialGlow()
	{

	}

	void RadialGlow::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		//take source rendertarget
		//downscale it a few times
		//blur radially
		//add the result on top of the source rendertarget

		const int iterations = std::get<int>(params["iterations"]);
		const float spread = std::get<float>(params["spread"]);
		const float exponent = std::get<float>(params["exponent"]);
		const float alpha = std::get<float>(params["alpha"]);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		Shader& simpleCopy = g_shaders->getShader("simpletexture");
		Shader& expShader = g_shaders->getShader("postprocess_exp");
		Shader& blurShader = g_shaders->getShader("postprocess_blur_radial");
		Shader& combine = g_shaders->getShader("postprocess_add");

		//downscale and blit source to downscale1
		g_renderTargets->bindTexture("downscale1");
		simpleCopy.bind();
		simpleCopy.setUniform1i("tex", 0);
		g_textures->bindTexture(source, GL_TEXTURE0); //tästä lähtee
		g_renderUtils->fullscreenQuad(simpleCopy);

		//downscale and use exp filter to downscale 2
		g_renderTargets->bindTexture("downscale2");
		expShader.bind();
		g_textures->bindTexture("downscale1", GL_TEXTURE0);
		expShader.setUniform1i("tex", 0);
		expShader.setUniform1f("expfactor", exponent);
		g_renderUtils->fullscreenQuad(expShader);

		//downscale and blit downcale2 to downscale3
		g_renderTargets->bindTexture("downscale3");
		simpleCopy.bind();
		g_textures->bindTexture("downscale2", GL_TEXTURE0);
		simpleCopy.setUniform1i("tex", 0);
		g_renderUtils->fullscreenQuad(simpleCopy);

		blurShader.bind();
		blurShader.setUniform1i("tex", 0);

		for (int i = 0; i < iterations; i++)
		{
			//now blur horizontally from downscale3 to downscale 3_2
			g_renderTargets->bindTexture("downscale3_2");
			g_textures->bindTexture("downscale3", GL_TEXTURE0);
			blurShader.setUniform1f("blurscale", spread * (1.f + i));
			g_renderUtils->fullscreenQuad(blurShader);

			//now blur vertically from downscale3_2 to downscale 3
			g_renderTargets->bindTexture("downscale3");
			g_textures->bindTexture("downscale3_2", GL_TEXTURE0);
			blurShader.setUniform1f("blurscale", spread * (1.f + i));
			g_renderUtils->fullscreenQuad(blurShader);
		}

		//finally combine them with the original 
		g_renderTargets->bindTexture(target);
		g_textures->bindTexture(source, GL_TEXTURE0);
		g_textures->bindTexture("downscale3", GL_TEXTURE1);
		combine.bind();
		combine.setUniform1i("tex", 0);
		combine.setUniform1i("tex2", 1);
		combine.setUniform1f("amount", alpha);
		g_renderUtils->fullscreenQuad(source, combine);

		glDepthMask(GL_TRUE);


	}
	std::string RadialGlow::getName()
	{
		return "radialglow";
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// OldschoolRadialBlur
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	OldschoolRadialBlur::OldschoolRadialBlur()
	{

	}

	OldschoolRadialBlur::~OldschoolRadialBlur()
	{

	}

	void OldschoolRadialBlur::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		Shader& shader = g_shaders->getShader("postprocess_radial");
		shader.bind();

		glActiveTexture(GL_TEXTURE0);

		shader.setUniform1i("tex", 0);
		g_renderTargets->bindTexture(target);
		g_renderUtils->fullscreenQuad(source, shader);


	}
	std::string OldschoolRadialBlur::getName()
	{
		return "radial";
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SSAO
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SSAO::SSAO()
	{

	}

	SSAO::~SSAO()
	{

	}

	void SSAO::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		Shader& shader = g_shaders->getShader("postprocess_ssao");
		shader.bind();


		shader.setUniform1i("tex", 0);
		GLuint depthTextureId = g_renderTargets->getDepthTextureId("main");
		//g_debug << "depthtextureid = " << depthTextureId << std::endl;
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTextureId);

		glActiveTexture(GL_TEXTURE0);

		float nearValue = std::get<float>(params["near"]);
		float farValue = std::get<float>(params["far"]);
		shader.setUniform1i("depthtex", 1);
		shader.setUniform1f("near", nearValue);
		shader.setUniform1f("far", farValue);

		g_debug << "near = " << nearValue << " far = " << farValue << std::endl;
		g_renderTargets->bindTexture(target);
		g_renderUtils->fullscreenQuad(source, shader);
	}

	std::string SSAO::getName()
	{
		return "SSAO";
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DOF
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	DOF::DOF()
	{

	}

	DOF::~DOF()
	{

	}

	void DOF::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		Shader& shader = g_shaders->getShader("postprocess_dof");
		shader.bind();

		shader.setUniform1i("tex", 0);
		GLuint depthTextureId = g_renderTargets->getDepthTextureId("main");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTextureId);
		glActiveTexture(GL_TEXTURE0);

		shader.setUniform1i("depthtex", 1);
		shader.setUniform1f("near", std::get<float>(params["near"]));
		shader.setUniform1f("far", std::get<float>(params["far"]));
		shader.setUniform1f("focus", std::get<float>(params["focus"]));

		g_renderTargets->bindTexture(target);
		g_renderUtils->fullscreenQuad(source, shader);
	}

	std::string DOF::getName()
	{
		return "DOF";
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Lens
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Lens::Lens()
	{

	}

	Lens::~Lens()
	{

	}

	void Lens::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		GLuint depthTextureId = g_renderTargets->getDepthTextureId("main");
/*
		1. focus pass
			- laske CoC per pikseli omaan bufferiinsa
			- eli kopioi main rendertargetista rgb väri ja laske Coc w - komponenttiin
*/
		Shader& cocShader= g_shaders->getShader("postprocess_lens_coc");
		cocShader.bind();

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTextureId);
		glActiveTexture(GL_TEXTURE0);
		g_textures->bindTexture(source);

		cocShader.setUniform1i("colorTex", 0);
		cocShader.setUniform1i("depthTex", 1);
		cocShader.setUniform1f("nearPlane", std::get<float>(params["near"]));
		cocShader.setUniform1f("farPlane", std::get<float>(params["far"]));
		cocShader.setUniform1f("focus", std::get<float>(params["focus"]));

		g_renderTargets->bindTexture("lens_coc");
		g_renderUtils->fullscreenQuad(cocShader); //->lens_coc rendertargettiin

/*
		2. blur pass
			- käytä em.passissa laskettua arvoa ja blurraa
*/
		Shader& blurShader = g_shaders->getShader("postprocess_lens_blur");
		blurShader.bind();

		blurShader.setUniform1i("cocTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		g_textures->bindTexture("lens_coc", GL_TEXTURE0);

		g_renderTargets->bindTexture(target);
		g_renderUtils->fullscreenQuad(blurShader);

	}

	std::string Lens::getName()
	{
		return "Lens";
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Test
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TestEffect::TestEffect()
	{

	}

	TestEffect::~TestEffect()
	{

	}

	void TestEffect::apply(const std::string& source, const std::string& target, PostProcessParameters& params)
	{
		//take source rendertarget
		//downscale it a few times
		//blur horizontally and vertically
		//add the result on top of the source rendertarget


		const int iterations = std::get<int>(params["iterations"]);
		const float spreadX = std::get<float>(params["spreadX"]);
		const float spreadY = std::get<float>(params["spreadY"]);
		const float exponent = std::get<float>(params["exponent"]);
		const float alpha = std::get<float>(params["alpha"]);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(0);

		Shader& simpleCopy = g_shaders->getShader("simpletexture");
		//		Shader& expShader = g_shaders->getShader("postprocess_exp");
		//		Shader& horizontalBlur = g_shaders->getShader("postprocess_blurx");
		//		Shader& verticalBlur = g_shaders->getShader("postprocess_blury");
		Shader& combine = g_shaders->getShader("postprocess_add");

		//downscale and blit source to downscale1
		g_renderTargets->bindTexture("downscale1");
		simpleCopy.bind();
		simpleCopy.setUniform1i("tex", 0);
		g_textures->bindTexture("testi.png", GL_TEXTURE0); //tästä lähtee
		g_renderUtils->fullscreenQuad(simpleCopy);

		//downscale and use exp filter to downscale 2
		g_renderTargets->bindTexture("downscale2");
		g_textures->bindTexture("downscale1", GL_TEXTURE0);
		simpleCopy.setUniform1i("tex", 0);
		g_renderUtils->fullscreenQuad(simpleCopy);

		//downscale and blit downcale2 to downscale3
		g_renderTargets->bindTexture("downscale3");
		g_textures->bindTexture("downscale2", GL_TEXTURE0);
		simpleCopy.setUniform1i("tex", 0);
		g_renderUtils->fullscreenQuad(simpleCopy);

		g_renderTargets->bindTexture("downscale3_2");
		g_textures->bindTexture("downscale3", GL_TEXTURE0);
		simpleCopy.setUniform1i("tex", 0);
		g_renderUtils->fullscreenQuad(simpleCopy);

		//finally combine them with the original 
		g_renderTargets->bindTexture(target);
		g_textures->bindTexture(source, GL_TEXTURE0);
		g_textures->bindTexture("downscale3", GL_TEXTURE1);
		combine.bind();
		combine.setUniform1i("tex", 0);
		combine.setUniform1i("tex2", 1);
		combine.setUniform1f("amount", alpha);
		g_renderUtils->fullscreenQuad(source, combine);

	}
	std::string TestEffect::getName()
	{
		return "test";
	}
}