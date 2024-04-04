#include <fmt/core.h>
#include <engine/engine.h>
//#include "engine/triangle/triangle_renderer.h"
#include "engine/voxels/voxel_renderer.h"

int run() {
    try {
        auto engine = std::make_shared<Engine>();
        engine->init();

//        auto renderer = std::make_shared<TriangleRenderer>(engine);
        auto renderer = std::make_shared<VoxelRenderer>(engine);
        engine->setRenderer(renderer);

        engine->run();
        engine->destroy();

    } catch (const std::exception& e) {
        fmt::println("FATAL ERROR: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
