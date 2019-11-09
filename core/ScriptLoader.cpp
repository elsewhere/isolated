#include <string>
#include <algorithm>
#include <string>

#include "../render/Texture.h"
#include "ScriptLoader.h"

using namespace demorender;
using namespace demomath;

namespace democore
{

	ScriptLoader::ScriptLoader()
	{
	}

	ScriptLoader::~ScriptLoader()
	{
	}

	//prints out an error message
	void ScriptLoader::error(const std::string& msg)
	{
		//the getNextLine() has always been called before this is, so it's safe to use the -1 indexing
		g_debug << "!! parse error on line(" << m_file[m_currentLineIndex-1].first << ") : " << msg << std::endl;
	}

	bool ScriptLoader::readFile(const std::string& filename)
	{
		std::ifstream script(filename.c_str(), std::ios::in);
		int linenumber = 0;

		if (script.is_open())
		{
			//clear out old file
			m_file.clear();
			bool comment = false;
			
			//read in new file
			while (!script.eof())
			{
				std::string line;
				getline(script, line);
				linenumber++;

				//skip empty lines
				if (!line.empty())
				{
					//check for a comment block
					if (line.length() > 1)
					{
						if (line[0] == '/' && line[1] == '*') //comment starts
						{
							comment = true;
							continue; //go to next line without processing
						}
						else if (line[0] == '*' && line[1] == '/') //comment ends
						{
							comment = false;
							continue; //go to next line without processing
						}
					}

					//check that the line is not a single comment or inside a comment block
					if (!comment && (line[0] != ';' && line[0] != '#'))
					{
						//convert it into lowercase
//						transform(line.begin(), line.end(), line.begin(), tolower);
						//remove all crap that the programmer might put there by custom
						StringUtils::removeChar(line, ';');
						StringUtils::removeChar(line, '\t'); //remove tabs

						//TODO: check for lines that are only whitespace
						//push the processed line into the file
						if (line.size() > 0)
						{
							m_file.push_back(std::make_pair(linenumber, line));
						}
					}
				}
			}
			m_totalLineCount = (int)m_file.size();
	//		g_debug << "linenumber = " << linenumber << " totalLineCount = " << m_totalLineCount << std::endl;
		}
		else
		{
			g_debug << "ScriptLoader::readFile() - cannot open file " << filename << " for reading!" << std::endl;
			return false;
		}
		script.close(); 

		return true;
	}

