#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include "SceneManager.h"
#include "Quaternion.h"
#include "ConstructScene.h"
#include "DeferRenderPipeline.h"
#include "BufferManager.h"
#include "ReflectionCapture.h"
#include "SystemTextures.h"
#include "Application.h"
#include <thread>
#include <conio.h>

// settings
std::string AssetFolderPath = "..\\Assets\\";

std::shared_ptr<BufferManager> _GPUBuffers;
std::shared_ptr<SystemTextureFactory> GlobalTextures;

uint32 APP_DeltaTime = 0;
unsigned long APP_FrameCount = 0;

int32 main(int argc, char* argv[])
{
	unsigned long long x = 999;
	char str[256];
	sprintf_s(str, "%llu", x);
	std::string s = std::string(str);

	dkApplication a(argc, argv);
	return a.exec();
}

//void framebuffer_size_callback(GLFWwindow* window, int32 width, int32 height);
//void processInput(GLFWwindow* window);

//int32 main(int argc, char* argv[])
//{		
//	unsigned long long x = 999;
//	char str[256];
//	sprintf_s(str, "%llu", x);
//	std::string s = std::string(str);
//
//	dkApplication a(argc, argv);
//	return a.exec();
//
//	GLFWwindow* Window = InitWindow();
//	InitApplication();
//	InitView();
//
//	// uncomment this call to draw in wireframe polygons.
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//	// render loop
//	// -----------
//	while (!glfwWindowShouldClose(Window))
//	{
//		// input
//		// -----
//		processInput(Window);
//
//		// render
//		// ------
//		glClear(GL_COLOR_BUFFER_BIT);
//		Render();
//
//		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//	 // -------------------------------------------------------------------------------
//		if (bPause)
//		{
//			int32 a = 0;
//		}
//		glfwSwapBuffers(Window);
//		glfwPollEvents();
//	}
//
//	// glfw: terminate, clearing all previously allocated GLFW resources.
//	// ------------------------------------------------------------------
//	glfwTerminate();
//	return 0;
//}

//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int32 width, int32 height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and 
//	// height will be significantly larger than specified on retina displays.
//	glViewport(0, 0, width, height);
//}
//
//GLFWwindow* InitWindow()
//{
//	// glfw: initialize and configure
//// ------------------------------
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
//	//glfwWindowHint(GLFW_SAMPLES, 4);
//	const uint32 _ScreenWidth = 1200;
//	const uint32 _ScreenHeight = 900;
//
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
//#endif
//
//	// glfw window creation
//	// --------------------
//	GLFWwindow* window = glfwCreateWindow(_ScreenWidth, _ScreenHeight, "DigitalHumanDemo", NULL, NULL);
//	if (window == NULL)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return 0;
//	}
//
//	//glfwHideWindow(window);
//
//	glfwSetWindowPos(window, 10, 30);
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//	// At init, on windows
//	if (!gladLoadGL())
//	{
//		std::cout << "Failed to initialize glew" << std::endl;
//		return 0;
//	}
//
//	return window;
//}