// nn::mem for the port: StandardAllocator over the host's heap. Each block carries a header with
// the host pointer, its size and its alignment, so that Reallocate keeps the alignment and Free
// needs nothing else. Every stub names the boot-path function that depends on it in a TODO.

#include "nn/mem.h"

#include <cstdlib>
#include <cstring>

namespace nn::mem {

namespace {

constexpr u64 kInitialised = 0x445253414c4c4f43;  // 'DRSALLOC'
constexpr u64 kMinimumAlignment = 16;

struct Header {
    void* host;
    u64 size;
    u64 alignment;
};

Header* HeaderOf(void* pointer) { return static_cast<Header*>(pointer) - 1; }

void* AllocateAligned(u64 size, u64 alignment) {
    if (alignment < kMinimumAlignment)
        alignment = kMinimumAlignment;
    if (alignment & (alignment - 1))
        return nullptr;  // not a power of two
    void* host = std::malloc(size + alignment + sizeof(Header));
    if (!host)
        return nullptr;
    const u64 start = reinterpret_cast<u64>(host) + sizeof(Header);
    auto* user = reinterpret_cast<u8*>((start + alignment - 1) & ~(alignment - 1));
    *HeaderOf(user) = Header{host, size, alignment};
    return user;
}

}  // namespace

StandardAllocator::StandardAllocator() : m_state(0) {
    // TODO(M1): the static initialiser fn_7100132b80 constructs the game's one allocator at
    // 0x710ab70000 (in the mapped .bss) and registers its destructor.
}

void StandardAllocator::Initialize(void*, u64) {
    // TODO(M1): fn_71001325d0, over a 32 MB region of the game's (unused: the host's heap serves).
    m_state = kInitialised;
}

void StandardAllocator::Finalize() {
    // TODO(M1): the shutdown, through fn_71001322a0. Blocks still allocated stay with the host.
    m_state = 0;
}

void* StandardAllocator::Allocate(u64 size, u64 alignment) {
    // TODO(M1): nv's allocation callback fn_71001322b0.
    if (m_state != kInitialised)
        __builtin_trap();  // the game initialises it before handing it to nv
    return AllocateAligned(size, alignment);
}

void StandardAllocator::Free(void* pointer) {
    // TODO(M1): nv's free callback fn_71001322d0.
    if (pointer)
        std::free(HeaderOf(pointer)->host);
}

void* StandardAllocator::Reallocate(void* pointer, u64 size) {
    // TODO(M1): nv's reallocation callback fn_71001322e0.
    if (!pointer)
        return Allocate(size, kMinimumAlignment);
    const Header old = *HeaderOf(pointer);
    void* moved = AllocateAligned(size, old.alignment);
    if (!moved)
        return nullptr;  // the old block stays valid, as realloc's does
    std::memcpy(moved, pointer, old.size < size ? old.size : size);
    std::free(old.host);
    return moved;
}

}  // namespace nn::mem
