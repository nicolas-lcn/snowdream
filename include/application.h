#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"
#include "pipeline.h"
#include "swapchain.h"

#include <memory>
#include <vector>
#include "model.h"

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
		App& operator=(const App &) = delete;
		App(App &&) = delete;
		App &operator=(App &&) = delete;

		void run();


	private:
		
		void mainLoop();

		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

	private:

		Viewer::Window window;
		Device device;
		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers	;	

		std::unique_ptr<Vengine::Model> model;

	};
}

#endif //APPLICATION_H