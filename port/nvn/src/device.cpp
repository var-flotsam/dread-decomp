// The NVN device over Vulkan (include/nvn_shim/device.h). Signatures are those of the game's calls:
// fn_71001587f0 (src/game_s7/fn_71001587f0.cpp) for the builder, Initialize, GetInteger, the depth
// and window-origin modes and the fast-clear registrations.

#include "nvn_shim/device.h"

#include <atomic>
#include <chrono>
#include <mutex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <vector>

#include <vulkan/vulkan.h>

#include "nvn_shim/procs.h"
#include "objects.h"

namespace dread::nvn {

namespace {

Backend g_backend;
std::atomic<bool> g_backend_ready{false};
std::mutex g_backend_lock;  // CreateBackend and DestroyBackend
std::mutex g_queue_lock;    // the one VkQueue (Submit)
std::uint64_t g_submitted = 0;
int g_errors = 0, g_warnings = 0;
VkDebugUtilsMessengerEXT g_messenger = VK_NULL_HANDLE;

VKAPI_ATTR VkBool32 VKAPI_CALL OnMessage(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                         VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT* data,
                                         void*) {
    const bool error = severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    (error ? g_errors : g_warnings)++;
    std::fprintf(stderr, "nvn shim: Vulkan %s: %s\n", error ? "error" : "warning", data->pMessage ? data->pMessage : "");
    return VK_FALSE;
}

}  // namespace

std::uint32_t Backend::MemoryType(std::uint32_t type_bits, VkMemoryPropertyFlags flags) const {
    for (std::uint32_t i = 0; i < memory.memoryTypeCount; ++i)
        if ((type_bits & (1u << i)) && (memory.memoryTypes[i].propertyFlags & flags) == flags)
            return i;
    return UINT32_MAX;
}

const Backend* CurrentBackend() {
    return g_backend_ready ? &g_backend : nullptr;
}

int ValidationErrors() {
    return g_errors;
}
int ValidationWarnings() {
    return g_warnings;
}

bool CreateBackend(bool validation) {
    std::lock_guard<std::mutex> lock(g_backend_lock);
    if (g_backend_ready)
        return true;
    const char* env = std::getenv("DREAD_NVN_VALIDATION");
    validation = validation || (env && std::strcmp(env, "1") == 0);
    VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app.pApplicationName = "dread";
    app.apiVersion = VK_API_VERSION_1_3;
    const char* layers[] = {"VK_LAYER_KHRONOS_validation"};
    const char* extensions[] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
    VkDebugUtilsMessengerCreateInfoEXT messenger{VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    messenger.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messenger.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    messenger.pfnUserCallback = OnMessage;
    VkInstanceCreateInfo ici{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    ici.pApplicationInfo = &app;
    if (validation) {
        ici.pNext = &messenger;
        ici.enabledLayerCount = 1;
        ici.ppEnabledLayerNames = layers;
        ici.enabledExtensionCount = 1;
        ici.ppEnabledExtensionNames = extensions;
    }
    Backend b;
    b.validation = validation;
    if (vkCreateInstance(&ici, nullptr, &b.instance) != VK_SUCCESS) {
        std::fprintf(stderr, "nvn shim: vkCreateInstance failed%s\n", validation ? " (are the validation layers installed?)" : "");
        return false;
    }
    if (validation) {
        auto create = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(b.instance, "vkCreateDebugUtilsMessengerEXT"));
        if (create)
            create(b.instance, &messenger, nullptr, &g_messenger);
    }
    std::uint32_t count = 0;
    vkEnumeratePhysicalDevices(b.instance, &count, nullptr);
    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(b.instance, &count, devices.data());
    // The first device with Vulkan 1.3, the three features and a graphics queue.
    for (VkPhysicalDevice pd : devices) {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(pd, &props);
        if (props.apiVersion < VK_API_VERSION_1_3)
            continue;
        VkPhysicalDeviceVulkan12Features f12{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
        VkPhysicalDeviceVulkan13Features f13{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
        f13.pNext = &f12;
        VkPhysicalDeviceFeatures2 f{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
        f.pNext = &f13;
        vkGetPhysicalDeviceFeatures2(pd, &f);
        // The zero buffer for unfed vertex inputs is binding 16 (pipeline.h): 17 bindings.
        if (!f13.dynamicRendering || !f13.maintenance4 || !f12.timelineSemaphore || props.limits.maxVertexInputBindings < 17)
            continue;
        std::uint32_t families = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(pd, &families, nullptr);
        std::vector<VkQueueFamilyProperties> fam(families);
        vkGetPhysicalDeviceQueueFamilyProperties(pd, &families, fam.data());
        for (std::uint32_t i = 0; i < families; ++i) {
            if (fam[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                b.physical = pd;
                b.queue_family = i;
                b.properties = props;
                break;
            }
        }
        if (b.physical)
            break;
    }
    if (!b.physical) {
        std::fprintf(stderr, "nvn shim: no Vulkan 1.3 device with dynamicRendering, maintenance4, timelineSemaphore, 17 "
                             "vertex bindings and a graphics queue\n");
        vkDestroyInstance(b.instance, nullptr);
        return false;
    }
    vkGetPhysicalDeviceMemoryProperties(b.physical, &b.memory);
    const float priority = 1.0f;
    VkDeviceQueueCreateInfo qci{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    qci.queueFamilyIndex = b.queue_family;
    qci.queueCount = 1;
    qci.pQueuePriorities = &priority;
    // The optional extensions (device.h), each with its feature where it has one.
    std::uint32_t ext_count = 0;
    vkEnumerateDeviceExtensionProperties(b.physical, nullptr, &ext_count, nullptr);
    std::vector<VkExtensionProperties> available(ext_count);
    vkEnumerateDeviceExtensionProperties(b.physical, nullptr, &ext_count, available.data());
    auto has = [&](const char* name) {
        for (const VkExtensionProperties& e : available)
            if (std::strcmp(e.extensionName, name) == 0)
                return true;
        return false;
    };
    std::vector<const char*> enabled;
    void* chain = nullptr;
    VkPhysicalDeviceDepthClipControlFeaturesEXT clip{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_CONTROL_FEATURES_EXT};
    VkPhysicalDeviceIndexTypeUint8FeaturesKHR uint8{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_KHR};
    VkPhysicalDeviceFeatures2 query{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
    const bool has_clip = has(VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME);
    // The EXT name first: the validation layers of the port's image know it, not the KHR one yet.
    const char* uint8_name = has(VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME)   ? VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME
                             : has(VK_KHR_INDEX_TYPE_UINT8_EXTENSION_NAME) ? VK_KHR_INDEX_TYPE_UINT8_EXTENSION_NAME
                                                                           : nullptr;
    if (has_clip) {
        clip.pNext = query.pNext;
        query.pNext = &clip;
    }
    if (uint8_name) {
        uint8.pNext = query.pNext;
        query.pNext = &uint8;  // the KHR and EXT feature structures are the same (promoted)
    }
    vkGetPhysicalDeviceFeatures2(b.physical, &query);
    if (has_clip && clip.depthClipControl) {
        enabled.push_back(VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME);
        clip.pNext = chain;
        chain = &clip;
        b.depth_clip_control = true;
    } else {
        std::fprintf(stderr, "nvn shim: no VK_EXT_depth_clip_control; the game's -1..1 depth range is clipped to 0..1\n");
    }
    if (uint8_name && uint8.indexTypeUint8) {
        enabled.push_back(uint8_name);
        uint8.pNext = chain;
        chain = &uint8;
        b.index_uint8 = true;
    }
    if (has(VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME)) {
        enabled.push_back(VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME);
        VkPhysicalDeviceExternalMemoryHostPropertiesEXT host{
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT};
        VkPhysicalDeviceProperties2 p2{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
        p2.pNext = &host;
        vkGetPhysicalDeviceProperties2(b.physical, &p2);
        b.host_import = true;
        b.host_import_alignment = host.minImportedHostPointerAlignment;
    }
    VkPhysicalDeviceVulkan12Features v12{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
    v12.timelineSemaphore = VK_TRUE;
    v12.pNext = chain;
    VkPhysicalDeviceVulkan13Features v13{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
    v13.dynamicRendering = VK_TRUE;
    v13.maintenance4 = VK_TRUE;
    v13.pNext = &v12;
    VkPhysicalDeviceFeatures supported{};
    vkGetPhysicalDeviceFeatures(b.physical, &supported);
    VkPhysicalDeviceFeatures2 features{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
    features.pNext = &v13;
    features.features.textureCompressionBC = supported.textureCompressionBC;
    features.features.depthBounds = supported.depthBounds;
    features.features.independentBlend = supported.independentBlend;
    features.features.fillModeNonSolid = supported.fillModeNonSolid;
    b.bc_textures = supported.textureCompressionBC == VK_TRUE;
    b.depth_bounds = supported.depthBounds == VK_TRUE;
    b.independent_blend = supported.independentBlend == VK_TRUE;
    b.fill_mode_non_solid = supported.fillModeNonSolid == VK_TRUE;
    if (!b.bc_textures)
        std::fprintf(stderr, "nvn shim: the device has no BC texture compression; the game's BC textures will fail\n");
    VkDeviceCreateInfo dci{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    dci.pNext = &features;
    dci.queueCreateInfoCount = 1;
    dci.pQueueCreateInfos = &qci;
    dci.enabledExtensionCount = static_cast<std::uint32_t>(enabled.size());
    dci.ppEnabledExtensionNames = enabled.data();
    if (vkCreateDevice(b.physical, &dci, nullptr, &b.device) != VK_SUCCESS) {
        std::fprintf(stderr, "nvn shim: vkCreateDevice failed\n");
        vkDestroyInstance(b.instance, nullptr);
        return false;
    }
    vkGetDeviceQueue(b.device, b.queue_family, 0, &b.queue);
    VkCommandPoolCreateInfo cpi{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    cpi.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    cpi.queueFamilyIndex = b.queue_family;
    vkCreateCommandPool(b.device, &cpi, nullptr, &b.command_pool);
    VkSemaphoreTypeCreateInfo sti{VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO};
    sti.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    VkSemaphoreCreateInfo sci{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    sci.pNext = &sti;
    vkCreateSemaphore(b.device, &sci, nullptr, &b.timeline);
    g_submitted = 0;
    g_backend = b;
    g_backend_ready = true;
    std::fprintf(stderr, "nvn shim: Vulkan device %s (Vulkan %u.%u), validation %s\n", b.properties.deviceName,
                 VK_API_VERSION_MAJOR(b.properties.apiVersion), VK_API_VERSION_MINOR(b.properties.apiVersion),
                 validation ? "on" : "off");
    return true;
}

void DestroyBackend() {
    std::lock_guard<std::mutex> lock(g_backend_lock);
    if (!g_backend_ready)
        return;
    vkDeviceWaitIdle(g_backend.device);
    if (g_backend.command_pool)
        vkDestroyCommandPool(g_backend.device, g_backend.command_pool, nullptr);
    if (g_backend.timeline)
        vkDestroySemaphore(g_backend.device, g_backend.timeline, nullptr);
    vkDestroyDevice(g_backend.device, nullptr);
    if (g_messenger) {
        auto destroy = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(g_backend.instance, "vkDestroyDebugUtilsMessengerEXT"));
        if (destroy)
            destroy(g_backend.instance, g_messenger, nullptr);
        g_messenger = VK_NULL_HANDLE;
    }
    vkDestroyInstance(g_backend.instance, nullptr);
    g_backend = Backend();
    g_backend_ready = false;
}

std::uint64_t Submit(const VkCommandBuffer* cmds, std::uint32_t count) {
    const Backend* b = CurrentBackend();
    if (!b || !b->timeline)
        return 0;
    std::lock_guard<std::mutex> lock(g_queue_lock);
    const std::uint64_t value = g_submitted + 1;
    VkTimelineSemaphoreSubmitInfo tsi{VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO};
    tsi.signalSemaphoreValueCount = 1;
    tsi.pSignalSemaphoreValues = &value;
    VkSubmitInfo si{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    si.pNext = &tsi;
    si.commandBufferCount = count;
    si.pCommandBuffers = cmds;
    si.signalSemaphoreCount = 1;
    si.pSignalSemaphores = &b->timeline;
    if (vkQueueSubmit(b->queue, 1, &si, VK_NULL_HANDLE) != VK_SUCCESS)
        return 0;
    g_submitted = value;
    return value;
}

std::uint64_t LastSubmitted() {
    std::lock_guard<std::mutex> lock(g_queue_lock);
    return g_submitted;
}

std::uint64_t Completed() {
    const Backend* b = CurrentBackend();
    std::uint64_t value = 0;
    if (b && b->timeline)
        vkGetSemaphoreCounterValue(b->device, b->timeline, &value);
    return value;
}

bool WaitFor(std::uint64_t value, std::uint64_t timeout_ns) {
    const Backend* b = CurrentBackend();
    if (!b || !b->timeline)
        return false;
    VkSemaphoreWaitInfo wi{VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO};
    wi.semaphoreCount = 1;
    wi.pSemaphores = &b->timeline;
    wi.pValues = &value;
    return vkWaitSemaphores(b->device, &wi, timeout_ns) == VK_SUCCESS;
}

bool ImmediateSubmit(const std::function<void(VkCommandBuffer)>& record) {
    static std::mutex pool_lock;  // the one-off command pool
    std::lock_guard<std::mutex> lock(pool_lock);
    const Backend* b = CurrentBackend();
    if (!b || !b->command_pool)
        return false;
    VkCommandBufferAllocateInfo cai{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    cai.commandPool = b->command_pool;
    cai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cai.commandBufferCount = 1;
    VkCommandBuffer cmd = VK_NULL_HANDLE;
    if (vkAllocateCommandBuffers(b->device, &cai, &cmd) != VK_SUCCESS)
        return false;
    VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &begin);
    record(cmd);
    vkEndCommandBuffer(cmd);
    const std::uint64_t value = Submit(&cmd, 1);
    const bool ok = value && WaitFor(value, UINT64_MAX);
    vkFreeCommandBuffers(b->device, b->command_pool, 1, &cmd);
    return ok;
}

bool FormatSupported(VkFormat format, VkFormatFeatureFlags features) {
    const Backend* b = CurrentBackend();
    if (!b)
        return false;
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(b->physical, format, &props);
    return (props.optimalTilingFeatures & features) == features;
}

bool DeviceIntegerValue(std::int32_t name, std::int32_t* value) {
    switch (name) {
    case kApiMajorVersion:
        *value = 55;  // any version; the game does not compare it
        return true;
    case kApiMinorVersion:
        *value = 0;
        return true;
    case kProgramMemoryAlignment:
        *value = 256;  // Vulkan's largest minUniformBufferOffsetAlignment; the pools align to 0x1000 anyway
        return true;
    case kTextureDescriptorSize:
    case kSamplerDescriptorSize:
        *value = 32;  // the pools are memory the game sizes; the shim keeps descriptors itself
        return true;
    case kReservedTextureDescriptors:
    case kReservedSamplerDescriptors:
        *value = 256;
        return true;
    case kCommandBufferCommandAlignment:
        *value = 4;
        return true;
    case kCommandBufferControlAlignment:
        *value = 8;
        return true;
    case kMaxTexturePoolSize:
        *value = 1 << 20;
        return true;
    case kMaxSamplerPoolSize:
        *value = 1 << 16;  // above the 0x2000 + reserved the game asks for (fn_710015f100)
        return true;
    case kQueueCommandMemoryDefaultSize:
        *value = 0x10000;
        return true;
    case kLinearTextureStrideAlignment:
        *value = 32;  // the shim copies linear texels into its own images; any alignment works
        return true;
    default:
        *value = 0;
        return false;
    }
}

namespace {

struct DeviceBuilder {
    std::int32_t flags = 0;
};
struct Device {
    std::int32_t depth_mode = 0;
    std::int32_t window_origin_mode = 0;
    int fast_clear_colors = 0;
    int fast_clear_depths = 0;
};
detail::Registry<DeviceBuilder> g_builders;
detail::Registry<Device> g_devices;
std::set<std::int32_t> g_unknown_integers;

void DeviceBuilderSetDefaults(void* builder) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_builders.Reset(builder);
}

void DeviceBuilderSetFlags(void* builder, std::int32_t flags) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_builders.Get(builder).flags = flags;
}

std::int32_t DeviceBuilderGetFlags(const void* builder) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    DeviceBuilder* b = g_builders.Find(builder);
    return b ? b->flags : 0;
}

// Returns NVN's boolean: the start-up ignores it.
std::int32_t DeviceInitialize(void* device, const void*) {
    if (!CreateBackend())
        return 0;
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_devices.Reset(device);
    return 1;
}

void DeviceFinalize(void* device) {
    {
        std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
        g_devices.Erase(device);
    }
    if (CurrentBackend())
        WaitFor(LastSubmitted(), UINT64_MAX);  // nothing of the shim's may be in use when it is destroyed
    detail::ReleaseDrawVulkanObjects();     // before the device they belong to goes
    detail::ReleaseTextureVulkanObjects();
    detail::ReleaseMemoryVulkanObjects();
    DestroyBackend();
}

void* DeviceGetProcAddress(const void*, const char* name) {
    return reinterpret_cast<void*>(GetProc(name));
}

void DeviceGetInteger(const void*, std::int32_t name, std::int32_t* value) {
    if (!value)
        return;
    if (!DeviceIntegerValue(name, value)) {
        std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
        if (g_unknown_integers.insert(name).second)
            std::fprintf(stderr, "nvn shim: nvnDeviceGetInteger(%#x) is not known (answers 0)\n", static_cast<unsigned>(name));
    }
}

// The depth range and window origin modes (fn_71001587f0 sets 0 and 1): kept for the pipelines and
// the viewport transform, which will read them.
void DeviceSetDepthMode(void* device, std::int32_t mode) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_devices.Get(device).depth_mode = mode;
}
void DeviceSetWindowOriginMode(void* device, std::int32_t mode) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_devices.Get(device).window_origin_mode = mode;
}
std::int32_t DeviceGetDepthMode(const void* device) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    Device* d = g_devices.Find(device);
    return d ? d->depth_mode : 0;
}
std::int32_t DeviceGetWindowOriginMode(const void* device) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    Device* d = g_devices.Find(device);
    return d ? d->window_origin_mode : 0;
}

// Fast-clear registrations are a hint to the console's hardware; Vulkan has nothing to register.
void DeviceRegisterFastClearColor(void* device, const float*, std::int32_t) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_devices.Get(device).fast_clear_colors++;
}
void DeviceRegisterFastClearDepth(void* device, float) {
    std::lock_guard<std::recursive_mutex> lock(detail::StateLock());
    g_devices.Get(device).fast_clear_depths++;
}

std::uint64_t DeviceGetCurrentTimestampInNanoseconds(const void*) {
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}

void DeviceSetDebugLabel(void*, const char*) {}

}  // namespace

void detail::RegisterDeviceProcs() {
    Register("nvnDeviceBuilderSetDefaults", &DeviceBuilderSetDefaults);
    Register("nvnDeviceBuilderSetFlags", &DeviceBuilderSetFlags);
    Register("nvnDeviceBuilderGetFlags", &DeviceBuilderGetFlags);
    Register("nvnDeviceInitialize", &DeviceInitialize);
    Register("nvnDeviceFinalize", &DeviceFinalize);
    Register("nvnDeviceGetProcAddress", &DeviceGetProcAddress);
    Register("nvnDeviceGetInteger", &DeviceGetInteger);
    Register("nvnDeviceSetDepthMode", &DeviceSetDepthMode);
    Register("nvnDeviceSetWindowOriginMode", &DeviceSetWindowOriginMode);
    Register("nvnDeviceGetDepthMode", &DeviceGetDepthMode);
    Register("nvnDeviceGetWindowOriginMode", &DeviceGetWindowOriginMode);
    Register("nvnDeviceRegisterFastClearColor", &DeviceRegisterFastClearColor);
    Register("nvnDeviceRegisterFastClearDepth", &DeviceRegisterFastClearDepth);
    Register("nvnDeviceGetCurrentTimestampInNanoseconds", &DeviceGetCurrentTimestampInNanoseconds);
    Register("nvnDeviceSetDebugLabel", &DeviceSetDebugLabel);
}

}  // namespace dread::nvn
