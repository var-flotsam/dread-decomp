// CHecathonAIComponent unit (tools/tu_map.py, the name from reflection): the constructor. The base AI
// component's constructor (fn_710036932c), the class's vtable (0x7101a4f7f0), then its own state from
// +0x146c8: a pair of 1.0 scales, two allocator-backed lists (the 0x48-byte object of
// src/game/CAIComponent/UnkAIDeviceList.cpp, base vtable 0x7101a20998, final 0x7101a20950) and two
// cleared runs, then the virtual at slot 3. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkComponent;
void fn_710036932c(UnkComponent* self);
extern void* g_unk_7101a4f7f0[];  // the component's vtable
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];

struct UnkHecathonList_b55 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    base::UnkAllocator* field_0x28;
    u32 field_0x30;
    u64 field_0x38;
    u64 field_0x40;

    __attribute__((always_inline)) void Construct() {
        vtable = g_unk_7101a20998 + 2;
        field_0x8 = 0;
        field_0x10 = 0;
        field_0x18 = 10;
        field_0x1c = true;
        field_0x20 = 15;
        field_0x28 = fn_7100083990();
        field_0x30 = 0;
        vtable = g_unk_7101a20950 + 2;
        field_0x38 = 0;
    }
};

// The state at +0x146c8 (offsets relative to it).
struct UnkHecathonState_b55 {
    f32 scale[2];
    u8 pad_0x8[8];
    UnkHecathonList_b55 list0;  // 0x10
    UnkHecathonList_b55 list1;  // 0x58
    u8 pad_0xa0[0x20];
    u8 field_0xc0[13];
    u8 pad_0xcd[0x43];
    u64 field_0x110[2];
};

struct UnkHecathonAI_b55 {
    void** vtable;
};

void fn_71005abd7c(UnkHecathonAI_b55* self) {
    UnkHecathonState_b55* state =
        reinterpret_cast<UnkHecathonState_b55*>(reinterpret_cast<u8*>(self) + 0x146c8);
    fn_710036932c(reinterpret_cast<UnkComponent*>(self));
    self->vtable = g_unk_7101a4f7f0 + 2;
    state->scale[0] = 1.0f;
    state->scale[1] = 1.0f;
    state->list0.Construct();
    state->list1.Construct();
    u8* bytes = state->field_0xc0;
    u64* words = state->field_0x110;
    __builtin_memset(bytes, 0, 13);
    words[0] = 0;
    words[1] = 0;
    reinterpret_cast<void (*)(UnkHecathonAI_b55*)>(self->vtable[3])(self);
}
