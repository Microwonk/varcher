#include "grid_stage.h"
#include <glm/gtc/matrix_transform.hpp>
#include "engine/commands/command_util.h"

const vk::PipelineLayout &GridStage::getPipelineLayout() const {
    return _pipeline->layout;
}

GridStage::GridStage(const std::shared_ptr<Engine> &engine, const RenderPass &renderPass) : AResource(engine) {

    _cameraBuffer = std::make_unique<Buffer>(engine, sizeof(ViewUniforms), vk::BufferUsageFlagBits::eUniformBuffer,
                                             VMA_MEMORY_USAGE_CPU_TO_GPU, "Grid Camera Buffer");

    _pipeline = std::make_unique<GridPipeline>(GridPipeline::build(engine, renderPass.renderPass));

    engine->recreationQueue->push(RecreationEventFlags::TARGET_RESIZE, [&]() {
        _pipeline->descriptorSet->initBuffer(0, _cameraBuffer->buffer, _cameraBuffer->size,
                                             vk::DescriptorType::eUniformBuffer);

        return [=](const std::shared_ptr<Engine> &) {};
    });
}

void GridStage::record(const vk::CommandBuffer &cmd, uint32_t flightFrame,
                       const Framebuffer &windowFramebuffer, const RenderPass &windowRenderPass,
                       const std::function<void(const vk::CommandBuffer &)>& uiStage,
                       GridParams params) {

    glm::mat4 view, projection;
    view = glm::lookAt(params.cam.pos, params.cam.center, params.cam.up);
    projection = glm::perspective(params.fov, (float)engine->windowSize.x / (float)engine->windowSize.y, 0.1f, INFINITY);

    _camera.view = view;
    _camera.proj = projection;
    _camera.pos = params.cam.pos;

    _cameraBuffer->copyData(&_camera, sizeof(ViewUniforms));

    _pipeline->descriptorSet->writeBuffer(0, flightFrame, _cameraBuffer->buffer, sizeof(ViewUniforms),
                                          vk::DescriptorType::eUniformBuffer);

    windowRenderPass.recordBegin(cmd, windowFramebuffer);
    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline->pipeline);

    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipeline->layout,
                           0, 1,
                           _pipeline->descriptorSet->getSet(flightFrame),
                           0, nullptr);

    _pipeline->descriptorSet->writeBuffer(0, flightFrame, _cameraBuffer->buffer, sizeof(ViewUniforms), vk::DescriptorType::eUniformBuffer);

    cmd.draw(0, 1, 0, 0);
    uiStage(cmd);
    cmd.endRenderPass();

}
