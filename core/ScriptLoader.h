#pragma once

#include "../Globals.h"

/*
    Parser that reads the script file and does the appropriate stuff.
*/

namespace demorender
{
	class MeshInfo;
	class Texture;
	class TextureParameters;
	class Image;
	class ImageFactory;
}

namespace democore
{

	class ScriptLoader
	{
	public:
		ScriptLoader();
		~ScriptLoader();

		//parses a script and inserts it into params
		bool load(const std::string& filename, DemoContext *context);

	private:
		// methods to read the file into a buffer and access the buffer
		bool readFile(const std::string& filename);
		bool getNextLine(std::vector<std::string>& tokens, bool lowercase = true);


		void skipThisBlock();

		std::vector<std::pair<int, std::string>> m_file;
		int m_currentLineIndex;
		int m_totalLineCount;

		void error(const std::string& msg);

		//parse methods for individual blocks
		bool parseDemoConfig(System &params, Demo* demo);
		bool parseNamespace(Parameters &params);
		bool parseTexture(demorender::TextureParameters &params);
		bool parseRenderTarget(demorender::Image &image, bool createImage);

		//demo specific
		bool parseTimeline(Demo &demo);
		bool parseBPM(BPM &beat);
		bool parseTrigger(TriggerSystem &triggers);
		bool parseEvent(Event &e);
		
		int m_line;
		std::string m_currentnamespace;

		//converts 
		glm::vec3 convertToVector3(const std::string& token1, const std::string& token2, const std::string& token3);

	};

}