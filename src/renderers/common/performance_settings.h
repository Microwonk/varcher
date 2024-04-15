#pragma once

class PerformanceSettings {
public:
    VkPresentModeKHR presentMode = VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR;

    [[nodiscard]] std::string presentModeName() const {
        return presentModeNameStr(presentMode);
    }

    static std::string presentModeNameStr(VkPresentModeKHR presentMode) {
        switch (presentMode) {
            case VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR:
                return "Immediate";
            case VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR:
                return "Mailbox";
            case VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR:
                return "FIFO";
            case VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR:
                return "FIFO Relaxed";
            default: return "Invalid";
        }
    }
};