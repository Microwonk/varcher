#include "grid_stage.h"


const vk::PipelineLayout &GridStage::getPipelineLayout() const {
    return _pipeline->layout;
}

GridStage::GridStage(const std::shared_ptr<Engine> &engine, const RenderPass &renderPass) : AResource(engine) {
    _pipeline = std::make_unique<GridPipeline>(GridPipeline::build(engine, renderPass.renderPass));
}

void GridStage::record(const vk::CommandBuffer &cmd, uint32_t flightFrame, const RenderImage &image,
                       const Framebuffer &windowFramebuffer, const RenderPass &windowRenderPass,
                       std::function<void(const vk::CommandBuffer &)> uiStage) {

}
