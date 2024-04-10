#include <fmt/core.h>
#include <engine/engine.h>
#include "app.h"
#include "renderers/triangle/triangle_renderer.h"
#include "renderers/voxel/voxel_renderer.h"

static auto makeRenderer(Type type, const std::shared_ptr<Engine>& engine) -> std::shared_ptr<ARenderer> {
    switch (type) {
        case TRIANGLE: return std::make_shared<TriangleRenderer>(engine);
        case VOXEL: return std::make_shared<VoxelRenderer>(engine);
        case GALAXY: return nullptr;
    }
}

auto run(Type rendererType) -> int {
    try {
        auto engine = std::make_shared<Engine>();
        engine->init();

        auto renderer = makeRenderer(rendererType, engine);
        engine->setRenderer(renderer);

        engine->run();
        engine->destroy();

    } catch (const std::exception& e) {
        fmt::println("FATAL ERROR: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
