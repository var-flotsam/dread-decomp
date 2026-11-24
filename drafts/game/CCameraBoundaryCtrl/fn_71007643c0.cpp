// CCameraBoundaryCtrl unit: the camera boundary's update. While it is holding (+0x265) it follows the sub-area
// manager's state and tells the camera component the new stage's blend time; otherwise it measures the segment its two
// end points span, stores the origin and direction and keeps the cached extent where it moved less than a threshold.
// Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2656.h"
#include "game/CCameraBoundaryCtrl.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkCamComp8_ovb5;
struct UnkCamTarget2_ovb5;
void fn_71010e6278(UnkCamComp8_ovb5* self, UnkCamTarget2_ovb5* target, f32 a, f32 b, f32 time);

struct UnkSubAreaManagerView_l7643c0;
bool fn_7100eeb244(UnkSubAreaManagerView_l7643c0* self);
struct UnkPairTable_A6_b72;
bool CSubAreaManager_GetPositiveValuesByKeyPair(UnkPairTable_A6_b72* self, const u64* key0, const u64* key1, f32* second, f32* first);

struct UnkNamed_l7643c0 {
    u8 pad_0x0[0x20];
    CStrId name;  // 0x20
};

struct UnkObject_l7643c0 {
    u8 pad_0x0[0x20];
    bool field_0x20;
    u8 pad_0x21[0xc38 - 0x21];
    UnkWeakHandle* named;  // 0xc38
};

struct UnkOwner_l7643c0 {
    u8 pad_0x0[0x10];
    UnkObject_l7643c0* object;  // 0x10
};

struct UnkVec_l7643c0 {
    f32 x;
    f32 y;
    f32 z;
};

// An origin and a direction.
struct UnkRay_l7643c0 {
    UnkVec_l7643c0 origin;
    UnkVec_l7643c0 direction;
};

struct UnkCtrl_l7643c0 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkRay_l7643c0 saved;  // 0x38
    u8 pad_0x50[0x18];
    UnkRay_l7643c0 current;  // 0x68
    u8 pad_0x80[0x30];
    UnkOwner_l7643c0* owner;  // 0xb0
    u8 pad_0xb8[0x180];
    UnkVec_l7643c0 offset;  // 0x238
    u8 pad_0x244[0x21];
    bool holding;  // 0x265
    u8 pad_0x266[2];
    f32 field_0x268;
    f32 field_0x26c;
    f32 field_0x270;
    f32 field_0x274;
    f32 field_0x278;
    f32 field_0x27c;
    f32 field_0x280;
    f32 field_0x284;
    f32 field_0x288;
    bool field_0x28c;
    bool field_0x28d;
    bool field_0x28e;
    bool field_0x28f;
    bool field_0x290;
    bool field_0x291;
    u8 pad_0x292[6];
    void* field_0x298;  // 0x298

    template <typename R, typename... Args>
    R Call(u32 offset, Args... args) {
        return reinterpret_cast<R (*)(UnkCtrl_l7643c0*, Args...)>(vtable[offset / 8])(this, args...);
    }
};
static_assert(__builtin_offsetof(UnkCtrl_l7643c0, current) == 0x68, "layout");
static_assert(__builtin_offsetof(UnkCtrl_l7643c0, owner) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkCtrl_l7643c0, offset) == 0x238, "layout");
static_assert(__builtin_offsetof(UnkCtrl_l7643c0, holding) == 0x265, "layout");
static_assert(__builtin_offsetof(UnkCtrl_l7643c0, field_0x298) == 0x298, "layout");

static inline UnkSubAreaManagerView_l7643c0* SubAreaManager() {
    if (!g_unk_7101d47360) {
        void* m = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(m);
        g_unk_7101d47360 = static_cast<UnkManager*>(m);
    }
    return reinterpret_cast<UnkSubAreaManagerView_l7643c0*>(g_unk_7101d47360);
}

// What the object's second reference refers to, when there is one.
static inline UnkNamed_l7643c0* NamedOf(UnkCtrl_l7643c0* self) {
    if (UnkOwner_l7643c0* owner = self->owner) {
        if (UnkObject_l7643c0* object = owner->object) {
            if (UnkWeakHandle* named = object->named)
                return reinterpret_cast<UnkNamed_l7643c0*>(named->target);
        }
    }
    return nullptr;
}

static inline UnkCamComp8_ovb5* CameraOf(UnkCtrl_l7643c0* self) {
    return self->owner ? reinterpret_cast<UnkCamComp8_ovb5*>(self->owner->object) : nullptr;
}

