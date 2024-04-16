#include <fmt/core.h>
#include <engine/engine.h>
#include <renderers/galaxy/galaxy_renderer.h>
#include "app.h"
#include "renderers/triangle/triangle_renderer.h"
#include "renderers/voxel/voxel_renderer.h"

static auto makeRenderer(char type, const std::shared_ptr<Engine>& engine) -> std::shared_ptr<ARenderer> {
    switch (type) {
        case 'T': return std::make_shared<TriangleRenderer>(engine);
        case 'V': return std::make_shared<VoxelRenderer>(engine);
        case 'G': return std::make_shared<GalaxyRenderer>(engine);
        default:
            fmt::println("'{}' is not a valid renderer, using default", type);
            return std::make_shared<TriangleRenderer>(engine);
    }
}

auto run(char rendererType) -> int {
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
