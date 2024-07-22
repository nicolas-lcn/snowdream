#include "window.h"

// std
#include <stdexcept>

Viewer::Window::Window(uint32_t w, uint32_t h, std::string name) :
	width(w),
	height(h),
	name(name)
{
	this->initWindow();
}

Viewer::Window::~Window() 
{
	this->cleanup();
}

void Viewer::Window::initWindow() 
{
	glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    this->window = glfwCreateWindow(width, height, name.c_str() , nullptr, nullptr);

}

void Viewer::Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) 
{
	if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create window surface");
}

void Viewer::Window::cleanup() 
{
	glfwDestroyWindow(window);
    glfwTerminate();
}
