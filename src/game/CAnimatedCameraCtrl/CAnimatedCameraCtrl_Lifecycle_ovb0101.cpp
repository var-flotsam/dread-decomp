// CAnimatedCameraCtrl's unit (tools/tu_map.py): the interpolation kind a name gives, the end-point
// update that keeps the placement inside the boundaries unless the target's track ignores them
// (CCharClassIgnoreCameraUseBoundariesTrack), the start (from the argument's node and definition and
// the player's position), the end and reset steps (the camera manager's release of the id at +0x5a0,
// the script function at +0x3b8, the one-shot callback at +0x480) and the destructor. Names and
// layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCameraBoundaryCtrl.h"
#include "game/CCameraCtrl.h"
#include "game/CInputComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

// The interpolation kind a name gives.
s32 fn_71010ec868(const char* name) {
    if (strcmp(name, "Smooth") == 0)
        return 0;
    if (strcmp(name, "Linear") == 0)
        return 1;
    if (strcmp(name, "EaseInOutQuadratic") == 0)
        return 2;
    if (strcmp(name, "EaseOutCubic") == 0)
        return 3;
    if (strcmp(name, "EaseOutQuadratic") == 0)
        return 4;
    if (strcmp(name, "EaseInQuadratic") == 0)
        return 5;
    return 6;
}

// ----

struct UnkCamVec3_b0101 {
    f32 x, y, z;
    UnkCamVec3_b0101(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkCamVec3_b0101(const UnkCamVec3_b0101& o) : x(o.x), y(o.y), z(o.z) {}
    UnkCamVec3_b0101& operator=(const UnkCamVec3_b0101& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A camera placement: a position and a direction.
struct UnkCamPlacement_b0101 {
    UnkCamVec3_b0101 pos;
    UnkCamVec3_b0101 dir;
    UnkCamPlacement_b0101& operator=(const UnkCamPlacement_b0101& o) {
        pos = o.pos;
        dir = o.dir;
        return *this;
    }
};

struct UnkCamTargetActor_b0101 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkCamTargetHandle_b0101 {
    u8 pad_0x0[0x10];
    UnkCamTargetActor_b0101* actor;
};

struct UnkAnimatedCameraCtrl_b0101 {
    u8 pad_0x0[0x38];
    UnkCamPlacement_b0101 field_0x38;
    u8 pad_0x50[0x18];
    UnkCamPlacement_b0101 field_0x68;
    u8 pad_0x80[0x1b8];
    UnkCamVec3_b0101 field_0x238;
    u8 pad_0x244[0x20];
    bool field_0x264;
    bool field_0x265;
    u8 pad_0x266[0xa];
    UnkCamVec3_b0101 field_0x270;
    u8 pad_0x27c[0x23c];
    UnkCamTargetHandle_b0101* target;  // 0x4b8
};
static_assert(__builtin_offsetof(UnkAnimatedCameraCtrl_b0101, target) == 0x4b8, "UnkAnimatedCameraCtrl_b0101 layout");

static inline bool IgnoresBoundaries(UnkAnimatedCameraCtrl_b0101* self) {
    UnkCamTargetHandle_b0101* target = self->target;
    if (!target || !target->actor)
        return false;
    UnkAnimationComponent* animation = target->actor->animation;
    return animation && fn_710117b348(animation, fn_7100c282c8());
}

void fn_71010ea3e4(UnkAnimatedCameraCtrl_b0101* self, bool reset) {
    if (self->field_0x265)
        return;
    UnkCamVec3_b0101 from = self->field_0x238;
    f32 y = self->field_0x68.pos.y;
    f32 x = self->field_0x68.pos.x;
    f32 z = self->field_0x68.pos.z;
    if (reset)
        self->field_0x38 = self->field_0x68;
    if (self->field_0x264 && !IgnoresBoundaries(self)) {
        UnkCamVec3_b0101 to(x, y, z);
        fn_7100764a50(self, &from, &to, reset);
    }
    if (reset)
        self->field_0x38 = self->field_0x68;
    self->field_0x270 = self->field_0x68.pos;
}

// ----

struct UnkManager;
extern UnkManager* g_unk_7101d47360;
void fn_7100ee1f34(void*);

// A bit mask passed by value (copied into a temporary), and its two guarded constants (as in
// src/game/UnkLoadingScreen.cpp).
struct UnkCamMask_b0101 {
    u32 bits;
    UnkCamMask_b0101() : bits(0) {}
    UnkCamMask_b0101(u32 b) : bits(b) {}
    UnkCamMask_b0101(const UnkCamMask_b0101& o) : bits(o.bits) {}
};
template <class T>
struct UnkCamMaskOf_b0101 {
    static const UnkCamMask_b0101& None() {
        static UnkCamMask_b0101 s;
        return s;
    }
    static const UnkCamMask_b0101& All() {
        static UnkCamMask_b0101 s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkCamMaskTag_b0101;

void fn_7100eed3a4(void* manager, const CStrId* id, UnkCamMask_b0101 mask, bool flag);

static inline void* CameraManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return manager;
}

struct UnkPlayer;
struct UnkInputComponent;
extern __attribute__((visibility("hidden"))) char g_unk_7172a04768[];
struct UnkGrabCameraCtrl;

struct UnkBossCameraCtrl_b49a3;
extern void* g_unk_7101cb1550[];

// A reference-counted object released through its vtable (slot 2).
struct UnkCamRef_b0101 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};

// A file-path id released in two steps (the count reloaded after the decrement, as CStrId::Reset).
static inline void ResetFilePath(base::global::UnkFilePathStrIdBase* id) {
    if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
        if (id->data && id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0) {
                fn_71000803a4(pool, id->data);
                pool = g_unk_7101d47200;
            }
        }
        id->data = &pool->empty;
    }
}

// A callback run once: fn(arg), then cleared.
struct UnkCamCallback_b0101 {
    void* arg;
    void (*fn)(void* arg);
};

struct UnkAnimatedCameraEnd_b0101 {
    void** vtable;
    u8 pad_0x8[0x298];
    bool field_0x2a0;
    u8 pad_0x2a1[0x7];
    UnkItemBase_71011a91c4* field_0x2a8;
    base::global::UnkFilePathStrIdBase field_0x2b0;
    CStrId field_0x2b8;
    u8 pad_0x2c0[0x20];
    base::global::CRntString field_0x2e0;
    u8 pad_0x310[0xa8];
    base::global::CRntString script;  // 0x3b8: the script function run at the end
    u8 pad_0x3e8[0x98];
    UnkCamCallback_b0101 on_end;  // 0x480
    u8 pad_0x490[0x28];
    UnkCamRef_b0101* field_0x4b8;
    u8 pad_0x4c0[0x1c];
    s32 state;  // 0x4dc
    u8 pad_0x4e0[0x30];
    CStrId field_0x510;
    u8 pad_0x518[0x80];
    bool field_0x598;
    bool field_0x599;
    bool field_0x59a;
    u8 pad_0x59b[0x5];
    CStrId field_0x5a0;
    u8 field_0x5a8[0x8];
};
static_assert(__builtin_offsetof(UnkAnimatedCameraEnd_b0101, field_0x5a0) == 0x5a0, "UnkAnimatedCameraEnd_b0101 layout");

struct UnkPlayer_b0101 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
};