void fn_71007643c0(UnkCtrl_l7643c0* self, bool reset) {
    if (self->holding) {
        UnkNamed_l7643c0* named = NamedOf(self);
        CStrId stage(named ? named->name : base::global::CStrId::Empty());
        f32 unused;
        self->field_0x28d = self->Call<bool>(0x1b0, &self->offset, &unused);
        bool inside = fn_7100eeb244(SubAreaManager());
        if (self->owner && self->owner->object) {
            UnkNamed_l7643c0* current = NamedOf(self);
            if (self->field_0x298 != current) {
                UnkSubAreaManagerView_l7643c0* manager = SubAreaManager();
                if (*reinterpret_cast<u32*>(reinterpret_cast<char*>(manager) + 0x110) != 2) {
                    if (self->field_0x298 && self->field_0x28c == inside)
                        self->field_0x28d = true;
                    self->field_0x298 = NamedOf(self);
                }
            }
        }
        self->field_0x28c = inside;
        if (self->field_0x28f) {
            self->field_0x28e = true;
            self->field_0x28f = false;
        }
        bool moved = self->field_0x28d;
        bool changing = self->field_0x28e;
        bool trigger = changing;
        if (moved)
            trigger = changing || fn_7101053a98(&g_unk_7101d47310->field_0x1040) ||
                      fn_7101053a7c(&g_unk_7101d47310->field_0x1040);
        if (!trigger) {
            if (moved) {
                self->field_0x28e = true;
                self->current.origin.x = self->saved.origin.x;
                self->current.origin.y = self->saved.origin.y;
            }
            return;
        }
        self->field_0x28e = false;
        if (self->Call<bool>(0x198)) {
            fn_71010e6278(CameraOf(self), reinterpret_cast<UnkCamTarget2_ovb5*>(self), 10.0f, -1.0f, 0.01f);
        } else {
            UnkNamed_l7643c0* now = NamedOf(self);
            CStrId next(now ? now->name : base::global::CStrId::Empty());
            f32 time = 8.0f;
            f32 blend = 0.3f;
            CSubAreaManager_GetPositiveValuesByKeyPair(reinterpret_cast<UnkPairTable_A6_b72*>(SubAreaManager()), reinterpret_cast<const u64*>(&stage),
                          reinterpret_cast<const u64*>(&next), &time, &blend);
            fn_71010e6278(CameraOf(self), reinterpret_cast<UnkCamTarget2_ovb5*>(self), 0.0f, -1.0f, 0.01f);
            fn_71010e6278(CameraOf(self), reinterpret_cast<UnkCamTarget2_ovb5*>(self), time, 4.0f, blend);
        }
        return;
    }
    f32 ax = self->Call<f32>(0x150) + self->offset.x;
    f32 ay = self->Call<f32>(0x158) + self->offset.y;
    f32 az = self->Call<f32>(0x160) + self->offset.z;
    UnkVec_l7643c0 from;
    from.x = ax;
    from.y = ay;
    from.z = az;
    f32 bx = self->Call<f32>(0x168) + self->offset.x;
    f32 by = self->Call<f32>(0x170) + self->offset.y;
    f32 bz = self->Call<f32>(0x178) + self->offset.z;
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    f32 inverse = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    self->current.origin.y = self->field_0x26c + by;
    self->current.origin.z = bz;
    self->current.origin.x = self->field_0x268 + bx;
    self->current.direction.x = dx * inverse;
    self->current.direction.y = dy * inverse;
    self->current.direction.z = dz * inverse;
    if (reset) {
        if (self->owner->object->field_0x20) {
            self->saved = self->current;
        }
        f32 out[3];
        self->Call<bool>(0x1b0, &self->offset, out);
    }
    if (self->Call<bool>(0x1a8))
        fn_7100764a50(reinterpret_cast<UnkAnimatedCameraCtrl_b0101*>(self), reinterpret_cast<UnkCamVec3_b0101*>(&from), nullptr, false);
    if (reset)
        self->saved = self->current;
    f32 x = self->current.origin.x;
    f32 y = self->current.origin.y;
    bool movedX = false;
    if (!(fabsf(self->field_0x270 - x) > 2.0f) && fabsf(self->field_0x27c - self->field_0x284) > 2.0f) {
        x = self->field_0x270;
        movedX = true;
        self->current.origin.x = x;
    }
    self->field_0x290 = movedX;
    if (!(fabsf(self->field_0x274 - y) > 4.0f) && fabsf(self->field_0x280 - self->field_0x288) > 2.0f) {
        self->field_0x291 = true;
        y = self->field_0x274;
        self->current.origin.y = y;
    } else {
        self->field_0x291 = false;
    }
    self->field_0x270 = x;
    self->field_0x274 = y;
    self->field_0x27c = self->field_0x284;
    self->field_0x278 = self->current.origin.z;
    self->field_0x280 = self->field_0x288;
}
