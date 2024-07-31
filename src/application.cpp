#include "application.h"

#include <stdexcept>

#include "pipeline.h"


Vengine::App::App():
window(Viewer::Window(WIDTH, HEIGHT, "Snowdream")),
device(Device(window)),
renderer(Renderer(window, device))
{
	loadModels();
	createPipelineLayout();
	createPipeline();
	
}

Vengine::App::~App() 
{
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void Vengine::App::run()
{
	mainLoop();
}

void Vengine::App::mainLoop()
{
	while(!window.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device.device());
}

void Vengine::App::loadModels() 
{
	std::vector<Vengine::Model::Vertex> vertices =
	{
	{{0.0, -0.5, 0.0}, {1.0, 0.0, 0.0}},
	{{0.5, 0.5, 0.0 }, {0.0, 1.0, 0.0}},
	{{-0.5, 0.5, 0.0}, {0.0, 0.0, 1.0}}
	};

	this->model = std::make_unique<Vengine::Model>(device, vertices);
}

void Vengine::App::createPipelineLayout() 
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if(vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error{"Unable to create pipeline layout."};
	}
}

void Vengine::App::createPipeline() 
{
	assert(pipelineLayout != nullptr);

	Vengine::ConfigurationInfo pipelineConfig{};
	Vengine::Pipeline::defaultConfig(pipelineConfig);
	pipelineConfig.renderPass = renderer.getSwapChainRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<Pipeline>(
		device, 
		"shaders/vertex.vert.spv",
		"shaders/fragment.frag.spv",
		pipelineConfig);

}


void Vengine::App::recordCommandBuffer(int imageIndex) 
{

	pipeline->bind(commandBuffers[imageIndex]);

	model->bind(commandBuffers[imageIndex]);
	model->draw(commandBuffers[imageIndex]);
}


