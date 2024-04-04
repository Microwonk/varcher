project(engine CXX)

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

find_package(VulkanHeaders REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(vk-bootstrap CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#stb
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
        Vulkan::Headers
        GPUOpen::VulkanMemoryAllocator
        fmt::fmt
        vk-bootstrap::vk-bootstrap
        glfw
        glm::glm
        unofficial::nativefiledialog::nfd
        imgui::imgui
)

target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        <memory>
        <vector>
        <unordered_map>
        <unordered_set>
        <deque>
        <functional>
        <optional>
        <fmt/format.h>
        <VkBootstrap.h>
        <vulkan/vulkan.hpp>
        <glm/glm.hpp>
)