void fn_71010e917c(UnkAnimatedCameraEnd_b0101* self) {
    if (self->state == 2 && self->field_0x2a8) {
        if (self->field_0x2a0)
            fn_71002199d8(g_unk_7101d47300, self->field_0x2a8, 0);
        self->field_0x2a8 = nullptr;
    }
    if (self->field_0x5a0.data != &g_unk_7101d47108->empty)
        fn_7100eed3a4(CameraManager(), &self->field_0x5a0, UnkCamMaskOf_b0101<UnkCamMaskTag_b0101>::All(), false);
    self->field_0x598 = false;
    self->field_0x599 = false;
    self->field_0x59a = false;
    fn_710002fa90(self->field_0x5a8);
    if (auto* player = reinterpret_cast<UnkPlayer_b0101*>(fn_7100dec788(g_unk_7101d47310))) {
        if (UnkInputComponent* input = player->input)
            fn_7100872460(input, false, false, g_unk_7172a04768);
    }
    fn_71010edd64(reinterpret_cast<UnkGrabCameraCtrl*>(self));
}

void fn_71010e8ff8(UnkAnimatedCameraEnd_b0101* self) {
    if (self->field_0x599)
        return;
    self->state = 2;
    if (self->field_0x5a0.data != &g_unk_7101d47108->empty)
        fn_7100eed3a4(CameraManager(), &self->field_0x5a0, UnkCamMaskOf_b0101<UnkCamMaskTag_b0101>::All(), false);
    if (self->script.data && *self->script.data) {
        const char* args = g_unk_7101d58d10.data;
        fn_7100000790(self->script.data, args ? args : g_unk_710162e2ed);
    }
    if (self->on_end.fn) {
        self->on_end.fn(self->on_end.arg);
        __builtin_memset(&self->on_end, 0, sizeof(self->on_end));
    }
}

