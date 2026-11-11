// CWaterPlatformUsableComponent's unit: hands the component's block at +0x5c8 to fn_710110fae4 once per
// entry of the Emmy registry's inline list (registry +0x8: a u32 count followed directly by the
// pointers), passing each entry's +0x10 (null for an empty slot); fn_7100aa5718 does the same through
// fn_710110f97c unless its flag at +0x5c5 is set. Names and layouts are placeholders.
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkEmmyRegistry_b55;
extern UnkEmmyRegistry_b55* g_unk_71726af0a8;

void fn_710110f97c(void* emmy, void* block);

struct UnkEmmyEntry_A5_b69 {
    u8 pad_0x0[0x10];
    void* emmy;
};

#pragma pack(push, 4)
struct UnkEmmyRegistry_A5_b69 {
    u8 pad_0x0[0x8];
    u32 count;
    UnkEmmyEntry_A5_b69* items[1];  // 0xc, 4-byte aligned
};
#pragma pack(pop)

struct UnkWaterPlatformUsable_A5_b69 {
    u8 pad_0x0[0x5c5];
    bool field_0x5c5;
    u8 pad_0x5c6[2];
    u8 block[1];
};

template <void (*Notify)(void*, void*)>
static inline __attribute__((always_inline)) void NotifyAll(UnkWaterPlatformUsable_A5_b69* self) {
    UnkEmmyRegistry_A5_b69* registry = reinterpret_cast<UnkEmmyRegistry_A5_b69*>(g_unk_71726af0a8);
    u64 bytes = (u64)registry->count * sizeof(void*);
    asm("" : "+r"(bytes));
    for (UnkEmmyEntry_A5_b69 **it = registry->items, **end = (UnkEmmyEntry_A5_b69**)((char*)it + bytes); it != end; ++it)
        Notify(*it ? (*it)->emmy : nullptr, self->block);
}

// Only while the flag at +0x5c5 is clear, through fn_710110f97c.
void fn_7100aa5718(UnkWaterPlatformUsable_A5_b69* self) {
    if (!self->field_0x5c5)
        NotifyAll<fn_710110f97c>(self);
}

void fn_7100aa5790(UnkWaterPlatformUsable_A5_b69* self) { NotifyAll<fn_710110fae4>(self); }
