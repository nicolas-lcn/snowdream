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

		inline VkExtent2D getExtent(){return {width, height};}
		inline bool isResized(){ return framebufferResized;}
		inline void resetFlags() { framebufferResized = false;}

		Window(const Window& ) = delete;
		Window &operator=(const Window&) = delete;

	private:
		void initWindow();
		void cleanup();

		static void resize(GLFWwindow* window, int width, int height);

		GLFWwindow* window;
		uint32_t width, height;
		std::string name;

		bool framebufferResized = false;

	
	};
}
#endif //WINDOW_H