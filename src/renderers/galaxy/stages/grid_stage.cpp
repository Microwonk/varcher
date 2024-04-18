#include "grid_stage.h"

const vk::PipelineLayout &GridStage::getPipelineLayout() const {
    return _pipeline->layout;
}

GridStage::GridStage(const std::shared_ptr<Engine> &engine, const RenderPass &renderPass) : AResource(engine) {
    _pipeline = std::make_unique<GridPipeline>(GridPipeline::build(engine, renderPass.renderPass));

    engine->recreationQueue->push(RecreationEventFlags::TARGET_RESIZE, [&]() {
        _pipeline->descriptorSet->initBuffer(1, _cameraBuffer->buffer, _cameraBuffer->size,
                                             vk::DescriptorType::eUniformBuffer);

        return [=](const std::shared_ptr<Engine> &) {};
    });
}

void GridStage::record(const vk::CommandBuffer &cmd, uint32_t flightFrame, const RenderImage &image,
                       const Framebuffer &windowFramebuffer, const RenderPass &windowRenderPass,
                       std::function<void(const vk::CommandBuffer &)> uiStage) {

    vk::Viewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(engine->windowSize.x);
    viewport.height = static_cast<float>(engine->windowSize.y);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    cmd.setViewport(0, 1, &viewport);

    vk::Rect2D scissor;
    scissor.offset = vk::Offset2D(0, 0);
    scissor.extent = vk::Extent2D(engine->windowSize.x, engine->windowSize.y);
    cmd.setScissor(0, 1, &scissor);

    _cameraBuffer->copyData(&_camera, sizeof(Camera));

    _pipeline->descriptorSet->writeImage(0, flightFrame, image.imageView, image.sampler,
                                         vk::ImageLayout::eShaderReadOnlyOptimal);
    _pipeline->descriptorSet->writeBuffer(1, flightFrame, _cameraBuffer->buffer, sizeof(Camera),
                                          vk::DescriptorType::eUniformBuffer);

    windowRenderPass.recordBegin(cmd, windowFramebuffer);
    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline->pipeline);
    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipeline->layout,
                           0, 1,
                           _pipeline->descriptorSet->getSet(flightFrame),
                           0, nullptr);
    cmd.draw(3, 1, 0, 0);
    uiStage(cmd);
    cmd.endRenderPass();

}
