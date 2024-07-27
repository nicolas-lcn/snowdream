#ifndef PIPELINE_H
#define PIPELINE_H 

#include <string>
#include <vector>
#include <memory>


#include "device.h"

namespace Vengine
{
	struct ConfigurationInfo
	{
		ConfigurationInfo() = default;
		ConfigurationInfo(const ConfigurationInfo&) = delete;
		ConfigurationInfo& operator=(const ConfigurationInfo) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;

		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline
	{
	public:
		Pipeline(Vengine::Device& device, const std::string& vertexFilePath, const std::string& fragmentFilePath, const Vengine::ConfigurationInfo& configInfo);
		~Pipeline();

		Pipeline(const Pipeline&) = delete;
		Pipeline &operator=(const Pipeline&) = delete;
	
		static void defaultConfig(ConfigurationInfo& configurationInfo);
		void bind(VkCommandBuffer commandBuffer);
	private:
		std::vector<char> loadShader(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentFilePath, const ConfigurationInfo& configInfo);
		void createShaderModule(const std::vector<char> & buffer, VkShaderModule *module);

	private:
		
		VkShaderModule vertexModule;
		VkShaderModule fragmentModule;

		Vengine::Device& device;
		VkPipeline graphicsPipeline;

	};

	
}


#endif //PIPELINE_H