	bool ScriptLoader::getNextLine(std::vector<std::string>& tokens, bool lowercase)
	{
		if (m_currentLineIndex < m_totalLineCount)
		{
			//fetch the next line and split it into tokens. The tokenization is done here because it would
			//convolute the data structure further if it was done during loading. It's as fast anyway.. 
			tokens.clear();
			std::string& line = m_file[m_currentLineIndex].second;
			if (lowercase)
			{
				std::transform(line.begin(), line.end(), line.begin(), tolower);
			}

			StringUtils::tokenize(line, tokens);
			m_currentLineIndex++;
	//		g_debug << "getNextLine : " << line << std::endl;
			return true;
		}
		else
		{
			//we're done
			return false;
		}
	}
	bool ScriptLoader::parseNamespace(Parameters &params)
	{
		std::vector<std::string> tokens;

		getNextLine(tokens);
		if (tokens[0] != "{")
		{
			error("namespace must start with {");
			return false;
		}

		//read until the closing curly brace
		while (true)
		{
			tokens.clear();
			getNextLine(tokens);
			//break on ending curly brace
			if (tokens[0] == "}")
			{
				break;
			}
			//check for the equal sign that is in every namespace value
			if (tokens[2] != "=")
			{
				error("syntax error");
			}
			//read in all kinds of values
			if (tokens[0] == "float")
			{
				float value = 0.0f;
	        
				if (StringUtils::convertStringToNumber<float>(value, tokens[3], std::dec))
				{
					if (m_currentnamespace != "")
						params.addFloat(m_currentnamespace, tokens[1], value);
					else
						error("trying to add parameter to an empty namespace!");
				}
				else
				{
					error("cannot parse float");
				}
			}
			else if (tokens[0] == "int")
			{
				int value = 0;
	        
				if (StringUtils::convertStringToNumber<int>(value, tokens[3], std::dec))
				{
					if (m_currentnamespace != "")
						params.addInt(m_currentnamespace, tokens[1], value);
					else
						error("trying to add parameter to an empty namespace!");
				}
				else
				{
					error("cannot parse int");
				}
			}
			else if (tokens[0] == "bool")
			{
				int value = 0;
	        
				if (m_currentnamespace != "")
				{
					if (tokens[3] == "true")
					{
						params.addBool(m_currentnamespace, tokens[1], true);
					}
					else if (tokens[3] == "false")
					{
						params.addBool(m_currentnamespace, tokens[1], false);
					}
					else
					{
						error("cannot parse bool");
					}
				}
				else
				{
					error("trying to add parameter to an empty namespace!");
				}
			}
			else if (tokens[0] == "blendmode")
			{
				int value = 0;

				if (m_currentnamespace != "")
				{
					if (tokens[3] == "gl_zero")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_ZERO);
					else if (tokens[3] == "gl_one")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE);
					else if (tokens[3] == "gl_dst_color")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_DST_COLOR);
					else if (tokens[3] == "gl_one_minus_dst_color")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE_MINUS_DST_COLOR);
					else if (tokens[3] == "gl_src_alpha")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_SRC_ALPHA);
					else if (tokens[3] == "gl_one_minus_src_alpha")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE_MINUS_SRC_ALPHA);
					else if (tokens[3] == "gl_one_minus_src_color")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE_MINUS_SRC_COLOR);
					else if (tokens[3] == "gl_dst_alpha")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_DST_ALPHA);
					else if (tokens[3] == "gl_one_minus_dst_alpha")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE_MINUS_DST_ALPHA);
					else if (tokens[3] == "gl_src_alpha_saturate")
						params.addBlendMode(m_currentnamespace, tokens[1], GL_SRC_ALPHA_SATURATE);
					else
						error("cannot parse blendmode");
				}
				else
				{
					error("trying to add parameter to an empty namespace!");
				}
			}
			else if (tokens[0] == "vector3" || tokens[0] == "vec3")
			{
				float x, y, z;
				if (StringUtils::convertStringToNumber<float>(x, tokens[3], std::dec) &&
					StringUtils::convertStringToNumber<float>(y, tokens[4], std::dec) &&
					StringUtils::convertStringToNumber<float>(z, tokens[5], std::dec))
				{
					glm::vec3 v = glm::vec3(x, y, z);
					if (m_currentnamespace != "")
						params.addVector3(m_currentnamespace, tokens[1], v);
					else
						error("trying to add parameter to an empty namespace!");
				}
				else
				{
					error("cannot parse vector3");
				}
			}
			else if (tokens[0] == "range")
			{
				float minimum, maximum;
				if (StringUtils::convertStringToNumber<float>(minimum, tokens[3], std::dec) &&
					StringUtils::convertStringToNumber<float>(maximum, tokens[4], std::dec))
				{
					Range r = Range(minimum, maximum);
					if (m_currentnamespace != "")
						params.addRange(m_currentnamespace, tokens[1], r);
					else
						error("trying to add parameter to an empty namespace!");
				}
				else
				{
					error("cannot parse range");
				}
			}
			else if (tokens[0] == "string")
			{
				if (m_currentnamespace != "")
				{
					std::string str = tokens[3];

					//check for multiple words. They will be separated with space.
					bool multiple = false;
					for (auto it = tokens.begin() + 4; it < tokens.end(); it++)
					{
						if (!multiple)
						{
							str += " "; //add space after the first word that was stripped while tokenization. Ugly.. 
							multiple = true;
						}
						str += *it;
						str += " ";
					}
					//take out the last space (yes, this is kind of hackish but..)
					if (multiple)
					{
						str = str.substr(0, str.size() -1);
					}

					params.addString(m_currentnamespace, tokens[1], str);
				}
				else
					error("trying to add parameter to an empty namespace!");
			}
			else
			{
				std::string err = std::string("unknown symbol \"")+tokens[0]+std::string("\"");
				error(err.c_str());
			}
		}

		return true;
	}
	glm::vec3 ScriptLoader::convertToVector3(const std::string& token1, const std::string& token2, const std::string& token3)
	{
		float x, y, z;
		if (StringUtils::convertStringToNumber<float>(x, token1, std::dec) &&
			StringUtils::convertStringToNumber<float>(y, token2, std::dec) &&
			StringUtils::convertStringToNumber<float>(z, token3, std::dec))
		{
			glm::vec3 v = glm::vec3(x, y, z);
			return v;
		}
		else
		{
			g_debug << "cannot parse {" << token1 << "," << token2 << "," << token3 << "} into a vec3!" << std::endl;
			return glm::vec3(0, 0, 0);
		}
	}


	bool ScriptLoader::parseDemoConfig(System &system, Demo *demo)
	{
		std::vector<std::string> tokens;

		getNextLine(tokens);
		if (tokens[0] != "{")
		{
			error("config must start with {");
			return false;
		}

		//read until the closing curly brace
		while (true)
		{
			tokens.clear();
			getNextLine(tokens);
			if (tokens[0] == "}")
			{
				break;
			}
			if (tokens[1] != "=")
			{
				error("syntax error");
				return false;
			}

			//read in values
			if (tokens[0] == "sound")
			{
				if (tokens[2] == "true")
				{
					system.setSoundEnabled(true);
				}
				else if (tokens[2] == "false")
				{
					system.setSoundEnabled(false);
				}
				else
				{
					error("cannot parse sound");
				}
			}
			else if (tokens[0] == "developermode")
			{
				if (tokens[2] == "true")
				{
					system.setDeveloperMode(true);
				}
				else if (tokens[2] == "false")
				{
					system.setDeveloperMode(false);
				}
				else
				{
					error("cannot parse developermode");
				}
			}
			else if (tokens[0] == "song")
			{
				system.setSongFilename(tokens[2]);
			}
			else if (tokens[0] == "endtime")
			{
				int time = 100;
				if (!StringUtils::convertStringToNumber<int>(time, tokens[2], std::dec))
				{
					error("cannot parse endtime number");
					time = 100; //default value
				}
				demo->setEndTime(time);
//				system.setEndTime(time);
			}
			else if (tokens[0] == "jumptime")
			{
				int time = 100;
				if (!StringUtils::convertStringToNumber<int>(time, tokens[2], std::dec))
				{
					error("cannot parse jumptime number");
					time = 100; //default value
				}
				system.setJumpTime(time);
			}
			else
			{
				std::string err = std::string("unknown symbol \"")+tokens[0]+std::string("\"");
				error(err.c_str());
			}
		}
		return true;
	}

	bool ScriptLoader::parseTimeline(Demo& demo)
	{
		std::vector<std::string> tokens;

		getNextLine(tokens);
		if (tokens[0] != "{")
		{
			error("timeline must start with {");
			return false;
		}

		//read until the closing curly brace
		while (true)
		{
			tokens.clear();
			getNextLine(tokens);
			if (tokens[0] == "}")
			{
				break;
			}
/*
			g_debug << "tokens[0] = " << tokens[0] << std::endl;
			g_debug << "tokens[1] = " << tokens[1] << std::endl;
			g_debug << "tokens[2] = " << tokens[2] << std::endl;
			g_debug << "tokens[3] = " << tokens[3] << std::endl;
*/
			std::string name = tokens[0];
			int starttime, endtime, priority;

			if (!StringUtils::convertStringToNumber<int>(starttime, tokens[1], std::dec))
				error("cannot parse timeline entry start time");
			if (!StringUtils::convertStringToNumber<int>(endtime, tokens[2], std::dec))
				error("cannot parse timeline entry end time");
			if (!StringUtils::convertStringToNumber<int>(priority, tokens[1], std::dec))
				error("cannot parse timeline entry priority");
			
			demo.addSceneToTimeline(name, starttime, endtime, priority);
		}
		return true;
	}

	bool ScriptLoader::parseTexture(demorender::TextureParameters &params)
	{
		std::vector<std::string> tokens;

		getNextLine(tokens);
		if (tokens[0] != "{")
		{
			error("texture must start with {");
			return false;
		}

		params.resetDefaults();

		//read until the closing curly brace
		while (true)
		{
			tokens.clear();
			getNextLine(tokens);
			if (tokens[0] == "}")
			{
				break;
			}
			if (tokens[1] != "=")
			{
				error("syntax error");
				return false;
			}
			if (tokens[0][0] == ';')
				continue;
			
			if (tokens.size() < 3)
			{
				error("not enough parameters!");
				return false;
			}

			//read in values
			if (tokens[0] == "upload")
			{
				if (tokens[2] == "true")
					params.m_upload = true;
				else if (tokens[2] == "false")
					params.m_upload = false;
				else
					error("cannot parse texture upload");
			}
			else if (tokens[0] == "retain")
			{
				if (tokens[2] == "true")
					params.m_retain = true;
				else if (tokens[2] == "false")
					params.m_retain = false;
				else
					error("cannot parse texture retain");
			}
			else if (tokens[0] == "filter")
			{
				if (tokens[2] == "linear")
					params.m_linear = true;
				else if (tokens[2] == "nearest")
					params.m_linear = false;
				else
					error("cannot parse texture filter");
			}
			else if (tokens[0] == "wrap")
			{
				if (tokens[2] == "repeat")
					params.m_repeat = true;
				else if (tokens[2] == "clamp")
					params.m_repeat = false;
				else
					error("cannot parse texture wrap mode");
			}
			else if (tokens[0] == "mipmap")
			{
				if (tokens[2] == "true")
					params.m_mipmap = true;
				else if (tokens[2] == "false")
					params.m_mipmap = false;
				else
					error("cannot parse texture mipmap ");
			}
			else if (tokens[0] == "format")
			{
				if (tokens[2] == "rgba")
					params.m_format = demorender::TextureParameters::RGBA;
				else if (tokens[2] == "float16")
					params.m_format = demorender::TextureParameters::FLOAT16;
				else if (tokens[2] == "float32")
					params.m_format = demorender::TextureParameters::FLOAT32;
				else
					error("cannot parse texture format");
			}
			else if (tokens[0] == "type")
			{
				if (tokens[2] == "cubemap")
					params.m_type = demorender::TextureParameters::CUBEMAP;
				else if (tokens[2] == "texture")
					params.m_type = demorender::TextureParameters::TEXTURE_2D;
				else
					error("cannot parse texture type");
			}
			else if (tokens[0] == "width")
			{
				int width = 0;
				if (!StringUtils::convertStringToNumber<int>(width, tokens[2], std::dec))
					error("cannot parse rendertarget width!");

				params.m_width = width;
			}
			else if (tokens[0] == "height")
			{
				int height = 0;
				if (!StringUtils::convertStringToNumber<int>(height, tokens[2], std::dec))
					error("cannot parse rendertarget height!");

				params.m_height = height;
			}
			else if (tokens[0] == "filename")
			{
				params.m_filename = tokens[2];
			}
			else if (tokens[0] == "filenameposx")
			{
				params.m_filenamePosX = tokens[2];
			}
			else if (tokens[0] == "filenameposy")
			{
				params.m_filenamePosY = tokens[2];
			}
			else if (tokens[0] == "filenameposz")
			{
				params.m_filenamePosZ = tokens[2];
			}
			else if (tokens[0] == "filenamenegx")
			{
				params.m_filenameNegX = tokens[2];
			}
			else if (tokens[0] == "filenamenegy")
			{
				params.m_filenameNegY = tokens[2];
			}
			else if (tokens[0] == "filenamenegz")
			{
				params.m_filenameNegZ = tokens[2];
			}
			else
			{
				std::string err = std::string("unknown symbol \"")+tokens[0]+ std::string("\"");
				error(err.c_str());
			}
		}

		return true;
	}

	bool ScriptLoader::parseRenderTarget(demorender::Image &image, bool createImage)
	{
		std::vector<std::string> tokens;

		int width = 0;
		int height = 0;

		getNextLine(tokens);
		if (tokens[0] != "{")
		{
			error("Rendertarget must start with {");
			return false;
		}

		//read until the closing curly brace
		while (true)
		{
			tokens.clear();
			getNextLine(tokens);
			if (tokens[0] == "}")
			{
				break;
			}
			if (tokens[1] != "=")
			{
				error("syntax error");
				return false;
			}

			//read in values
			if (tokens[0] == "width")
			{
				if (!StringUtils::convertStringToNumber<int>(width, tokens[2], std::dec))
					error("cannot parse rendertarget width!");
			}
			else if (tokens[0] == "height")
			{
				if (!StringUtils::convertStringToNumber<int>(height, tokens[2], std::dec))
					error("cannot parse rendertarget height!");
			}
			else if (tokens[0] == "size")
			{
				int size = 0;
				if (!StringUtils::convertStringToNumber<int>(size, tokens[2], std::dec))
				{
					error("cannot parse rendertarget size!");
				}
				else
				{
					if (size < 1)
					{
						error("invalid size for rendertarget!");
						size = 1;
					}
					int screenWidth, screenHeight;
					g_system->getWindowSize(screenWidth, screenHeight);

					width = screenWidth / size;
					height = screenHeight / size;
				}

			}
			else
			{
				std::string err = std::string("unknown symbol \"")+tokens[0]+ std::string("\"");
				error(err.c_str());
			}
		}
		if (height > 0 && width > 0)
		{
			if (createImage)
			{
				demorender::Image *temp = demorender::ImageFactory::createEmpty(width, height);
				image = *temp;
			}
			else if (height != image.getHeight() || width != image.getWidth())
			{
				//need to resize
				//should release memory here but who cares with dev mode...
				demorender::Image *temp = demorender::ImageFactory::createEmpty(width, height);
				image = *temp;
			}
		}
		else
		{
			error("rendertarget definition incomplete!");
		}

		return true;
	}

	bool ScriptLoader::parseTrigger(TriggerSystem &triggers)
	{
		std::vector<std::string> tokens;

		getNextLine(tokens);
		if (tokens[0] != "{")
		{
			error("triggers must start with {");
			return false;
		}

		//read until the closing curly brace
		while (true)
		{
			tokens.clear();
			getNextLine(tokens);
			if (tokens[0] == "}")
			{
				break;
			}
			Trigger trigger;

			if (StringUtils::convertStringToNumber<int>(trigger.m_time, tokens[0], std::dec))
			{
			}
			else
			{
				error("cannot parse trigger time!");
			}

			trigger.m_power = 1.f;

			if (tokens.size() == 3)
			{
				if (StringUtils::convertStringToNumber<float>(trigger.m_power, tokens[2], std::dec))
				{
				}
				else
				{
					error("cannot parse trigger power");
				}
			}

			if (StringUtils::convertStringToNumber<int>(trigger.m_length, tokens[1], std::dec))
			{
			}
			else
			{
				error("cannot parse trigger length!");
			}
			triggers.addTrigger(trigger);
		}
		return true;
	}

	void ScriptLoader::skipThisBlock()
	{
		std::vector<std::string> tokens;
		while (true)
		{
			tokens.clear();
			getNextLine(tokens);
			if (tokens[0] == "}")
			{
				break;
			}
		}
	}
	bool ScriptLoader::parseBPM(BPM &beat)
	{
		std::vector<std::string> tokens;

		float bpm = 0;
		int offset = 0;

		getNextLine(tokens);
		//first non-empty line
		if (tokens[0] != "{")
		{
			error("BPM must start with {");
			return false;
		}

		//read until the closing curly brace
		while (true)
		{
			tokens.clear();
			getNextLine(tokens);
			if (tokens[0] == "}")
			{
				break;
			}
			if (tokens[1] != "=")
			{
				error("syntax error");
				return false;
			}
			//read in values
			if (tokens[0] == "bpm")
			{
				if (StringUtils::convertStringToNumber<float>(bpm, tokens[2], std::dec))
				{
					beat.setBPM(bpm);
				}
				else
				{
					error("cannot parse BPM value!");
				}
			}
			else if (tokens[0] == "offset")
			{
				if (StringUtils::convertStringToNumber<int>(offset, tokens[2], std::dec))
				{
					beat.setOffset(offset);
				}
				else
				{
					error("cannot parse BPM offset!");
				}
			}
			else
			{
				std::string err = std::string("unknown symbol \"")+tokens[0]+ std::string("\"");
				error(err.c_str());
			}
		}
		return true;
	}


	bool ScriptLoader::parseEvent(Event &e)
	{
		std::vector<std::string> tokens;

		int time = 0;
		int length = 0;

		getNextLine(tokens);

		//first non-empty line
		if (tokens[0] != "{")
		{
			error("Event must start with {");
			return false;
		}

		//read until the closing curly brace
		while(true)
		{
			tokens.clear();
			getNextLine(tokens);
			if (tokens[0] == "}")
			{
				break;
			}
			if (tokens[1] != "=")
			{
				error("syntax error");
				return false;
			}

			//read in values
			if (tokens[0] == "time")
			{
				if (StringUtils::convertStringToNumber<int>(time, tokens[2], std::dec))
				{
					e.m_time = time;
					//g_debug << "parsed event time = " << e.m_time << "\n";
				}
				else
				{
					error("cannot parse event time!");
				}
			}
			else if (tokens[0] == "length")
			{
				if (StringUtils::convertStringToNumber<int>(length, tokens[2], std::dec))
				{
					e.m_length = length;
				}
				else
				{
					error("cannot parse event value!");
				}
			}
			else
			{
				std::string err = std::string("unknown symbol \"")+tokens[0]+ std::string("\"");
				error(err.c_str());
			}
		}
		return true;
	}



	bool ScriptLoader::load(const std::string& filename, DemoContext *context)
	{
		g_debug << "loading script " << filename << std::endl;

		if (readFile(filename))
		{
			//file was found and read into the memory, so it can be processed
			m_currentLineIndex = 0;

			g_sync->clear();
			g_system->clear();
			g_params->clear();

			std::vector<std::string> tokens;
			while (getNextLine(tokens, false))
			{
				//the ugly If-Else of Doom...
				//only rudimentary error checking implemented, but it's obvious of something goes wrong >;)
				if (tokens[0] == "namespace")
				{
					if (tokens.size() != 2)
					{
						error("namespace declaration must have exactly 2 tokens");
						skipThisBlock();
					}
					else
					{
						transform(tokens[1].begin(), tokens[1].end(), tokens[1].begin(), tolower);
						m_currentnamespace = tokens[1];
						parseNamespace(*g_params);
					}
				}
				else if (tokens[0] == "democonfig")
				{
					if (context->getType() == CONTEXT_TYPE_DEMO)
					{
						parseDemoConfig(*g_system, static_cast<Demo*>(context));
					}
					else
					{
						error("demo config not supported in this prod type");
						skipThisBlock();
					}
				}
				else if (tokens[0] == "timeline")
				{
					if (context->getType() == CONTEXT_TYPE_DEMO)
					{
						parseTimeline(*static_cast<Demo*>(context));
					}
					else
					{
						error("timeline not supported in this prod type");
						skipThisBlock();
					}
				}
				else if (tokens[0] == "texture")
				{
					demorender::TextureParameters* textureparams = new demorender::TextureParameters();
					parseTexture(*textureparams);
					demorender::g_textures->addTextureParameters(tokens[1], textureparams);
				}
				else if (tokens[0] == "rendertarget")
				{
					//image exists?
					demorender::Image *image = demorender::g_textures->image(tokens[1]);

					bool createImage = image == 0;
					//image does not exist
					if (createImage)
					{
						image = new demorender::Image();
					}

					parseRenderTarget(*image, createImage);
					demorender::g_textures->addImage(tokens[1], image);
				}
				else if (tokens[0] == "bpm")
				{
					if (context->getType() == CONTEXT_TYPE_DEMO)
					{
						BPM* bpm = new BPM();
						bpm->setName(tokens[1]);
						parseBPM(*bpm);
						g_sync->addBeat(tokens[1], bpm);
					}
					else
					{
						error("beat not supported in this prod type");
						skipThisBlock();
					}
				}
				else if (tokens[0] == "trigger")
				{
					if (context->getType() == CONTEXT_TYPE_DEMO)
					{
						TriggerSystem *triggers = new TriggerSystem();
						parseTrigger(*triggers);
						triggers->setName(tokens[1]);
						g_sync->addTriggers(tokens[1], triggers);
					}
					else
					{
						error("triggers not supported in this prod type");
						skipThisBlock();
					}
				}
				else if (tokens[0] == "event")
				{
					if (context->getType() == CONTEXT_TYPE_DEMO)
					{
						Event *e = new Event();
						parseEvent(*e);
						e->m_name = tokens[1]; //kludge
						g_sync->addEvent(tokens[1], e);
					}
					else
					{
						error("events not supported in this prod type");
						skipThisBlock();
					}
				}
			}
		}

		else
		{
			g_debug << "cannot open script file " << filename << " for reading!" << std::endl;
		}
		g_debug << "Parsing done!" << std::endl;
		return true;
	}
}