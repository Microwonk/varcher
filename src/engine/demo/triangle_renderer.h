#include "engine/renderer.h"
#include "engine/util/resource_ring.h"
#include "engine/demo/triangle_pipeline.h"

class TriangleRenderer : public ARenderer
{
private:
    std::unique_ptr<TrianglePipeline> _pipeline;

    float _time = 0;

public:
    TriangleRenderer(const std::shared_ptr<Engine>& engine);
    virtual void update(float delta) override;
    virtual void recordCommands(const vk::CommandBuffer& commandBuffer, uint32_t swapchainImage, uint32_t flightFrame) override;
};
