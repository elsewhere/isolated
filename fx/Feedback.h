#pragma once

#include "../Globals.h"

namespace demofx
{
	class Feedback
	{

	public:
		Feedback(const std::string& shader, int width = 1024, int height = 1024);
		~Feedback();

		void startFrame();
		void endFrame();

		void draw();

	private:
		struct FeedbackBuffer
		{
			std::unique_ptr<demorender::Texture> texture;
		};

		FeedbackBuffer m_buffer1;
		FeedbackBuffer m_buffer2;

		int m_textureWidth;
		int m_textureHeight;

		std::string m_shader = "no shader";
		bool m_flip = false;

	};

}