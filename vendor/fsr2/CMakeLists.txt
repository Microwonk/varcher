add_library(fsr2 INTERFACE)

target_include_directories(fsr2 INTERFACE include)

add_library(ffx_fsr_api IMPORTED UNKNOWN)

if(UNIX)
    set_target_properties(ffx_fsr_api PROPERTIES
            IMPORTED_LOCATION_RELEASE
            ${PROJECT_SOURCE_DIR}/vendor/fsr2/lib/libffx_fsr2_api_x86_64.a
            IMPORTED_LOCATION_DEBUG
            ${PROJECT_SOURCE_DIR}/vendor/fsr2/lib/libffx_fsr2_api_x86_64.a
    )
    set_target_properties(ffx_fsr_api PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

    add_library(ffx_fsr_api_vk IMPORTED UNKNOWN)
    set_target_properties(ffx_fsr_api_vk PROPERTIES
            IMPORTED_LOCATION_RELEASE
            ${PROJECT_SOURCE_DIR}/vendor/fsr2/lib/libffx_fsr2_api_vk_x86_64.a
            IMPORTED_LOCATION_DEBUG
            ${PROJECT_SOURCE_DIR}/vendor/fsr2/lib/libffx_fsr2_api_vk_x86_64.a
    )
    set_target_properties(ffx_fsr_api_vk PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)
elseif(WIN32)
    set_target_properties(ffx_fsr_api PROPERTIES
            IMPORTED_LOCATION_RELEASE
            ${PROJECT_SOURCE_DIR}/vendor/fsr2/lib/ffx_fsr2_api_x64.lib
            IMPORTED_LOCATION_DEBUG
            ${PROJECT_SOURCE_DIR}/vendor/fsr2/lib/ffx_fsr2_api_x64d.lib
    )
    set_target_properties(ffx_fsr_api PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

    add_library(ffx_fsr_api_vk IMPORTED UNKNOWN)
    set_target_properties(ffx_fsr_api_vk PROPERTIES
            IMPORTED_LOCATION_RELEASE
            ${PROJECT_SOURCE_DIR}/vendor/fsr2/lib/ffx_fsr2_api_vk_x64.lib
            IMPORTED_LOCATION_DEBUG
            ${PROJECT_SOURCE_DIR}/vendor/fsr2/lib/ffx_fsr2_api_vk_x64d.lib
    )
    set_target_properties(ffx_fsr_api_vk PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)
else()
    message(FATAL_ERROR "Unsupported OS")
endif()

target_link_libraries(fsr2 INTERFACE ffx_fsr_api ffx_fsr_api_vk)

