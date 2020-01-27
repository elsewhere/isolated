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


#include "scenes/tyhjyys.h"
#include "scenes/kasvot.h"
#include "scenes/tunne.h"
#include "scenes/kalpeus.h"
#include "scenes/shadowtest.h"

democore::Debugger g_debug;
democore::Profiler g_profiler;
democore::ErrorLog g_error;
democore::ErrorLog g_screenText;

glm::vec3 g_clearColor = glm::vec3(0, 0, 0);

const std::string g_prodName = "Isafjördur";

using namespace democore;
using namespace demorender;

/*
	demo:
		viisi skeneä, pentagrammin muotoinen symboli (ympyrä pentagrammin kärjissä)

		skene yksi - musta tausta, hohtava valo, kohinaa. Fokus. 
		skene kaksi - valkoinen pohja ja musta valo (invertoitu), maassa ympyrä sen ympärillä, ehkä fft. Repivä tyhjyys. Raivo.
		skene kolme - Auringonpimennys. Logiikka ja rationaalinen mieli.
		skene neljä - Jokin asia, joka hajoaa miljoonaksi pieneksi asiaksi. Katharsis. 
		skene viisi - symboli 




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

		g_renderDebug->init();

		Math::init();
		g_postProcess->init();
//		static_cast<Demo*>(context)->addScene("tyhjyys", new Tyhjyys());
//		static_cast<Demo*>(context)->addScene("kasvot", new Kasvot());
//		static_cast<Demo*>(context)->addScene("tunne", new Tunne());
//		static_cast<Demo*>(context)->addScene("kalpeus", new Kalpeus());
		static_cast<Demo*>(context)->addScene("shadowtest", new ShadowTest());

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



