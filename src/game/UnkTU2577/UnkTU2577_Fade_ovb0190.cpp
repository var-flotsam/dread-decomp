// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee4d34: steps the sub-area change by its state: state 1 fades the two render item lists by the elapsed time, state 2 waits for the effect player. Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2656.h"
#include "types.h"

struct CSubAreaManager;
struct UnkSubArea;

struct UnkPlayerSub {
    u8 pad_0x0[0x6c];
    bool field_0x6c;
};
struct UnkPlayer {
    u8 pad_0x0[0x1c0];
    UnkPlayerSub* field_0x1c0;
};

// The fade durations of the sub-area change (g_unk_71729da878's tunable, as far as read here).
struct UnkTunable {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
};

void fn_7100ee5bc4(CSubAreaManager* self);
void fn_7100ee5dcc(CSubAreaManager* self);
void fn_7100ee62bc(CSubAreaManager* self, UnkSubArea* area);
void fn_7100219560(UnkRenderManager* self, void* item, f32 value, bool flag);

struct UnkRenderItems_b190 {
    void** data;
    u32 count;
};

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x108];
    u32 field_0x108;
    u8 pad_0x10c[4];
    u32 field_0x110;
    u8 pad_0x114[0x2c];
    u32 field_0x140;
    u8 pad_0x144[0x2ac];
    UnkSubArea* field_0x3f0;
    u8 pad_0x3f8[0x540];
    UnkRenderItems_b190 field_0x938;
    u8 pad_0x948[0x18];
    UnkRenderItems_b190 field_0x960;
    u8 pad_0x970[0x18];
    f32 field_0x988;
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x140) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x938) == 0x938, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x960) == 0x960, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x988) == 0x988, "layout");

static inline f32 Lerp(f32 a, f32 b, f32 t) {
    f32 rest = 1.0f - t;
    return t * b + rest * a;
}
static inline f32 LerpClamped(f32 a, f32 b, f32 t) {
    if (t >= 1.0f)
        return b;
    if (t < 0.0f)
        return a;
    return Lerp(a, b, t);
}

static inline void ApplyAll(UnkRenderItems_b190* items, f32 value) {
    for (void** it = items->data; it != items->data + items->count; ++it)
        fn_7100219560(g_unk_7101d47300, *it, value, false);
}

void fn_7100ee4d34(UnkSubAreaMgr_b190* self, f32 dt) {
    CSubAreaManager* manager = reinterpret_cast<CSubAreaManager*>(self);
    switch (self->field_0x110) {
    case 2: {
        void* effects = g_unk_7101d47310->field_0x1040;
        fn_7100ee5bc4(manager);
        if (fn_7101053a7c(effects))
            return;
        if (fn_7101053a0c(effects)) {
            if (self->field_0x140 == 0)
                fn_7100ee5dcc(manager);
            else
                --self->field_0x140;
        } else if (!fn_7101053a0c(effects)) {
            if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
                if (player->field_0x1c0)
                    player->field_0x1c0->field_0x6c = false;
            }
            self->field_0x110 = 0;
        }
        break;
    }
    case 1: {
        if (self->field_0x108 == 3) {
            fn_7100ee5bc4(manager);
            void* effects = g_unk_7101d47310->field_0x1040;
            if (fn_7101053a0c(effects) && fn_7101053a98(effects))
                return;
            fn_7100ee62bc(manager, self->field_0x3f0);
            return;
        }
        self->field_0x988 = self->field_0x988 + dt;
        f32 elapsed = self->field_0x988;
        f32 out_alpha = LerpClamped(1.0f, 0.0f, elapsed / g_unk_71729da878.field_0x28);
        f32 in_alpha = LerpClamped(0.0f, 1.0f, elapsed / g_unk_71729da878.field_0x24);
        ApplyAll(&self->field_0x960, out_alpha);
        ApplyAll(&self->field_0x938, in_alpha);
        break;
    }
    }
}
