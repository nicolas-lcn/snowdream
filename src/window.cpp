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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    this->window = glfwCreateWindow(width, height, name.c_str() , nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, &Window::resize);

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

void Viewer::Window::resize(GLFWwindow* window, int width, int height)
{
	auto wind = reinterpret_cast<Viewer::Window*>(glfwGetWindowUserPointer(window));
	wind->framebufferResized = true;
	wind->width = static_cast<uint32_t>(width);
	wind->height = static_cast<uint32_t>(height);
}