#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN           

//#define GLEW_STATIC

#include "Globals.h"
/*
#include "fx_test/sigiltest.h"
#include "fx_test/test.h"
#include "fx_test/lightingtest.h"
#include "fx_test/postprocesstesti.h"
#include "fx_test/postprocesstesti2.h"
#include "fx_test/cubemaptesti.h"
#include "fx_test/cubemaptesti2.h"
#include "fx_test/particletesti2.h"
#include "fx_test/particletesti3.h"
#include "scenes/huuru.h"

*/
//#include "fx_test/computetest.h"
//#include "fx_test/computetest2.h"

//#include "fx_test/postprocesstesti2.h"

//#include "scenes/fjara.h"
//#include "scenes/midaftann.h"
//#include "scenes/rivers.h"
//#include "scenes/mountain.h"


#include "scenes/ikuisuus.h"


democore::Debugger g_debug;
democore::ErrorLog g_error;
democore::ErrorLog g_screenText;

glm::vec3 g_clearColor = glm::vec3(0, 0, 0);

const std::string g_prodName = "Isafjördur";

using namespace democore;
using namespace demorender;

/*
	demo:
		- alussa piiirtyy vaakasuora viiva, jonka ylle tulee demon nimi in our dreams we were birds once
		- kamera kääntyy ja viiva onkin lasia
		- lasin päälle valuu mustaa öljyä
			- shaderissa specular + ehkä kromaattinen aberraatio
			- valo tulee ylhäältä 
		- lasiin ilmaantuu säröjä
		- öljyä pääsee läpi, se tippuu pisaroina alaspäin
		- kamera kääntyy ja pisarat tippuu äärettömän kokoiseen vesialtaaseen
		- altaasta nousee lintu
		- valaistus muuttuu ja kamera kääntyy ylöspäin
		- taivaalla on auringonpimennys, jonka reunat savuavat
		- linnut lentävät kohti auringonpimennystä

		in our dreams we were birds once
		the light

*/

int APIENTRY WinMain(HINSTANCE hInstance,

                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg = {0};

	//first initialize system
	g_system->init();
	//and then the demo
	std::string scriptfile("data/script.txt");

	DemoContext *context = 0;

	context = new Demo(CONTEXT_TYPE_DEMO, scriptfile);
	//all ok? 
    if (context->initOk())
    {
		g_error.setColor(1.f, 0.f, 0.f);
		g_screenText.setColor(1.f, 1.f, 1.f);
		g_textures->init();
		g_textures->loadImages();
		g_meshes->init();
		g_meshes->loadMeshes();

		Math::init();
		g_postProcess->init();
/*
		static_cast<Demo*>(context)->addScene("sigiltest", new SigilTest());
		static_cast<Demo*>(context)->addScene("test", new Test());
		static_cast<Demo*>(context)->addScene("lightingtest", new LightingTest());
		static_cast<Demo*>(context)->addScene("huuru", new Huuru());
		static_cast<Demo*>(context)->addScene("postprocesstesti", new PostProcessTesti());
		static_cast<Demo*>(context)->addScene("cubemaptesti", new CubemapTesti());
		static_cast<Demo*>(context)->addScene("cubemaptesti2", new CubemapTesti2());
		static_cast<Demo*>(context)->addScene("particletesti2", new ParticleTesti2());
		static_cast<Demo*>(context)->addScene("postprocesstesti2", new PostProcessTesti2());
		static_cast<Demo*>(context)->addScene("particletesti2", new ParticleTesti2());
		static_cast<Demo*>(context)->addScene("particletesti3", new ParticleTesti3());
*/
//		static_cast<Demo*>(context)->addScene("postprocesstesti2", new PostProcessTesti2());
//		static_cast<Demo*>(context)->addScene("computetest", new ComputeTest());
//		static_cast<Demo*>(context)->addScene("computetest2", new ComputeTest2());

//		static_cast<Demo*>(context)->addScene("rivers", new Rivers());
//		static_cast<Demo*>(context)->addScene("mountain", new Mountain());
//		static_cast<Demo*>(context)->addScene("fjara", new Fjara());
//		static_cast<Demo*>(context)->addScene("midaftann", new Midaftann());
		static_cast<Demo*>(context)->addScene("ikuisuus", new Ikuisuus());

		//init the prodand play
		g_textures->createTextures();
		g_shaders->loadShaders();

		context->init();
        context->start();

		demorender::Buffer::debugPrint();

		bool running = true;
        while (running)
        {
			g_renderDebug->resetGL();
			g_screenText.clear();
			g_system->update();
			g_sync->update();

			std::stringstream ss;
			if (g_system->getDeveloperMode())
			{
				ss << "time = " << g_system->getTime() << " fps = " << g_system->getFPS();
				glfwSetWindowTitle(g_system->getWindow(), ss.str().c_str());
			}
			else
			{
				glfwSetWindowTitle(g_system->getWindow(), g_prodName.c_str());
			}

			running = context->isRunning();
			context->update();

			RenderStatistics::clear();

			context->draw();


			if (g_system->getDeveloperMode())
			{
				g_error.draw();
				if (g_error.getBuffer().empty())
				{
					g_screenText.draw();
				}
			}



/*
			g_renderDebug->drawTexture("post1", 0, 0, 256.f);
			g_renderDebug->drawTexture("post2", 256, 0, 256.f);
			g_renderDebug->drawTexture("downscale1", 512, 0, 256.f);
			g_renderDebug->drawTexture("downscale2", 768, 0, 256.f);
			g_renderDebug->drawTexture("downscale3", 1024, 0, 256.f);
			g_renderDebug->drawTexture("downscale3_2", 1280, 0, 256.f);
*/
			g_system->swapBuffers();
			if (g_system->exitRequested() || context->isExitRequested())
				running = false;
        }
        context->stop();
		demorender::g_textures->dumpUnusedImages();
		demorender::g_shaders->dumpUnusedShaders();
    }

    delete context;
    return msg.wParam;
}



