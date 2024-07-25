#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"
#include "pipeline.h"
#include "swapchain.h"

#include <memory>
#include <vector>

namespace Vengine
{
	class App
	{
	public:
		static constexpr uint32_t WIDTH = 800;
		static constexpr uint32_t HEIGHT = 600;

	public:
		App();
		~App();

		 // Not copyable or movable
		App(const App &) = delete;
		void operator=(const App &) = delete;
		App(App &&) = delete;
		App &operator=(App &&) = delete;

		void run();


	private:
		
		void mainLoop();

		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

	private:

		Viewer::Window window;
		Device device;
		SwapChain swapChain;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers	;	

	

	};
}

#endif //APPLICATION_H