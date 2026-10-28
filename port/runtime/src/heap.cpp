// The port's C++ heap: the replaceable global operator new and delete for the whole program.
//
// The game allocates through its own default allocator: its operator new and delete, its malloc and
// free (src/base/UnkDefaultAllocatorGlobal.cpp, src/engine/SystemStartup.cpp) and most of its code,
// which calls the allocator directly. Matched code that writes `new` or `delete`, or calls
// operator delete in a deleting destructor, names the C++ operators instead, which on the port are
// these. So once the game's allocator exists they allocate through it, as the game's own operator
// new does. Every delete goes back to the allocator a pointer came from: the game's when the pointer
// lies in the game's memory block (dread_port_in_game_heap), the host's otherwise. Before the game's
// memory block exists (the data loader, the static initialisers of the runtime) everything is the
// host's malloc. Handing the host's free a block of the game's heap aborted the boot in
// fn_7100087124 (a file object's deleting destructor).
//
// The game's functions are weak references, so the runtime still links without the game.

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>

#include "dread_port.h"

namespace base {
struct UnkAllocator;
}

void* fn_7100083b60(u64 size) __attribute__((weak));                                             // operator new
void* fn_7100083bc0(u64 size, u32 flags, u64 align, base::UnkAllocator* allocator) __attribute__((weak));
void fn_7100083c00(void* ptr) __attribute__((weak));                                              // operator delete
base::UnkAllocator* fn_7100083990() __attribute__((weak));                                        // the allocator

namespace {

// The memory block first: before the image is mapped, the allocator's pointer cannot be read.
bool GameAllocator() {
    return fn_7100083b60 && fn_7100083bc0 && fn_7100083990 && dread_port_game_heap_ready() &&
           fn_7100083990() != nullptr;
}

[[noreturn]] void OutOfMemory(std::size_t size) {
    std::fprintf(stderr, "dread: operator new: no memory for 0x%zx bytes\n", size);
    std::abort();
}

void* Allocate(std::size_t size, std::size_t align, bool nothrow) {
    if (size == 0)
        size = 1;
    void* p;
    if (GameAllocator())
        p = align <= 8 ? fn_7100083b60(size) : fn_7100083bc0(size, 2, align, nullptr);
    else
        p = align <= alignof(std::max_align_t) ? std::malloc(size)
                                              : std::aligned_alloc(align, (size + align - 1) & ~(align - 1));
    if (!p && !nothrow)
        OutOfMemory(size);
    return p;
}

void Free(void* p) {
    if (!p)
        return;
    if (fn_7100083c00 && dread_port_in_game_heap(p))
        fn_7100083c00(p);
    else
        std::free(p);
}

}  // namespace

void* operator new(std::size_t size) { return Allocate(size, 8, false); }
void* operator new[](std::size_t size) { return Allocate(size, 8, false); }
void* operator new(std::size_t size, const std::nothrow_t&) noexcept { return Allocate(size, 8, true); }
void* operator new[](std::size_t size, const std::nothrow_t&) noexcept { return Allocate(size, 8, true); }
void* operator new(std::size_t size, std::align_val_t align) { return Allocate(size, static_cast<std::size_t>(align), false); }
void* operator new[](std::size_t size, std::align_val_t align) { return Allocate(size, static_cast<std::size_t>(align), false); }
void* operator new(std::size_t size, std::align_val_t align, const std::nothrow_t&) noexcept {
    return Allocate(size, static_cast<std::size_t>(align), true);
}
void* operator new[](std::size_t size, std::align_val_t align, const std::nothrow_t&) noexcept {
    return Allocate(size, static_cast<std::size_t>(align), true);
}

void operator delete(void* p) noexcept { Free(p); }
void operator delete[](void* p) noexcept { Free(p); }
void operator delete(void* p, std::size_t) noexcept { Free(p); }
void operator delete[](void* p, std::size_t) noexcept { Free(p); }
void operator delete(void* p, const std::nothrow_t&) noexcept { Free(p); }
void operator delete[](void* p, const std::nothrow_t&) noexcept { Free(p); }
void operator delete(void* p, std::align_val_t) noexcept { Free(p); }
void operator delete[](void* p, std::align_val_t) noexcept { Free(p); }
void operator delete(void* p, std::size_t, std::align_val_t) noexcept { Free(p); }
void operator delete[](void* p, std::size_t, std::align_val_t) noexcept { Free(p); }
void operator delete(void* p, std::align_val_t, const std::nothrow_t&) noexcept { Free(p); }
void operator delete[](void* p, std::align_val_t, const std::nothrow_t&) noexcept { Free(p); }
