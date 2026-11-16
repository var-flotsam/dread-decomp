// Unit 177 (the graphics state of game/UnkGraphicsState.h): applies the blend state on top of the blend stack
// again (the base state when the stack is empty), unless it is locked, pushing each changed value to NVN.
// Names and layouts are placeholders.
#include "game/UnkGraphicsState.h"
#include "game/UnkTU172.h"
#include "game/UnkTU176.h"
#include "game/UnkTU177.h"
#include "types.h"

struct UnkBlendState_l15b8a0 {
    s32 field_0x0;
    s32 field_0x4;
    s32 field_0x8;
    s32 mode;  // 0xc
    s32 field_0x10;
    s32 field_0x14;
    bool enable;  // 0x18
    u8 field_0x19;
};

struct UnkBlendStack_l15b8a0 {
    u8 applied;  // 0x0
    u8 pad_0x1[7];
    u32 count;  // 0x8
    u8 pad_0xc[0x24];
    UnkBlendState_l15b8a0 base;  // 0x30
    UnkBlendState_l15b8a0 current;  // 0x4c
    UnkBlendState_l15b8a0 entries[0x3f];  // 0x68
};
static_assert(__builtin_offsetof(UnkBlendStack_l15b8a0, entries) == 0x68, "");

extern UnkBlendStack_l15b8a0 g_unk_716e29c6d0 __attribute__((visibility("hidden")));
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

inline void SetBlendEnable(u8 enable) {
    if (g_unk_716e29c691 != enable) {
        MarkDirty();
        g_unk_716e29c691 = enable;
        g_unk_716e29b6a8(g_unk_716e29c6a8, enable);
    }
}

inline void SetBlendMode(s32 mode) {
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
}

inline void SetBlendValue14(s32 value) {
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
}

inline void SetBlendValue10(s32 value) {
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
}

void fn_710015b160() {
    u32 count = g_unk_716e29c6d0.count;
    const u8& allowed = count == 0 ? g_unk_716e29c6d0.base.field_0x19 : g_unk_716e29c6d0.entries[count - 1].field_0x19;
    if (!allowed)
        return;
    g_unk_716e29c6d0.applied = 0;
    if (count != 0) {
        UnkBlendState_l15b8a0 top = g_unk_716e29c6d0.entries[count - 1];
        SetBlendEnable(top.enable);
        SetBlendMode(top.mode);
        SetBlendValue14(top.field_0x14);
        SetBlendValue10(top.field_0x10);
        fn_710015afd0(top.field_0x0, top.field_0x4, top.field_0x8);
    } else {
        const UnkBlendState_l15b8a0& base = g_unk_716e29c6d0.base;
        SetBlendEnable(base.enable);
        SetBlendMode(base.mode);
        SetBlendValue14(base.field_0x14);
        SetBlendValue10(base.field_0x10);
        fn_710015afd0(base.field_0x0, base.field_0x4, base.field_0x8);
    }
}
