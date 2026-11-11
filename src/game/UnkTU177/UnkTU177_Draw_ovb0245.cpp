// Unit 177 (tools/tu_map.py; the graphics state of game/UnkGraphicsState.h): draw and state setters that go through the
// NVN entry points: the draw calls that flush the pending state first (fn_710015ac00, fn_7100159f90), the
// target clear (fn_7100159660), the mode setter (fn_710015a8c0) and the blend state push
// (fn_710015b650). Names are placeholders.
#include "game/UnkGraphicsState.h"
#include "game/UnkTU172.h"
#include "game/UnkTU174.h"
#include "game/UnkTU177.h"
#include "types.h"

struct UnkDrawState_ovb0245 {
    u8 pad_0x0[0x14];
    s32 field_0x14;
    s32 type;  // 0x18
    u8 pad_0x1c[0x8];
    u32 value_0x24;  // 0x24
    u8* data;  // 0x28
};

struct UnkUnit174Singleton;
void fn_710015e700();

extern bool g_unk_7101a1d7c4;
extern u8 g_unk_716e29c668;
extern f32 g_unk_716e29c660;
extern f32 g_unk_716e29c664;
extern void (*g_unk_716e29b968)(void* context, u8 flag, f32 a, f32 b);
extern const u32 g_unk_7101647eb0[3] __attribute__((visibility("hidden")));
extern void (*g_unk_716e29b8e0)(void* context, s32 a, s64 type, u32 b, void* data, u32 c, u32 d, u32 e);

// Through a wrapper: the arguments are evaluated before the entry point is loaded.
template <class R, class... A, class... B>
__attribute__((always_inline)) static inline R CallNvn(R (*&fn)(A...), B... args) {
    return fn(args...);
}

static inline void FlushState_ovb0245() {
    if (g_unk_7101a1d7c4) {
        g_unk_7101a1d7c4 = false;
        void* context = fn_7100153d50();
        CallNvn(g_unk_716e29b968, context, g_unk_716e29c668, g_unk_716e29c660, g_unk_716e29c664);
    }
}

static inline u32 Stride_ovb0245(s64 type) { return (u64)type <= 2 ? g_unk_7101647eb0[type] : 0; }

void fn_710015ac00(u32 a, u32 unused, u32 index, u32 b, u32 c, u32 d) {
    UnkDrawState_ovb0245* state = reinterpret_cast<UnkDrawState_ovb0245*>(fn_7100154470());
    fn_710015e700();
    FlushState_ovb0245();
    void* context = fn_7100153d50();
    s64 type = state->type;
    CallNvn(g_unk_716e29b8e0, context, state->field_0x14, type, b, state->data + Stride_ovb0245(type) * index, a, c, d);
}

extern void (*g_unk_716e29b8c8)(void* context, s32 a, s64 type, u32 value, void* data);

void fn_7100159f90(u32 count, u32 value) {
    if (value == 0)
        return;
    UnkDrawState_ovb0245* state = reinterpret_cast<UnkDrawState_ovb0245*>(fn_7100154470());
    u32* source;
    if (value == (u32)-1)
        source = &reinterpret_cast<UnkDrawState_ovb0245*>(fn_7100154470())->value_0x24;
    else
        source = &value;
    fn_710015e700();
    FlushState_ovb0245();
    void* context = fn_7100153d50();
    s64 type = state->type;
    CallNvn(g_unk_716e29b8c8, context, state->field_0x14, type, *source, state->data + Stride_ovb0245(type) * count);
}


extern bool g_unk_7101a1d7d8;

// ---- fn_7100159660 ----

struct __attribute__((packed)) UnkTargetList_ovb0245 {
    s32 count;
    u64 entries[1];  // 0x4
};
UnkTargetList_ovb0245* fn_710015c1d0();

extern bool g_unk_7101a1d7d8;
extern void (*g_unk_716e29b7d8)(void* context, void* target);
extern u8 g_unk_716e29c220[];
extern void (*g_unk_716e29b908)(void* context, s32 index, void* value, s32 flags);
extern f32 g_unk_716e29c230;
extern u32 g_unk_716e29c234;
extern void (*g_unk_716e29b920)(void* context, s32 a, f32 b, u32 c, s32 d);

void fn_7100159660(bool a, bool b, bool c) {
    if (a) {
        if (g_unk_7101a1d7d8) {
            void* context = fn_7100153d50();
            g_unk_716e29b7d8(context, g_unk_716e29c680);
            g_unk_7101a1d7d8 = false;
        }
        u64 zero[2] = {0, 0};
        UnkTargetList_ovb0245* list = fn_710015c1d0();
        s32 n = list->count;
        if (n >= 1) {
            for (u64 i = 0; i != (u64)(u32)n; ++i) {
                if (list->entries[i] == 0)
                    continue;
                void* context = fn_7100153d50();
                void* target = i == 0 ? (void*)g_unk_716e29c220 : (void*)zero;
                g_unk_716e29b908(context, i, target, 0xf);
            }
        }
    }
    if (b || c) {
        void* context = fn_7100153d50();
        CallNvn(g_unk_716e29b920, context, b, g_unk_716e29c230, g_unk_716e29c234, -(s32)c);
    }
}


