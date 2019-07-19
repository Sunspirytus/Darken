#pragma once

#include "TypeDefine.h"

class OpenGLContext
{
public:

	static void Init() 
	{
		/*if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize glew" << std::endl;
			return;
		}*/
		if (!gladLoadGL())
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
	};

private:

};