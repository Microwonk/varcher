#include <renderers/common/performance_settings.h>
#include "engine/renderer.h"
#include "engine/util/resource_ring.h"
#include "triangle_pipeline.h"
#include <engine/gui/imgui_renderer.h>

class TriangleRenderer : public ARenderer
{
private:
    std::unique_ptr<TrianglePipeline> _pipeline;
    std::shared_ptr<PerformanceSettings> _p_settings;
    std::shared_ptr<ImguiRenderer> _imguiRenderer;

    float _time = 0;

public:
    explicit TriangleRenderer(const std::shared_ptr<Engine>& engine);
    void update(float delta) override;
    void recordCommands(const vk::CommandBuffer& commandBuffer, uint32_t swapchainImage, uint32_t flightFrame) override;
};