extern s32 g_unk_7101a1d7e8;
extern void (*g_unk_716e29b6a0)(void* object, s32 value);

void fn_710015a8c0(u32 mode) {
    if (g_unk_7101a1d7e8 == (s32)mode)
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7e4 = true;
    g_unk_7101a1d7e8 = mode;
    switch (mode) {
    case 0:
        g_unk_7101a1d7ec = 8;
        break;
    case 1:
        g_unk_7101a1d7ec = 1;
        break;
    case 2:
        g_unk_7101a1d7ec = 3;
        break;
    case 3:
        g_unk_7101a1d7ec = 6;
        break;
    case 4:
        g_unk_7101a1d7ec = 2;
        break;
    case 5:
        g_unk_7101a1d7ec = 4;
        break;
    case 6:
        g_unk_7101a1d7ec = 5;
        break;
    case 7:
        g_unk_7101a1d7ec = 7;
        break;
    }
    g_unk_716e29b6a0(g_unk_716e29c6a8, g_unk_7101a1d7ec);
}

// ---- fn_710015b650 ----
struct UnkBlendRequest_ovb0245 {
    s32 field_0x0;
    s32 field_0x4;
    s32 field_0x8;
    s32 mode;      // 0xc
    s32 field_0x10;
    s32 field_0x14;
    u8 enable;  // 0x18
    u8 pad_0x19[3];
};

struct UnkBlendStack_ovb0245 {
    u8 pushed;  // 0x0
    u8 pad_0x1[7];
    u32 count;  // 0x8
    u8 pad_0xc[0x5c];
    UnkBlendRequest_ovb0245 entries[0x3f];  // 0x68
};

extern UnkBlendStack_ovb0245 g_unk_716e29c6d0 __attribute__((visibility("hidden")));
extern void (*g_unk_716e29b6a8)(void* object, u8 value);
extern s32 g_unk_716e29c694;
extern void (*g_unk_716e29b6b0)(void* object, s32 which, s32 value);
extern s32 g_unk_716e29c240;
extern void (*g_unk_716e29b9b8)(void* context, s32 which, s32 value);
extern s32 g_unk_716e29c244;
extern void (*g_unk_716e29b9b0)(void* context, s32 which, s32 value);
void fn_710015afd0(s32 a, s32 b, s32 c);

// As src/game/UnkTU260/UnkTU260_Render_ovb0237.cpp declares it (the request is its flags block; the original returns
// whether the request was taken).
struct UnkFlags_b0237;
bool fn_710015b650(UnkFlags_b0237* request_) {
    auto* request = reinterpret_cast<UnkBlendRequest_ovb0245*>(request_);
    g_unk_716e29c6d0.pushed = 0;
    u32 count = g_unk_716e29c6d0.count;
    if (count > 0x3e)
        return false;
    g_unk_716e29c6d0.count = count + 1;
    g_unk_716e29c6d0.entries[count] = *request;
    u8 enable = request->enable;
    if (g_unk_716e29c691 != enable) {
        g_unk_7101a1d7c5 = true;
        g_unk_7101a1d7e4 = true;
        g_unk_716e29c691 = enable;
        g_unk_716e29b6a8(g_unk_716e29c6a8, enable);
    }
    s32 mode = request->mode;
    if (g_unk_716e29c694 != mode) {
        g_unk_7101a1d7c5 = true;
        g_unk_7101a1d7e4 = true;
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
        CallNvn(g_unk_716e29b6b0, (void*)g_unk_716e29c6a8, 1, g_unk_7101a1d7f0);
    }
    s32 value = request->field_0x14;
    if (g_unk_716e29c240 != value) {
        g_unk_7101a1d7c5 = true;
        g_unk_7101a1d7e4 = true;
        g_unk_716e29c240 = value;
        CallNvn(g_unk_716e29b9b8, fn_7100153d50(), 2, g_unk_716e29c240);
        CallNvn(g_unk_716e29b9b8, fn_7100153d50(), 1, g_unk_716e29c240);
    }
    value = request->field_0x10;
    if (g_unk_716e29c244 != value) {
        g_unk_7101a1d7c5 = true;
        g_unk_7101a1d7e4 = true;
        g_unk_716e29c244 = value;
        CallNvn(g_unk_716e29b9b0, fn_7100153d50(), 2, g_unk_716e29c244);
        CallNvn(g_unk_716e29b9b0, fn_7100153d50(), 1, g_unk_716e29c244);
    }
    fn_710015afd0(request->field_0x0, request->field_0x4, request->field_0x8);
    return true;
}
