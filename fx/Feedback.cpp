#include "Feedback.h"

using namespace demorender;
using namespace democore;

namespace demofx
{
	Feedback::Feedback(const std::string& shader, int width, int height):
		m_textureWidth(width),
		m_textureHeight(height),
		m_shader(shader),
		m_flip(false)
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
	}

	void Feedback::endFrame()
	{
		//do actual feedback here

		m_flip = !m_flip;
	}

	void Feedback::draw()
	{

	}
}