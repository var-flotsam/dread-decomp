// Unit 260 (render passes, no reliable name): creates the render owner's targets for a screen size, the
// downscaled chains among them, and resets its three bind lists. Names and layouts are placeholders.
#include "game/UnkTU179.h"
#include "types.h"

void* fn_710015abd0();

struct __attribute__((packed)) UnkPackedU64_1d9c28 {
    u64 v;
};

// A count, packed 8-byte entries and a trailing pointer.
struct UnkBindList_1d9c28 {
    u32 count;
    UnkPackedU64_1d9c28 items[16];
    void* tail;

    void PushBack(u64 value) {
        items[count].v = value;
        count++;
    }
};
static_assert(sizeof(UnkBindList_1d9c28) == 0x90, "UnkBindList_1d9c28 layout");

struct UnkRenderOwner_1d9c28 {
    u8 pad_0x0[0x1d0];
    UnkBindList_1d9c28 list_0x1d0;  // 0x1d0
    UnkHandle* slots_0x260[3];  // 0x260
    UnkBindList_1d9c28 list_0x278;  // 0x278
    UnkBindList_1d9c28 list_0x308;  // 0x308
    UnkHandle* half_0x398[4];  // 0x398
    UnkHandle* chains_0x3b8[3][4];  // 0x3b8
    UnkHandle* slot_0x418;  // 0x418
    u8 pad_0x420[0x20];
    UnkHandle* quarter_0x440[4];  // 0x440
    UnkHandle* slot_0x460;  // 0x460
    UnkHandle* slot_0x468;  // 0x468
    UnkHandle* slots_0x470[3];  // 0x470
    u8 pad_0x488[0x8];
    UnkHandle* slot_0x490;  // 0x490
    UnkHandle* slot_0x498;  // 0x498
    u8 pad_0x4a0[0xa9a8];
    UnkHandle* slot_0xae48;  // 0xae48
};
static_assert(__builtin_offsetof(UnkRenderOwner_1d9c28, half_0x398) == 0x398, "UnkRenderOwner_1d9c28 layout");
static_assert(__builtin_offsetof(UnkRenderOwner_1d9c28, slot_0x418) == 0x418, "UnkRenderOwner_1d9c28 layout");
static_assert(__builtin_offsetof(UnkRenderOwner_1d9c28, slot_0xae48) == 0xae48, "UnkRenderOwner_1d9c28 layout");

// Each target's format is its own local: the callee takes it by reference. The chains step down in size.
void fn_71001d9c28(UnkRenderOwner_1d9c28* self, u32 width, u32 height) {
    alignas(8) s32 formatColor0 = 0x12;
    fn_710015c1e0(&self->slots_0x260[0], width, height, 3, formatColor0);
    alignas(8) s32 formatColor1 = 0x12;
    fn_710015c1e0(&self->slots_0x260[1], width, height, 3, formatColor1);
    alignas(8) s32 formatColor2 = 0x12;
    fn_710015c1e0(&self->slots_0x260[2], width, height, 3, formatColor2);
    alignas(8) s32 format498 = 0x12;
    fn_710015c1e0(&self->slot_0x498, width, height, 5, format498);
    alignas(8) s32 format460 = 0x12;
    fn_710015c1e0(&self->slot_0x460, width, height, 11, format460);
    for (u32 i = 0; i < 3; i++) {
        alignas(8) s32 format = 0x12;
        fn_710015c1e0(&self->slots_0x470[i], width, height, 11, format);
    }
    alignas(8) s32 format490 = 0x12;
    fn_710015c1e0(&self->slot_0x490, 960, 540, 11, format490);
    alignas(8) s32 formatHalf0 = 0x12;
    fn_710015c1e0(&self->half_0x398[0], 960, 540, 3, formatHalf0);
    alignas(8) s32 formatHalf1 = 0x12;
    fn_710015c1e0(&self->half_0x398[1], 960, 540, 3, formatHalf1);
    alignas(8) s32 formatHalf2 = 0x12;
    fn_710015c1e0(&self->half_0x398[2], 960, 540, 11, formatHalf2);
    alignas(8) s32 formatHalf3 = 0x12;
    fn_710015c1e0(&self->half_0x398[3], 960, 540, 11, formatHalf3);
    for (u32 i = 0; i < 4; i++) {
        alignas(8) s32 format = 0x12;
        fn_710015c1e0(&self->quarter_0x440[i], width / 2, height / 2, 9, format);
    }
    static const s32 chainSizes[3][2] = {{320, 180}, {80, 45}, {10, 6}};
    for (u32 level = 0; level < 3; level++) {
        UnkHandle** chain = self->chains_0x3b8[level];
        s32 w = chainSizes[level][0];
        s32 h = chainSizes[level][1];
        alignas(8) s32 format0 = 0x12;
        fn_710015c1e0(&chain[0], w, h, 11, format0);
        alignas(8) s32 format1 = 0x12;
        fn_710015c1e0(&chain[1], w, h, 11, format1);
        alignas(8) s32 format2 = 0x12;
        fn_710015c1e0(&chain[2], w, h, 11, format2);
        alignas(8) s32 format3 = 0x12;
        fn_710015c1e0(&chain[3], w, h, 11, format3);
    }
    alignas(8) s32 format418 = 0x12;
    fn_710015c1e0(&self->slot_0x418, width, height, 3, format418);

    self->list_0x278.count = 0;
    self->list_0x308.count = 0;
    self->list_0x1d0.items[0].v = reinterpret_cast<u64>(self->slots_0x260[0]);
    self->list_0x1d0.items[1].v = reinterpret_cast<u64>(self->slots_0x260[1]);
    self->list_0x1d0.items[2].v = reinterpret_cast<u64>(self->slots_0x260[2]);
    self->list_0x1d0.items[3].v = reinterpret_cast<u64>(self->slot_0x498);
    self->list_0x1d0.items[4].v = 0;
    self->list_0x1d0.items[5].v = 0;
    self->list_0x1d0.items[6].v = 0;
    self->list_0x1d0.items[7].v = 0;
    self->list_0x1d0.count = 8;
    self->list_0x1d0.tail = fn_710015abd0();
    self->list_0x278.PushBack(0);
    self->list_0x308.PushBack(0);
    self->list_0x308.PushBack(0);

    alignas(8) s32 formatAe48 = 0x1a;
    fn_710015c1e0(&self->slot_0xae48, 0x200, 1, 5, formatAe48);
}
