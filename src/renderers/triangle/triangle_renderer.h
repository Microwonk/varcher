#include "engine/renderer.h"
#include "engine/util/resource_ring.h"
#include "triangle_pipeline.h"

class TriangleRenderer : public ARenderer
{
private:
    std::unique_ptr<TrianglePipeline> _pipeline;

    float _time = 0;

public:
    explicit TriangleRenderer(const std::shared_ptr<Engine>& engine);
    void update(float delta) override;
    void recordCommands(const vk::CommandBuffer& commandBuffer, uint32_t swapchainImage, uint32_t flightFrame) override;
};
