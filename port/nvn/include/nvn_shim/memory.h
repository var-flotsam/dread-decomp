#pragma once

// NVN memory pools and buffers over Vulkan. The game gives every memory pool its own storage
// (fn_710015385c: 4 KB-aligned memory from its allocator, MemoryPoolBuilderSetStorage) and
// suballocates it itself (fn_7100153a10, fn_7100153968); buffers name a pool, an offset and a size
// (the shader loader fn_710015c9b0: BufferBuilderSetStorage, BufferInitialize, BufferGetAddress,
// BufferMap). So the shim:
//  - gives each pool a range of a GPU virtual address space of its own (from 4 GB up, 64 KB
//    aligned); a buffer's address is its pool's address plus its offset, which is what
//    nvnBufferGetAddress returns and nvnCommandBufferBindUniformBuffer later takes;
//  - backs each pool with a Vulkan buffer, made when the GPU first needs the pool (ResolveGpuAddress,
//    SyncPoolRange): where the device can import host memory (VK_EXT_external_memory_host), the
//    buffer is the game's own storage, so the GPU reads what the game writes, as on the console;
//    otherwise a host-visible copy, into which the queue's submit copies each range a command
//    buffer reads before the GPU runs it (SyncPoolRange; port/nvn/README.md, "Drawing");
//  - maps a buffer to the game's own storage (nvnBufferMap returns storage + offset).
// Memory pool flags are kept, not interpreted yet (fn_710015385c's callers pass 0x22;
// fn_7100153350 passes 0x22, 0x21 and 0x24).

#include <cstddef>
#include <cstdint>

#include <vulkan/vulkan_core.h>

#include "nvn_shim/nvn_types.h"

namespace dread::nvn {

// The Vulkan buffer and offset behind a GPU address range: the pool whose range holds
// [address, address + size). False when no pool does (or the range crosses its end).
bool ResolveGpuAddress(BufferAddress address, std::uint64_t size, VkBuffer* buffer, VkDeviceSize* offset);

// Copies [address, address + size) from the game's storage into the pool's Vulkan buffer (nothing to
// copy for an imported pool).
bool SyncPoolRange(BufferAddress address, std::uint64_t size);

// The game storage behind a GPU address (null when no pool holds it): for checks.
void* GpuAddressToStorage(BufferAddress address);

// Memory pools alive, those with a Vulkan buffer made, and the GPU address space handed out (for
// checks).
std::size_t MemoryPoolCount();
std::size_t PoolsWithVulkanBuffer();
std::size_t PoolsImported();
std::uint64_t GpuAddressSpaceUsed();

}  // namespace dread::nvn