void fn_71010e85bc(UnkAnimatedCameraEnd_b0101* self) {
    self->vtable = g_unk_7101cb1550 + 2;
    if (self->field_0x2a8) {
        if (self->field_0x2a0)
            fn_71002199d8(g_unk_7101d47300, self->field_0x2a8, 0);
        self->field_0x2a8 = nullptr;
    }
    fn_710002f67c(self->field_0x5a8);
    self->field_0x5a0.Reset();
    self->field_0x510.Reset();
    if (UnkCamRef_b0101* ref = self->field_0x4b8) {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
    }
    fn_710007ed40(&self->script);
    fn_710007ecd4(&self->script);
    fn_710007ed40(&self->field_0x2e0);
    fn_710007ecd4(&self->field_0x2e0);
    self->field_0x2b8.Reset();
    ResetFilePath(&self->field_0x2b0);
    fn_7100763f74(reinterpret_cast<UnkBossCameraCtrl_b49a3*>(self));
}

// ----

struct UnkDropCameraVec3;
struct UnkFixedCamera_fc;
struct UnkArg_fc;

// The camera definition the argument points at (+0x130).
struct UnkCamDef_b0101 {
    u8 pad_0x0[0x74];
    f32 field_0x74;
    u8 pad_0x78[0x8];
    f32 field_0x80;
    u8 pad_0x84[0xc];
    f32 field_0x90;
    u8 pad_0x94[0x20];
    UnkVec3f field_0xb4;
    u8 pad_0xc0[0x5c];
    UnkVec3f field_0x11c;
};

struct UnkCamArg_b0101 {
    u8 pad_0x0[0x20];
    UnkXformNode node;      // 0x20
    UnkCamDef_b0101* def;   // 0x130
};

struct UnkCamStart_b0101 {
    UnkVec3f pos;
    UnkVec3f dir;
    UnkVec3f field_0x18;
    UnkVec3f field_0x24;
};

struct UnkAnimatedCameraStart_b0101 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkCamStart_b0101 field_0x38;
    u8 pad_0x68[0x1d0];
    UnkVec3f field_0x238;
    UnkVec3f field_0x244;
    u8 pad_0x250[0x10];
    f32 field_0x260;
    u8 pad_0x264[0x27c];
    UnkVec3f field_0x4e0;
    UnkVec3f field_0x4ec;
    UnkVec3f field_0x4f8;
    f32 field_0x504;
    f32 field_0x508;
    f32 field_0x50c;
    CStrId field_0x510;
    bool field_0x518;
    u8 pad_0x519[0x33];
    bool field_0x54c;
    u8 pad_0x54d[0x33];
    bool field_0x580;
    u8 pad_0x581[0x3];
    u32 field_0x584[4];
    u8 pad_0x594[0xc4];
    bool field_0x658;

    bool vf_0x60() { return reinterpret_cast<bool (*)(UnkAnimatedCameraStart_b0101*)>(vtable[0x60 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkAnimatedCameraStart_b0101, field_0x658) == 0x658, "UnkAnimatedCameraStart_b0101 layout");

void fn_71010e8998(UnkAnimatedCameraStart_b0101* self);

void fn_71010e87b8(UnkAnimatedCameraStart_b0101* self, UnkCamArg_b0101* arg) {
    if (self->vf_0x60()) {
        self->field_0x518 = false;
        self->field_0x54c = false;
        self->field_0x580 = false;
        __builtin_memset(reinterpret_cast<u8*>(self) + 0x584, 0, 0x10);
        self->field_0x260 = 0.01f;
        self->field_0x658 = false;
        if (self->field_0x510.data != &g_unk_7101d47108->empty)
            self->field_0x510 = g_unk_7101d470e0;
    }
    self->field_0x4e0 = fn_7100206154(&arg->node);
    self->field_0x4ec = arg->def->field_0xb4;
    self->field_0x4f8 = arg->def->field_0x11c;
    *reinterpret_cast<a_u32*>(&self->field_0x504) = *reinterpret_cast<const a_u32*>(&arg->def->field_0x80);
    *reinterpret_cast<a_u32*>(&self->field_0x508) = *reinterpret_cast<const a_u32*>(&arg->def->field_0x90);
    *reinterpret_cast<a_u32*>(&self->field_0x50c) = *reinterpret_cast<const a_u32*>(&arg->def->field_0x74);
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
        self->field_0x238 = *reinterpret_cast<const UnkVec3f*>(fn_7100d7c9f4(player));
        self->field_0x244 = self->field_0x238;
    }
    fn_71010ed98c(reinterpret_cast<UnkFixedCamera_fc*>(self), reinterpret_cast<UnkArg_fc*>(arg), 1);
    fn_71010e8998(self);
    self->field_0x4e0 = self->field_0x38.pos;
    self->field_0x4ec = self->field_0x38.dir;
    UnkVec3f* d = reinterpret_cast<UnkVec3f*>(&self->field_0x504);
    *d = self->field_0x38.field_0x24;
}
