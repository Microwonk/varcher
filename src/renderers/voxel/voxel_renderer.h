#include "engine/renderer.h"

#include <optional>
#include <renderers/common/performance_settings.h>
#include "renderers/voxel/resource/camera_controller.h"
#include "renderers/voxel/resource/voxel_scene.h"
#include "voxel_render_settings.h"

#include "engine/resource/texture_2d.h"
#include "renderers/voxel/stages/geometry_stage.h"
#ifdef _WIN32
#include "renderers/voxel/stages/upscaler_stage.h"
#endif
#include "renderers/voxel/stages/blit_stage.h"
#include "engine/gui/imgui_renderer.h"

class VoxelRenderer : public ARenderer
{
private:
    std::unique_ptr<CameraController> _camera;

    std::shared_ptr<VoxelRenderSettings> _v_settings;
    std::shared_ptr<PerformanceSettings> _p_settings;

    std::shared_ptr<VoxelScene> _scene;

    std::unique_ptr<GeometryStage> _geometryStage;
#ifdef _WIN32
    std::unique_ptr<UpscalerStage> _upscalerStage;
#endif
    std::unique_ptr<BlitStage> _blitStage;

    std::unique_ptr<ImguiRenderer> _imguiRenderer;

    float _time = 0;

public:
    explicit VoxelRenderer(const std::shared_ptr<Engine>& engine);
    void update(float delta) override;
    void recordCommands(const vk::CommandBuffer& commandBuffer, uint32_t swapchainImage, uint32_t flightFrame) override;
};
