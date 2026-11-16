// Unit 177 (the graphics state of game/UnkGraphicsState.h): applies a blend description as the current one,
// unless the top of the blend stack locks it, pushing each changed value to NVN.
// Names and layouts are placeholders.
#include "game/UnkGraphicsState.h"
#include "game/UnkTU172.h"
#include "game/UnkTU176.h"
#include "game/UnkTU177.h"
#include "types.h"

struct UnkBlendDesc_ovb0098 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u8 field_0x18;
    u8 field_0x19;
};

struct UnkBlendStack_l15acf0 {
    u8 applied;  // 0x0
    u8 pad_0x1[7];
    u32 count;  // 0x8
    u8 pad_0xc[0x24];
    UnkBlendDesc_ovb0098 base;  // 0x30
    UnkBlendDesc_ovb0098 current;  // 0x4c
    UnkBlendDesc_ovb0098 entries[0x3f];  // 0x68
};
static_assert(__builtin_offsetof(UnkBlendStack_l15acf0, entries) == 0x68, "");

extern UnkBlendStack_l15acf0 g_unk_716e29c6d0 __attribute__((visibility("hidden")));
extern s32 g_unk_716e29c694;
extern void (*g_unk_716e29b6b0)(void* object, s32 which, s32 value);
extern s32 g_unk_716e29c240;
extern void (*g_unk_716e29b9b8)(void* context, s32 which, s32 value);
extern s32 g_unk_716e29c244;
extern void (*g_unk_716e29b9b0)(void* context, s32 which, s32 value);
void fn_710015afd0(s32 a, s32 b, s32 c);

static inline void MarkDirty() {
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7e4 = true;
}

void fn_710015acf0(const UnkBlendDesc_ovb0098* desc) {
    u32 count = g_unk_716e29c6d0.count;
    const u8& allowed = count == 0 ? g_unk_716e29c6d0.base.field_0x19 : g_unk_716e29c6d0.entries[count - 1].field_0x19;
    if (!allowed)
        return;
    g_unk_716e29c6d0.applied = 1;
    if (!desc->field_0x18) {
        g_unk_716e29c6d0.current.field_0x18 = 0;
        if (g_unk_716e29c691 != 0) {
            MarkDirty();
            g_unk_716e29c691 = 0;
            g_unk_716e29b6a8(g_unk_716e29c6a8, 0);
        }
        return;
    }
    g_unk_716e29c6d0.current = *desc;
    u8 enable = desc->field_0x18;
    if (g_unk_716e29c691 != enable) {
        MarkDirty();
        g_unk_716e29c691 = enable;
        g_unk_716e29b6a8(g_unk_716e29c6a8, enable);
    }
    s32 mode = desc->field_0xc;
    if (g_unk_716e29c694 != mode) {
        MarkDirty();
        g_unk_716e29c694 = mode;
        switch (mode) {
        case 0:
            g_unk_7101a1d7f0 = 8;
            break;
        case 1:
            g_unk_7101a1d7f0 = 1;
            break;
        case 2:
            g_unk_7101a1d7f0 = 3;
            break;
        case 3:
            g_unk_7101a1d7f0 = 6;
            break;
        case 4:
            g_unk_7101a1d7f0 = 2;
            break;
        case 5:
            g_unk_7101a1d7f0 = 4;
            break;
        case 6:
            g_unk_7101a1d7f0 = 5;
            break;
        case 7:
            g_unk_7101a1d7f0 = 7;
            break;
        }
        g_unk_716e29b6b0(g_unk_716e29c6a8, 2, g_unk_7101a1d7f0);
        s32 op = g_unk_7101a1d7f0;
        g_unk_716e29b6b0(g_unk_716e29c6a8, 1, op);
    }
    s32 value = desc->field_0x14;
    if (g_unk_716e29c240 != value) {
        MarkDirty();
        g_unk_716e29c240 = value;
        void* context = fn_7100153d50();
        value = g_unk_716e29c240;
        g_unk_716e29b9b8(context, 2, value);
        context = fn_7100153d50();
        value = g_unk_716e29c240;
        g_unk_716e29b9b8(context, 1, value);
    }
    value = desc->field_0x10;
    if (g_unk_716e29c244 != value) {
        MarkDirty();
        g_unk_716e29c244 = value;
        void* context = fn_7100153d50();
        value = g_unk_716e29c244;
        g_unk_716e29b9b0(context, 2, value);
        context = fn_7100153d50();
        value = g_unk_716e29c244;
        g_unk_716e29b9b0(context, 1, value);
    }
    fn_710015afd0(desc->field_0x0, desc->field_0x4, desc->field_0x8);
}
