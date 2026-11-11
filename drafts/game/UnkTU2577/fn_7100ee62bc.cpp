// From unit 2577 (CSubAreaManager): finishes a change of sub-area. While the change waits (state 3) it
// shows the new sub-area, hands it to the camera and the environment, then hides the previous
// sub-area, restores the faded render items and tells the listeners. Names and layouts are placeholders.
#include "UnkLeftoverReg.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::global::CStrId;

// A bit mask passed by value (copied into a temporary).
struct UnkMask {
    u32 bits;
    UnkMask() : bits(0) {}
    UnkMask(u32 b) : bits(b) {}
    UnkMask(const UnkMask& o) : bits(o.bits) {}
};

inline const UnkMask& UnkSubAreaNoneMask() {
    static UnkMask s_value asm("g_unk_7172719980");
    return s_value;
}

inline const UnkMask& UnkSubAreaAllMask() {
    static UnkMask s_mask asm("g_unk_7172719970")(~UnkSubAreaNoneMask().bits & 0x1ff);
    return s_mask;
}

struct UnkSubArea {
    CStrId field_0x0;
    u8 pad_0x8[0x48];
    CStrId field_0x50;
    u8 pad_0x58[0x120];
    u32 field_0x178;  // the layers shown (nine bits)
    u8 pad_0x17c[0x5];
    bool field_0x181;
};

// A listener list (count at +0x8) inside the manager.
struct UnkListeners {
    void* data;
    u32 count;
    u8 pad_0xc[0x74];
};

struct UnkItems {
    void** data;
    u32 count;
};

struct CSubAreaManager {
    u8 pad_0x0[0x8];
    UnkListeners field_0x8;
    UnkListeners field_0x88;
    u32 field_0x108;
    u32 field_0x10c;
    u32 field_0x110;
    u8 pad_0x114[0x2dc];
    UnkSubArea* field_0x3f0;  // the current sub-area
    UnkSubArea* field_0x3f8;  // the previous one
    u8 pad_0x400[0x538];
    UnkItems field_0x938;  // render items fading in
    u8 pad_0x948[0x18];
    UnkItems field_0x960;  // render items fading out
    u8 pad_0x970[0x18];
    u32 field_0x988;
    u8 pad_0x98c[0x154];
    bool field_0xae0;
};
static_assert(__builtin_offsetof(CSubAreaManager, field_0x3f0) == 0x3f0, "CSubAreaManager placeholder layout");
static_assert(__builtin_offsetof(CSubAreaManager, field_0x938) == 0x938, "CSubAreaManager placeholder layout");
static_assert(__builtin_offsetof(CSubAreaManager, field_0xae0) == 0xae0, "CSubAreaManager placeholder layout");

// The camera holder fn_7100defcec gives.
struct UnkCameraHolder {
    u8 pad_0x0[0x258];
    u32 field_0x258;
    u8 pad_0x25c[0xc];
    u32 field_0x268;
    f32 field_0x26c;
};

struct UnkPlayerSub {
    u8 pad_0x0[0x70];
    u32 field_0x70;
};
struct UnkPlayer {
    u8 pad_0x0[0x1b8];
    UnkPlayerSub* field_0x1b8;
    u8 pad_0x1c0[0xa0];
    void* field_0x260;
};

struct UnkScenarioSub {
    u8 pad_0x0[0x58];
    bool field_0x58;
};
struct UnkScenario {
    u8 pad_0x0[0x820];
    UnkScenarioSub* field_0x820;
};

struct UnkCutsceneState {
    u8 pad_0x0[0xb14];
    bool field_0xb14;
};
extern void* g_unk_7101d47330;

struct UnkStruct_710024fa0c;
struct UnkBitMaskHolder_b47a3;

// Helpers that take an empty value first (their callers leave x0 as it is).
struct UnkSubAreaTag : UnkLeftoverReg {};
void* fn_7100eea4c8(UnkSubAreaTag, UnkSubArea* area, bool a, bool b, UnkMask mask);
void* fn_7100eea618(UnkSubAreaTag, UnkSubArea* area, bool flag);

void fn_7100fac120(UnkSubArea* area, UnkMask mask, bool a, bool b, void* c, bool d);
void fn_7100d59dd8(UnkScenarioSub* self);
void fn_7100eeaa48(CSubAreaManager* self, const CStrId& prev_a, const CStrId& prev_b, const CStrId& cur_a,
                   const CStrId& cur_b, bool flag);
UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);
bool fn_7101196fd8(void*);
bool fn_710113d83c(void* p);
void fn_7100450950(UnkCameraHolder* holder);
void fn_7101053c14(void* fx, const base::math::CVector3D* color, f32 a, f32 b, f32 time);
void fn_7100eea25c(CSubAreaManager* self);
void fn_7100faced0(UnkBitMaskHolder_b47a3* self, u32 bit, bool set);
void fn_7100219560(UnkRenderManager* self, void* item, f32 value, bool flag);
bool fn_7100eea7b0(void* manager);
void fn_71000303c0(UnkDeferredSet<UnkDeferredCallback<bool>>* s, const bool* arg);

