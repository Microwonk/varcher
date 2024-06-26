#pragma once

#include <vulkan/vulkan.hpp>
#include "util/resource_ring.h"

class Engine;

// Vulkan swapchain abstraction.
// Should only be created by the main engine.
class Swapchain
{
private:
    std::shared_ptr<Engine> _engine;
public:
    vk::SwapchainKHR swapchain;

    vk::Format imageFormat;
    ResourceRing<vk::Image> images;
    ResourceRing<vk::ImageView> imageViews;
public:
    void init(const std::shared_ptr<Engine>& engine);

    [[nodiscard]] uint32_t size() const;
};
