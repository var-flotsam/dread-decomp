// In the CSamusMagnetGloveMovementState::CParams unit: fn_71009a6310 decides whether a magnet surface can be
// grabbed: fast enough, accepted by fn_71009a64cc, facing it, the input pushing towards it and enough time since
// the last grab (unless it is the same surface again). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern UnkTunable g_unk_717273c650;
struct UnkAirMoveState_B71a6;
struct UnkAirWords_B71a6;
struct UnkAirRequest_B71a6;
bool fn_71009a64cc(UnkAirMoveState_B71a6*, const UnkAirWords_B71a6*, s32, UnkAirRequest_B71a6*, s32);

static inline f32 TunableAt(u32 offset) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_717273c650) + offset);
}

struct UnkMagnetRef_ovb0082 {
    u8 pad_0x0[0x10];
    void* target;
};

static inline void* RefTarget(const UnkMagnetRef_ovb0082* ref) {
    return ref ? ref->target : nullptr;
}

struct UnkMagnetRequest_ovb0082 {
    u8 pad_0x0[0x48];
    s32 field_0x48;
    u8 pad_0x4c[0xc];
    CVector3D normal;    // 0x58
    UnkSegment2D segment;  // 0x64
};

struct UnkMagnetRequestTail_ovb0082 {
    u8 pad_0x0[0x78];
    s32 field_0x78;
    u8 pad_0x7c[0xc];
    UnkMagnetRef_ovb0082* field_0x88;
};

struct UnkMagnetMovement_ovb0082 {
    virtual void vf_0x0();
    u8 pad_0x8[0x50];
    CVector3D dir;  // 0x58

    bool vf_0x3b0() {
        void** vtable = *reinterpret_cast<void***>(this);
        return reinterpret_cast<bool (*)(UnkMagnetMovement_ovb0082*)>(vtable[0x3b0 / 8])(this);
    }
};

struct UnkMagnetActor_ovb0082 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;
};

struct UnkMagnetCheckState_ovb0082 {
    u8 pad_0x0[0x30];
    UnkMagnetActor_ovb0082* actor;
    UnkMagnetMovement_ovb0082* movement;  // 0x38
    u8 pad_0x40[0x168];
    f32 field_0x1a8;
    f32 field_0x1ac;
    UnkMagnetRef_ovb0082* field_0x1b0;
    s32 field_0x1b8;
};

bool fn_71009a6310(UnkMagnetCheckState_ovb0082* self, f32 x, f32 y, f32 speed, const UnkAirWords_B71a6* words,
                   UnkMagnetRequest_ovb0082* req) {
    if (TunableAt(0x30) > speed)
        return false;
    if (!fn_71009a64cc(reinterpret_cast<UnkAirMoveState_B71a6*>(self), words, 1,
                       reinterpret_cast<UnkAirRequest_B71a6*>(req), 1))
        return false;
    if (req->field_0x48 == 0) {
        CVector3D fwd;
        if (self->movement->vf_0x3b0()) {
            const UnkMatrix44& m = fn_710002614c(&self->actor->xform);
            fwd = {m.m[2], m.m[6], m.m[10]};
        } else {
            fwd = self->movement->dir;
        }
        if (fwd.x * req->normal.x + fwd.y * req->normal.y + fwd.z * req->normal.z <= 0.0f)
            return false;
    }
    CVector2D dir;
    if (fn_7100125fbc(&req->segment) > 100.0f) {
        UnkVec2Result n = fn_7100125e78(&req->segment);
        dir = {-n.x, -n.y};
    } else {
        fn_710007b430(&dir, &req->normal);
    }
    f32 dot = dir.x * x + dir.y * y;
    if (dot < TunableAt(0x34))
        return false;
    f32 elapsed = *reinterpret_cast<f32*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x2444) - self->field_0x1a8;
    if (elapsed < TunableAt(0x38))
        return false;
    UnkMagnetRequestTail_ovb0082* tail = reinterpret_cast<UnkMagnetRequestTail_ovb0082*>(req);
    void* last = RefTarget(self->field_0x1b0);
    if (RefTarget(tail->field_0x88) == last && tail->field_0x78 == self->field_0x1b8 &&
        elapsed < self->field_0x1ac && dot < TunableAt(0x40))
        return false;
    return true;
}
