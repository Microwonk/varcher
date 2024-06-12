#include "grid_stage.h"
#include <glm/gtc/matrix_transform.hpp>
#include "engine/commands/command_util.h"

const vk::PipelineLayout &GridStage::getPipelineLayout() const {
    return _pipeline->layout;
}

GridStage::GridStage(const std::shared_ptr<Engine> &engine, const RenderPass &renderPass) : AResource(engine) {

    _cameraBuffer = std::make_unique<Buffer>(engine, sizeof(Camera), vk::BufferUsageFlagBits::eUniformBuffer,
                                             VMA_MEMORY_USAGE_CPU_TO_GPU, "Grid Camera Buffer");

    Vertex verts[4] = {{{-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f}}, {{0.5f, 0.0f, -0.5f}, {1.0f, 0.0f}}, {{-0.5f, 0.0f, 0.5f}, {0.0f, 1.0f}}, {{0.5f, 0.0f, 0.5f}, {1.0f, 1.0f}}};
    uint32_t indices[6] = {0, 1, 2, 1, 2, 3};

    _quadVertBuffer = std::make_unique<Buffer>(engine, sizeof(verts), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                                               VMA_MEMORY_USAGE_CPU_TO_GPU, "Grid Quad Buffer");
    _quadVertBuffer->copyData(&verts, sizeof(verts));

    _quadIndexBuffer = std::make_unique<Buffer>(engine, sizeof(indices), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
                                                VMA_MEMORY_USAGE_CPU_TO_GPU, "Grid Quad Buffer");
    _quadIndexBuffer->copyData(&indices, sizeof(indices));

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

    _camera.u_view = view;
    _camera.u_proj = projection;
    _camera.u_viewProj = projection * view;

    _cameraBuffer->copyData(&_camera, sizeof(Camera));

    _pipeline->descriptorSet->writeBuffer(0, flightFrame, _cameraBuffer->buffer, sizeof(Camera),
                                          vk::DescriptorType::eUniformBuffer);

    windowRenderPass.recordBegin(cmd, windowFramebuffer);
    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline->pipeline);

    vk::Buffer vertBuffers[] = {_quadVertBuffer->buffer};
    vk::DeviceSize offsets[] = {0};
    cmd.bindVertexBuffers(0, 1, vertBuffers, offsets);
    cmd.bindIndexBuffer(_quadIndexBuffer->buffer, 0, vk::IndexType::eUint32);

    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipeline->layout,
                           0, 1,
                           _pipeline->descriptorSet->getSet(flightFrame),
                           0, nullptr);

    _pipeline->descriptorSet->writeBuffer(0, flightFrame, _cameraBuffer->buffer, sizeof(Camera), vk::DescriptorType::eUniformBuffer);

    cmd.drawIndexed(6, 1, 0, 0, 0);
    uiStage(cmd);
    cmd.endRenderPass();

}
