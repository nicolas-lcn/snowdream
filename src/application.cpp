#include "application.h"

#include <vulkan/vulkan.h>


void Vengine::App::run()
{
	initVulkan();

	mainLoop();
}

void Vengine::App::mainLoop()
{
	while(!window.shouldClose()) {
        glfwPollEvents();
    }
}


