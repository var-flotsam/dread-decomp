// Unit 329 of tools/tu_map.py (next to media::CMovieStreamNx; src/engine/render/UnkRenderTargetSlots.cpp
// has the same object): the second render singleton's pool of render targets (0x18-byte slots at
// +0x1228): the slot accessor that grows the array, taking a free target of a size, the constructor and
// the destructor. Names are placeholders.
// The array accessor is inlined into fn_7100224910.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include <nn/os.h>
#include <new>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkVector.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU182.h"
#include "types.h"

// A pooled slot (0x18 bytes): the object it holds, a two-word key and an in-use flag.
struct UnkPoolSlot_ovb0224 {
    void* object;
    u32 key_a;
    u32 key_b;
    bool in_use;  // 0x10

    UnkPoolSlot_ovb0224() : object(nullptr), key_a(0), key_b(0), in_use(false) {}
    UnkPoolSlot_ovb0224(UnkPoolSlot_ovb0224&& o) { __builtin_memcpy(this, &o, sizeof(*this)); }
};

// The slot at the index, growing the array to hold it.
__attribute__((noinline)) UnkPoolSlot_ovb0224* fn_7100224910(base::UnkVector<UnkPoolSlot_ovb0224>* self, u32 index) {
    return &self->GrowAt(index);
}

struct UnkSlotPool_ovb0224 {
    void* vtable;
    u8 unk_0x8[0x60];
    u8 field_0x68[0x780];
    u8 field_0x7e8[0x9f8];
    u8 queue[0x48];                           // 0x11e0
    base::UnkVector<UnkPoolSlot_ovb0224> slots;  // 0x1228
    nn::os::MutexType mutex;                  // 0x1250
};

// Takes the object of the first free slot with the given size (marking it in use), or null.
struct UnkPlayer;

void* fn_7100224b00(UnkPlayer* owner, s32 key_a, s32 key_b) {
    UnkSlotPool_ovb0224* self = reinterpret_cast<UnkSlotPool_ovb0224*>(owner);
    nn::os::LockMutex(&self->mutex);
    void* result = nullptr;
    for (u32 i = 0; i < self->slots.count && result == nullptr; i++) {
        UnkPoolSlot_ovb0224* slot = fn_7100224910(&self->slots, i);
        if (slot->in_use == false && slot->key_a == key_a && slot->key_b == key_b) {
            result = slot->object;
            slot->in_use = true;
        }
    }
    fn_7100066920(&self->mutex);
    return result;
}

struct UnkMovieUnit327_b50;
struct UnkMovieUnit322_b50;
void fn_7100222bb0(UnkMovieUnit327_b50* self);
void fn_710021c808(UnkMovieUnit322_b50* self);
void fn_7100081378(void* allocator);
extern char g_unk_7101a20630[];  // the pool owner's vtable base

// The unit's statics: the "in use" flag, the mutex and two sub-allocators.
struct UnkPoolStatics_ovb0224 {
    u8 in_use;
    u8 unk_0x1[0x17];
    nn::os::MutexType mutex;  // 0x18
    base::UnkSubAllocator allocator_a;  // 0x38, "VideoNX_Big"
    base::UnkSubAllocator allocator_b;  // 0x70, "VideoNX_Small"
};
static_assert(__builtin_offsetof(UnkPoolStatics_ovb0224, allocator_b) == 0x70, "statics layout");
extern UnkPoolStatics_ovb0224 g_unk_7172699f08 __attribute__((visibility("hidden")));