// The sub-area's ids, the global empty id without one.
static inline const CStrId& FirstId(const UnkSubArea* area) { return area ? area->field_0x0 : g_unk_7101d470e0; }
static inline const CStrId& SecondId(const UnkSubArea* area) { return area ? area->field_0x50 : g_unk_7101d470e0; }

// The camera controller's slot 9.
static inline void CameraSnap(UnkCameraCtrl* camera, f32 time, bool flag) {
    reinterpret_cast<void (*)(UnkCameraCtrl*, f32, bool)>(camera->vtable[9])(camera, time, flag);
}

static inline void Notify(UnkListeners* listeners, const bool* value) {
    fn_71000303c0(reinterpret_cast<UnkDeferredSet<UnkDeferredCallback<bool>>*>(listeners), value);
}

void fn_7100ee62bc(CSubAreaManager* self, UnkSubArea* area) {
    if (self->field_0x108 == 3) {
        fn_7100fac120(self->field_0x3f0, UnkSubAreaAllMask(), true, false, self->field_0x3f8, false);
        UnkGameManager* manager = g_unk_7101d47310;
        fn_7100eea618(UnkSubAreaTag(), self->field_0x3f0, true);
        UnkCutsceneState* cutscene;
        if (UnkCameraHolder* holder = static_cast<UnkCameraHolder*>(fn_7100defcec(g_unk_7101d47310))) {
            if (self->field_0x3f0->field_0x181) {
                holder->field_0x268 = holder->field_0x258;
                holder->field_0x26c = -1.0f;
            }
            fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(holder), true);
            if (UnkCameraCtrl* camera = fn_71010e2080(holder)) {
                bool free = !fn_7101196fd8(g_unk_7101d47330) &&
                            !static_cast<UnkCutsceneState*>(g_unk_7101d47330)->field_0xb14;
                UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
                if (player && free && player->field_0x260)
                    free &= !fn_710113d83c(player->field_0x260);
                if (free && fn_710009d50c(camera->vf_0x0(), fn_71010e7f90())) {
                    fn_7100450950(holder);
                    camera = fn_71010e2080(holder);
                    if (!camera)
                        goto environment;
                }
                CameraSnap(camera, 0.0f, true);
            }
        }
    environment:
        fn_7100eea4c8(UnkSubAreaTag(), self->field_0x3f0, false, true, UnkMask(0x1c0));
        {
            UnkSubArea* prev = self->field_0x3f8;
            UnkSubArea* cur = self->field_0x3f0;
            fn_7100eeaa48(self, FirstId(prev), SecondId(prev), FirstId(cur), SecondId(cur), true);
        }
        bool flash = true;
        if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
            if (player->field_0x1b8)
                flash = player->field_0x1b8->field_0x70 != 2;
            else
                flash = true;
        }
        cutscene = static_cast<UnkCutsceneState*>(g_unk_7101d47330);
        if (flash && !cutscene->field_0xb14 && self->field_0xae0) {
            base::math::CVector3D color;
            color.x = 0.0f;
            color.y = 0.0f;
            color.z = 0.0f;
            fn_7101053c14(manager->field_0x1040, &color, 0.033f, 0.2f, 1.0f);
        }
        self->field_0x108 = 1;
        fn_7100eea25c(self);
    }
    UnkSubArea* prev = self->field_0x3f8;
    if (prev && prev != area) {
        if (self->field_0x3f0->field_0x181) {
            for (u32 bit = 0; bit < 9; bit++) {
                if (self->field_0x3f8->field_0x178 & (1u << bit))
                    fn_7100faced0(reinterpret_cast<UnkBitMaskHolder_b47a3*>(self->field_0x3f8), bit, false);
            }
        }
        fn_7100fac120(self->field_0x3f8, UnkSubAreaAllMask(), false, false, self->field_0x3f0, false);
    }
    UnkScenarioSub* sub = reinterpret_cast<UnkScenario*>(g_unk_7101d47310->field_0x608)->field_0x820;
    sub->field_0x58 = true;
    fn_7100d59dd8(sub);
    self->field_0x988 = 0;
    for (void** it = self->field_0x960.data; it != self->field_0x960.data + self->field_0x960.count; ++it)
        fn_7100219560(g_unk_7101d47300, *it, 0.0f, false);
    for (void** it = self->field_0x938.data; it != self->field_0x938.data + self->field_0x938.count; ++it)
        fn_7100219560(g_unk_7101d47300, *it, 1.0f, false);
    if (self->field_0x8.count) {
        bool value = fn_7100eea7b0(self);
        Notify(&self->field_0x8, &value);
    }
    if (self->field_0x88.count) {
        bool value = true;
        Notify(&self->field_0x88, &value);
    }
    self->field_0x960.count = 0;
    self->field_0x938.count = 0;
    self->field_0x110 = 0;
}
