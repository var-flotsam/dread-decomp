// Unit 259 (UnkGraphicsSystem, src/engine/UnkGraphicsSystem.cpp): uploads one draw item's uniforms
// and its buffer ranges. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "engine/UnkGraphicsSystem.h"
#include "game/UnkTU174.h"
#include "game/UnkTU177.h"
#include "game/UnkTU180.h"
#include "game/UnkTU181.h"
#include "game/UnkTU182.h"
#include "game/UnkTU230.h"
#include "types.h"

struct UnkGfxRange_b0155 {
    u64 data;  // 0x0
    u32 field_0x8;
    u32 field_0xc;
    u32 elements;  // 0x10
    u8 unk_0x14[0xc];
    u32 field_0x20;
    u8 unk_0x24[0x4];
};
static_assert(sizeof(UnkGfxRange_b0155) == 0x28, "range size");

struct UnkGfxItem_b0155 {
    void* field_0x0;
    void* field_0x8;
    u8 unk_0x10[0x4];
    u32 count;  // 0x14
    UnkGfxRange_b0155* ranges;  // 0x18
    u8 unk_0x20[0xf4];
    f32 values[4];  // 0x114
    u32 flags;  // 0x124
};
static_assert(__builtin_offsetof(UnkGfxItem_b0155, flags) == 0x124, "item layout");

struct UnkGraphicsSystem {
    u8 unk_0x0[0x317ab28];
    u64 field_0x317ab28;  // a matrix pointer
    u8 unk_0x317ab30[0x2a0];
    u32 field_0x317add0;
    u8 unk_0x317add4[0x14];
    u64 field_0x317ade8;
    u8 unk_0x317adf0[0xa0];
    u64 field_0x317ae90;
    u8 unk_0x317ae98[0xd0];
    void* field_0x317af68;  // the program state
    void* field_0x317af70;  // the program
};
static_assert(__builtin_offsetof(UnkGraphicsSystem, field_0x317af70) == 0x317af70, "system layout");

void fn_710015e2d0(void* state, u32 stage, u64 location, u32 a, u32 count, u64 data);
void fn_71001d2eec(UnkGraphicsSystem* self, u32 id, u32 flag);

void fn_71001d03f8(UnkGraphicsSystem* self, UnkGfxItem_b0155* item, bool upload, bool draw) {
    u32 count = item->count;
    u64 location;
    if (fn_71001aa0b4(self->field_0x317af70, 0, 0xe8, &location)) {
        u64 lo;
        __builtin_memcpy(&lo, &item->values[0], 8);
        u32 w2, w3;
        __builtin_memcpy(&w2, &item->values[2], 4);
        __builtin_memcpy(&w3, &item->values[3], 4);
        struct {
            u64 lo;
            u32 w2, w3;
        } values = {lo, w2, w3};
        fn_710015e1f0(self->field_0x317af68, 0, location, 4, reinterpret_cast<const f32*>(&values));
    }
    if (fn_71001aa0b4(self->field_0x317af70, 0, 0xe9, &location))
        fn_710015e020(self->field_0x317af68, 0, location, item->flags & 1);
    if (fn_71001aa0b4(self->field_0x317af70, 0, 0xea, &location))
        fn_710015e020(self->field_0x317af68, 0, location, (item->flags >> 1) & 1);
    if (self->field_0x317ade8) {
        const UnkMatrix44* matrix = reinterpret_cast<const UnkMatrix44*>(self->field_0x317ab28);
        fn_710015e4b0(self->field_0x317af68, 0, self->field_0x317ade8, 4, 1, matrix, 1);
    }
    if (upload) {
        fn_7100161800(item->field_0x8);
        fn_7100154420(item->field_0x0);
    }
    if (!count)
        return;
    if (draw) {
        for (u32 i = 0; i < count; ++i) {
            UnkGfxRange_b0155* range = &item->ranges[i];
            if (self->field_0x317ae90) {
                u32 elements = range->elements;
                void* state = self->field_0x317af68;
                u64 data = range->data;
                fn_710015e2d0(state, 0, self->field_0x317ae90, 4, elements * 3, data);
                fn_71001d2eec(self, range->field_0x20, 0);
            }
            u32 a = range->field_0x8, b = range->field_0xc;
            if (fn_7100159c80()) {
                self->field_0x317add0++;
                fn_7100159f90(a, b);
            }
        }
    } else {
        for (u32 i = 0; i < count; ++i) {
            if (self->field_0x317ae90) {
                UnkGfxRange_b0155* range = &item->ranges[i];
                fn_710015e2d0(self->field_0x317af68, 0, self->field_0x317ae90, 4, range->elements * 3, range->data);
                fn_71001d2eec(self, range->field_0x20, 0);
            }
        }
    }
}
