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
#include <functional>

#include <vulkan/vulkan_core.h>

namespace dread::nvn {

enum DeviceInteger : std::int32_t {
    // fn_71001587f0 reads both into locals it never uses.
    kApiMajorVersion = 0x00,
    kApiMinorVersion = 0x01,
    // fn_710015c9b0 (a shader program's load, from CProgramResource's fn_71001b0050) rounds each
    // program's buffer sizes and its code size plus 0x2000 up to a multiple of it before it places
    // them in its memory pools; a divisor, so never 0.
    kProgramMemoryAlignment = 0x04,
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
    // fn_710015fff0 rounds a linear texture's row stride up to a multiple of it.
    kLinearTextureStrideAlignment = 0x38,
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
    bool bc_textures = false;  // textureCompressionBC: the game's BC1/3/5/6H assets need it
    // Optional; each is used when the device has it (port/nvn/README.md, "Drawing"):
    bool depth_clip_control = false;  // VK_EXT_depth_clip_control: the game's -1..1 depth range (draw.h)
    bool host_import = false;         // VK_EXT_external_memory_host: memory pools over the game's storage
    VkDeviceSize host_import_alignment = 0;
    bool index_uint8 = false;         // VK_KHR/EXT_index_type_uint8: 8-bit indices
    bool depth_bounds = false;        // the depthBounds feature
    bool independent_blend = false;   // per-target blend state (otherwise target 0's for all)
    bool fill_mode_non_solid = false; // wireframe polygons
    VkCommandPool command_pool = VK_NULL_HANDLE;  // for the shim's own one-off work (ImmediateSubmit)
    VkSemaphore timeline = VK_NULL_HANDLE;        // Submit's timeline semaphore

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

// Records `record` into a one-off command buffer, submits it and waits for it (image layout
// transitions, buffer fills, texel uploads). False when there is no backend or the submit fails.
bool ImmediateSubmit(const std::function<void(VkCommandBuffer)>& record);

// The queue's timeline: every submit of the shim (the game's command buffers and the shim's own
// one-off work) signals the next value of one timeline semaphore, in submission order. Submit
// returns the value it signals (0 if it failed); a fence (nvnQueueFenceSync) is the last value
// submitted, and anything the GPU used is free once Completed() reaches the value that used it.
std::uint64_t Submit(const VkCommandBuffer* cmds, std::uint32_t count);
std::uint64_t LastSubmitted();
std::uint64_t Completed();
// Waits until the timeline reaches `value`; false on a timeout or a device loss.
bool WaitFor(std::uint64_t value, std::uint64_t timeout_ns);

// Whether the device can use `format` with these optimal-tiling features.
bool FormatSupported(VkFormat format, VkFormatFeatureFlags features);

// Validation messages the backend has seen (counted when validation is on).
int ValidationErrors();
int ValidationWarnings();

}  // namespace dread::nvn
