#include <renderers/common/performance_settings.h>
#include "engine/renderer.h"
#include "engine/util/resource_ring.h"
#include "renderers/triangle/triangle_pipeline.h"
#include "parameters.h"
#include "engine/resource/buffer.h"
#include "fractal_pipeline.h"
#include <engine/gui/imgui_renderer.h>

class FractalRenderer : public ARenderer
{
private:
    std::unique_ptr<FractalPipeline> _pipeline;
    std::shared_ptr<PerformanceSettings> _p_settings;
    std::shared_ptr<ImguiRenderer> _imguiRenderer;

    ShaderSettings _settings = {};
    std::unique_ptr<Buffer> _settingsBuffer;

    std::unique_ptr<Buffer> _vertexBuffer;

    // Quad
    std::vector<FractalVertex> vertices = {{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
                                    {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
                                    {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                    {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
                                    {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                    {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}};

    glm::vec3 rotation = glm::vec3();
    // glm::vec3 cameraPos = glm::vec3();
    float zoom = 0;

    float _time = 0;

public:
    explicit FractalRenderer(const std::shared_ptr<Engine>& engine);
    void update(float delta) override;
    void recordCommands(const vk::CommandBuffer& commandBuffer, uint32_t swapchainImage, uint32_t flightFrame) override;
};
