#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <string>

namespace Viewer
{
	class Window
	{
	public:
		Window(uint32_t w, uint32_t h, std::string name);
		~Window();

		bool shouldClose() {return glfwWindowShouldClose(this->window);}

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		Window(const Window& ) = delete;
		Window &operator=(const Window&) = delete;

	private:
		void initWindow();
		void cleanup();

		GLFWwindow* window;
		uint32_t width, height;
		std::string name;

	
	};
}
#endif //WINDOW_H