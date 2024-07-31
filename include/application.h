#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"
#include "pipeline.h"
#include "swapchain.h"

#include <memory>
#include <vector>
#include "model.h"
#include "renderer.h"



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

	private:

		Viewer::Window window;
		Device device;
		
		Renderer renderer;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;

		std::unique_ptr<Vengine::Model> model;

	};
}

#endif //APPLICATION_H