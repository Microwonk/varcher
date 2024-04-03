#pragma once

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <memory>
#include <functional>
#include <engine/util/recreation_queue.h>
#include <engine/util/resource_ring.h>
#include "input_callbacks.h"
#include "util/deletion_queue.h"
#include "renderer.h"
#include "swapchain.h"

#define MAX_FRAMES_IN_FLIGHT 2

struct GLFWwindow;

class Engine : public std::enable_shared_from_this<Engine> {
public:
    GLFWwindow* window;
    glm::uvec2 windowSize = { 1280, 720 };
    bool windowResized = false;

    InputCallbacks inputs;

    vk::Instance instance;
    vk::DebugUtilsMessengerEXT debugMessenger;

    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    PFN_vkGetInstanceProcAddr getInstanceProcAddr;
    PFN_vkGetDeviceProcAddr getDeviceProcAddr;

    VmaAllocator allocator;

    vk::SurfaceKHR surface;

    DeletionQueue deletionQueue;
    std::optional<RecreationQueue> recreationQueue;

    Swapchain swapchain;

    vk::Queue graphicsQueue;
    uint32_t graphicsQueueFamily;

    vk::CommandPool renderCommandPool;
    ResourceRing<vk::CommandBuffer> renderCommandBuffers;

    vk::CommandPool uploadCommandPool;
    vk::CommandBuffer uploadCommandBuffer;

    vk::DescriptorPool descriptorPool;

    ResourceRing<vk::Semaphore> presentSemaphores;
    ResourceRing<vk::Semaphore> renderSemaphores;
    ResourceRing<vk::Fence> renderFences;

    std::shared_ptr<ARenderer> renderer;

private:
    bool _initialized;
    uint32_t _frameCount;

public:
    void init();
    void setRenderer(const std::shared_ptr<ARenderer>& renderer);
    void run();
    void destroy();

    void upload_submit(const std::function<void(const vk::CommandBuffer& cmd)>& recordCommands);

private:
    void draw(float delta);

    void resize();

    void initGLFW();
    void initVulkan();
    void initSyncStructures();
};
