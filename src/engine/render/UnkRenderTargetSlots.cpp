// From unit 329 of tools/tu_map.py (0x71002236ac-0x7100224d44, no class hint): the second render
// singleton (0x1278 bytes, built by fn_71002236ac; src/game/UnkRenderManager.cpp creates it) and the
// render-target slots it sets up. Names and layouts are placeholders.
// Holds fn_7100223c2c.
#include "base/UnkDefaultAllocator.h"
#include "base/UnkVector.h"
#include "types.h"

// A render target's description: what fn_710015f810 fills in, then its size and a flag.
struct UnkRenderTargetSlot {
    u64 field_0x0;
    u32 width;
    u32 height;
    bool field_0x10;
};
static_assert(sizeof(UnkRenderTargetSlot) == 0x18, "UnkRenderTargetSlot placeholder layout");

// Fills a target's description from its format, size and options.
void fn_710015f810(UnkRenderTargetSlot* out, u32 format, u32 width, u32 height, u32 a, void* b, void* c, u32 d,
                   u32 e);

// The slots' array (base::UnkVector's layout). Its elements are copied as raw words (the original's
// stores may alias the array's own fields: its count is read again after each one).
struct UnkRenderTargetSlots : base::UnkVector<UnkRenderTargetSlot> {
    __attribute__((always_inline)) void Grow(u32 n) {
        auto* p = static_cast<UnkRenderTargetSlot*>(allocator->vf_0x40((u64)n * sizeof(UnkRenderTargetSlot), 8, flags));
        if (data) {
            UnkRenderTargetSlot* dst = p;
            const UnkRenderTargetSlot* src = data;
            for (u32 i = 0; i < count; ++i)
                __builtin_memcpy(dst++, src++, sizeof(UnkRenderTargetSlot));
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }
    __attribute__((always_inline)) void Append(const UnkRenderTargetSlot& slot) {
        u32 c = count;
        u32 cap = capacity;
        if (cap <= c && step != 0)
            cap = (c / step + 1) * step;
        if (!((s32)c < (s32)cap))
            for (;;) {
            }
        if (capacity < cap)
            Grow(cap);
        __builtin_memcpy(&data[count], &slot, sizeof(slot));
        ++count;
    }
};

struct UnkRenderTargetOwner {
    u8 pad_0x0[0x1228];
    UnkRenderTargetSlots slots;  // 0x1228
    nn::os::MutexType mutex;                     // 0x1250
};
static_assert(__builtin_offsetof(UnkRenderTargetOwner, mutex) == 0x1250, "UnkRenderTargetOwner placeholder layout");

__attribute__((always_inline)) static inline void AddSlot(UnkRenderTargetOwner* self, u32 width, u32 height) {
    UnkRenderTargetSlot slot;
    slot.field_0x10 = false;
    slot.field_0x0 = 0;
    slot.width = 0;
    slot.height = 0;
    fn_710015f810(&slot, 6, width, height, 0, nullptr, nullptr, 1, 0);
    slot.width = width;
    slot.height = height;
    slot.field_0x10 = false;
    self->slots.Append(slot);
}

// Sets up the mutex and the eight render-target slots: three full-size (1280x720), two of 512x288
// and three of 256x144, all in format 6.
void fn_7100223c2c(UnkRenderTargetOwner* self) {
    fn_71000668f8(&self->mutex);
    AddSlot(self, 0x500, 0x2d0);
    AddSlot(self, 0x500, 0x2d0);
    AddSlot(self, 0x500, 0x2d0);
    AddSlot(self, 0x200, 0x120);
    AddSlot(self, 0x200, 0x120);
    AddSlot(self, 0x100, 0x90);
    AddSlot(self, 0x100, 0x90);
    AddSlot(self, 0x100, 0x90);
}
