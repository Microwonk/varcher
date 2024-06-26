#include "shader_module.h"

#include "engine/engine.h"
#include <fstream>
#include <fmt/format.h>

ShaderModule::ShaderModule(const std::shared_ptr<Engine>& engine, const std::string& filename, vk::ShaderStageFlagBits stage)
    : AResource(engine), _stage(stage)
{
    // Open shader file
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error(fmt::format("Failed to open shader file {}.", filename));
    }

    // Reserve buffer to load file
    size_t filesize = file.tellg();
    std::vector<uint32_t> buffer(filesize / sizeof(uint32_t));

    // Load file into buffer
    file.seekg(0);
    file.read((char*)buffer.data(), filesize);

    vk::ShaderModuleCreateInfo shaderCreateInfo;
    shaderCreateInfo.codeSize = buffer.size() * sizeof(uint32_t);
    shaderCreateInfo.pCode = buffer.data();
    vk::ShaderModule createdModule = engine->device.createShaderModule(shaderCreateInfo);
    _shaderModule = createdModule;
    pushDeletor([=](const std::shared_ptr<Engine>& delEngine) {
        delEngine->device.destroyShaderModule(createdModule);
    });
}

vk::PipelineShaderStageCreateInfo ShaderModule::buildStageCreateInfo() const
{
    vk::PipelineShaderStageCreateInfo info;
    info.stage = _stage;
    info.module = _shaderModule;
    info.pName = "main";
    return info;
}
