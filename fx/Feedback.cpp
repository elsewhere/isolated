#include "Feedback.h"

using namespace demorender;
using namespace democore;

namespace demofx
{
	Feedback::Feedback(const std::string& shader, int width, int height):
		m_textureWidth(width),
		m_textureHeight(height),
		m_shader(shader),
		m_timer(0.f),
		m_flip(false),
		m_update(false)
	{
		demorender::TextureParameters params;
		params.m_width = width;
		params.m_height = height;
		params.m_linear = true;
		params.m_repeat = false;

		m_buffer1.texture = std::make_unique<demorender::Texture>();
		m_buffer2.texture = std::make_unique<demorender::Texture>();

		m_buffer1.texture->create(params, nullptr);
		m_buffer2.texture->create(params, nullptr);
	}

	Feedback::~Feedback()
	{
	}

	void Feedback::startFrame()
	{
		if (m_flip)
		{
			g_renderTargets->bindTexture(m_buffer2.texture.get(), "rendertarget texture 2");
		}
		else
		{
			g_renderTargets->bindTexture(m_buffer1.texture.get(), "rendertarget texture 1");
		}
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Feedback::endFrame()
	{
		//do actual feedback here if needed

		if (m_update)
		{
			m_flip = !m_flip;
		}
	}

	void Feedback::update()
	{
		const float FEEDBACK_FRAMERATE = 30.f;
		const float STEP = 100.f / FEEDBACK_FRAMERATE; //engine updates 100fps

		m_timer += 0.01f;

		if (m_timer > STEP)
		{
			m_update = true;
			m_timer -= STEP;
		}
	}

	void Feedback::draw()
	{
		//draw the feedback on top of the original 

		Shader& shader = g_shaders->getShader("feedback_draw");

		g_textures->bindTexture(m_flip ? m_buffer2.texture->getID() : m_buffer1.texture->getID());
		
		float alpha = 0.3f;

		shader.bind();
		shader.setUniform1f("alpha", alpha);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g_renderUtils->fullscreenQuad(shader);
		glDisable(GL_BLEND);


	}
}