// The destructor: finishes the thread and queue, releases every pooled object and the statics,
// frees the slots and ends in the base destructor.
void fn_71002245ec(UnkSlotPool_ovb0224* self) {
    self->vtable = g_unk_7101a20630 + 0x10;
    fn_710006686c(self->field_0x7e8);
    nn::os::FinalizeMessageQueue(reinterpret_cast<nn::os::MessageQueueType*>(self->queue));
    for (u32 i = 0; i < self->slots.count; ++i) {
        UnkPoolSlot_ovb0224* slot = fn_7100224910(&self->slots, i);
        if (slot->object) {
            fn_7100160590(reinterpret_cast<u64>(slot->object));
            slot->object = nullptr;
        }
        slot->in_use = false;
        slot->key_a = 0;
        slot->key_b = 0;
    }
    fn_7100066918(&self->mutex);
    self->slots.count = 0;
    if (g_unk_7172699f08.in_use == 1) {
        g_unk_7172699f08.in_use = 0;
        fn_7100066918(&g_unk_7172699f08.mutex);
        fn_7100081378(&g_unk_7172699f08.allocator_a);
        fn_7100081378(&g_unk_7172699f08.allocator_b);
    }
    // Reached by offset, as the original does (a separate address calculation from the loop's).
    reinterpret_cast<base::UnkVector<UnkPoolSlot_ovb0224>*>(reinterpret_cast<u8*>(self) + 0x1228)->Free();
    fn_7100222bb0(reinterpret_cast<UnkMovieUnit327_b50*>(self->field_0x68));
    fn_710021c808(reinterpret_cast<UnkMovieUnit322_b50*>(self));
}

namespace movie {
void SetAllocator(void* (*allocate)(u64, u64, void*), void (*deallocate)(void*, void*),
                  void* (*reallocate)(void*, u64, void*), void* user);
}
void fn_710021c4cc(UnkSlotPool_ovb0224* self);
void fn_7100222970(void* list);
// The movie library's allocator callbacks: unit statics.
__attribute__((visibility("hidden"))) void* fn_71002237f0(u64 size, u64 align, void* user);
__attribute__((visibility("hidden"))) void fn_7100223860(void* pointer, void* user);
__attribute__((visibility("hidden"))) void* fn_7100223980(void* pointer, u64 size, void* user);
void fn_7100223b8c(void* arg);
struct UnkRenderTargetOwner;
void fn_7100223c2c(UnkRenderTargetOwner* self);
void fn_7100066744(void* thread, const char* name, void (*entry)(void*), u32 a, u32 b, void* arg);
extern char g_unk_716e599f08[] __attribute__((visibility("hidden")));
extern char g_unk_7171799f08[] __attribute__((visibility("hidden")));

void fn_71002236ac(UnkPlayer* player) {
    UnkSlotPool_ovb0224* self = reinterpret_cast<UnkSlotPool_ovb0224*>(player);
    fn_710021c4cc(self);
    self->vtable = g_unk_7101a20630 + 0x10;
    fn_7100222970(self->field_0x68);
    __builtin_memset(reinterpret_cast<u8*>(self) + 0x9cc, 0, 0x14);
    void* thread = self->field_0x7e8;
    new (&self->slots) base::UnkVector<UnkPoolSlot_ovb0224>(0xf);
    if (!(g_unk_7172699f08.in_use & 1)) {
        fn_7100081330(&g_unk_7172699f08.allocator_a, g_unk_716e599f08,
                      g_unk_716e599f08 + 0x3200000, "VideoNX_Big", 0);
        fn_7100081330(&g_unk_7172699f08.allocator_b, g_unk_7171799f08,
                      g_unk_7171799f08 + 0xf00000, "VideoNX_Small", 0);
        g_unk_7172699f08.in_use = 1;
        fn_71000668f8(&g_unk_7172699f08.mutex);
    }
    movie::SetAllocator(fn_71002237f0, fn_7100223860, fn_7100223980, nullptr);
    nn::os::InitializeMessageQueue(reinterpret_cast<nn::os::MessageQueueType*>(self->queue),
                                   reinterpret_cast<u64*>(reinterpret_cast<u8*>(self) + 0x9e0), 0x100);
    fn_7100066744(thread, "MovieInputHandler", fn_7100223b8c, 0, 0x10, self);
    fn_7100223c2c(reinterpret_cast<UnkRenderTargetOwner*>(self));
}
