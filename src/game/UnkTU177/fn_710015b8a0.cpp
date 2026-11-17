// Unit 177 (the graphics state of game/UnkGraphicsState.h): pops the blend stack and, when asked, applies the
// blend state now on top (the base state once the stack is empty), pushing each changed value to NVN.
// Names and layouts are placeholders.
#include "game/UnkBlendStack.h"
#include "game/UnkGraphicsState.h"
#include "game/UnkTU172.h"
#include "game/UnkTU176.h"
#include "game/UnkTU177.h"
#include "types.h"

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

// Returns whether an entry is left on the stack.
bool fn_710015b8a0(bool apply) {
    u32 count = g_unk_716e29c6d0.count;
    g_unk_716e29c6d0.applied = 0;
    if (count >= 2) {
        g_unk_716e29c6d0.count = count - 1;
        if (apply) {
            UnkBlendState top = g_unk_716e29c6d0.entries[count - 2];
            SetBlendEnable(top.enable);
            SetBlendMode(top.mode);
            SetBlendValue14(top.field_0x14);
            SetBlendValue10(top.field_0x10);
            fn_710015afd0(top.field_0x0, top.field_0x4, top.field_0x8);
        }
        return true;
    }
    if (count == 1)
        g_unk_716e29c6d0.count = 0;
    if (apply) {
        const UnkBlendState& base = g_unk_716e29c6d0.base;
        SetBlendEnable(base.enable);
        SetBlendMode(base.mode);
        SetBlendValue14(base.field_0x14);
        SetBlendValue10(base.field_0x10);
        fn_710015afd0(base.field_0x0, base.field_0x4, base.field_0x8);
    }
    return false;
}
