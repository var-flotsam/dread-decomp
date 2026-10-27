#pragma once

// The NVN device over Vulkan. nvnDeviceInitialize (fn_71001587f0 calls it once, after the builder's
// defaults and flags 0) creates the shim's one Vulkan instance, device and graphics queue; the game
// has one device, so the backend is a single object the other parts share.
//
// The integers the game asks nvnDeviceGetInteger for are listed in DeviceInteger with the call site
// each comes from and what the game does with the answer. The numbers are the game's own arguments;
// the names are ours (NVN's own names come from its SDK, which the port does not use). The values
// the shim answers are its own choices, safe for how the game uses them.

#include <cstdint>

#include <vulkan/vulkan_core.h>

namespace dread::nvn {

enum DeviceInteger : std::int32_t {
    // fn_71001587f0 reads both into locals it never uses.
    kApiMajorVersion = 0x00,
    kApiMinorVersion = 0x01,
    // fn_710015f100 sizes its texture and sampler pools with them: descriptor size times count.
    kTextureDescriptorSize = 0x0f,
    kSamplerDescriptorSize = 0x10,
    // fn_710015f100: the pools' first ids for the game's own use start after these.
    kReservedTextureDescriptors = 0x11,
    kReservedSamplerDescriptors = 0x12,
    // fn_71001587f0: the alignment of the 15 MB command memory pool per command buffer, and of the
    // 2 MB control memory block it allocates for each (the matched source's locals call them the
    // other way round; these names follow what each value aligns).
    kCommandBufferCommandAlignment = 0x13,
    kCommandBufferControlAlignment = 0x14,
    // fn_710015f100: the pools hold min(reserved + 0x800, this) textures and min(reserved + 0x2000,
    // this) samplers.
    kMaxTexturePoolSize = 0x24,
    kMaxSamplerPoolSize = 0x25,
    // fn_71001587f0: the queue's command memory size and its flush threshold.
    kQueueCommandMemoryDefaultSize = 0x51,
};

// The shim's answer for a DeviceInteger (0 and false for a name it does not know).
bool DeviceIntegerValue(std::int32_t name, std::int32_t* value);

struct Backend {
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physical = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;
    std::uint32_t queue_family = 0;
    VkPhysicalDeviceProperties properties{};
    VkPhysicalDeviceMemoryProperties memory{};
    bool validation = false;

    // A memory type with `flags` among `type_bits`, or UINT32_MAX.
    std::uint32_t MemoryType(std::uint32_t type_bits, VkMemoryPropertyFlags flags) const;
};

// The backend once nvnDeviceInitialize has run, else null.
const Backend* CurrentBackend();

// Creates the backend (nvnDeviceInitialize does; checks may call it first). The validation layers
// are on when $DREAD_NVN_VALIDATION is 1 or `validation` is true. Needs Vulkan 1.3 with
// dynamicRendering and maintenance4 (port/nvn/README.md). False with a message on stderr if no
// device qualifies.
bool CreateBackend(bool validation = false);
void DestroyBackend();

// Validation messages the backend has seen (counted when validation is on).
int ValidationErrors();
int ValidationWarnings();

}  // namespace dread::nvn
