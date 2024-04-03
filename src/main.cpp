#include <fmt/core.h>
#include <engine/engine.h>

int main() {
    fmt::print("Test");
    try {
        auto engine = std::make_shared<Engine>();
        engine->init();

        // auto renderer = std::make_shared<MyRenderer>();

        engine->run();
        engine->destroy();

    } catch (const std::exception& e) {
        fmt::println("FATAL ERROR: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
