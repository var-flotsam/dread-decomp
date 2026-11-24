// behaviortree::CFleeFromTargetAction's unit: updates the node. It keeps a flee position fresh (asking
// fn_7100383c80 for a new one when the old one is unusable), reports 6 while the owner is still within the
// radius (+0x54) of it, and otherwise 2; when its ids match, it sends the owner's animation component a
// request or hands a parameter block to the target and reports 5. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector3D;

struct UnkFleeOwnerPart_l3849ec {
    u8 pad_0x0[0x68];
    u8 field_0x68[1];
};

struct UnkFleeFromTarget_l3849ec {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x10];
    UnkFleeOwnerPart_l3849ec* part;  // 0x30
    void* target;                    // 0x38
    u8 pad_0x40[0x4];
    bool field_0x44;
    bool field_0x45;
    u8 pad_0x46[0xe];
    f32 radius;  // 0x54
    u8 pad_0x58[0x18];
    CVector3D pos;  // 0x70
    bool field_0x7c;
    u8 pad_0x7d[3];
    CStrId id;  // 0x80
    bool field_0x88;
    bool field_0x89;
    bool field_0x8a;
    bool field_0x8b;
};

// The block fn_71006f4b0c takes for kind 5, as this node fills it.
struct UnkFleeParams_l3849ec {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 field_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

bool fn_7100383b64(UnkFleeFromTarget_l3849ec* self);
bool fn_7100383c80(UnkFleeFromTarget_l3849ec* self, CVector3D* pos, bool* out, bool* in);
bool fn_71003845a8(UnkFleeFromTarget_l3849ec* self);
u8 fn_710035e53c(char* p0);

static inline bool Within(f32 a, f32 b) {
    return a + 0.001f >= b && a - 0.001f <= b;
}

u32 fn_71003849ec(UnkFleeFromTarget_l3849ec* self) {
    u32 result;
    if (self->field_0x89 && !self->field_0x88) {
        UnkAnimationComponent* anim = nullptr;
        if (self->owner)
            anim = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
        if (anim) {
            if (fn_7101177ae8(anim, 0xf4061ff25a2163d6)->data == self->id.data &&
                !fn_71011782ec(anim, 0xf4061ff25a2163d6))
                return 2;
        }
        UnkFleeParams_l3849ec params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.vtable = g_unk_7101a25b10 + 2;
        __builtin_memset(params.field_0x20, 0, 16);
        params.field_0x39 = 0;
        params.field_0x3b = 1;
        params.field_0x3a = 1;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        params.field_0x30 = 0;
        fn_71006f4b0c(self->target, &params, 1);
        return 5;
    }
    CVector3D* current = &self->pos;
    CVector3D old = *current;
    bool ok;
    if (!self->field_0x45 && !(self->part && fn_710035e53c(reinterpret_cast<char*>(self->part) + 0x68))) {
        bool out = false;
        bool in = true;
        bool found = fn_7100383c80(self, current, &out, &in);
        if (self->field_0x8a) {
            self->field_0x8a = false;
            self->field_0x8b = in;
        }
        ok = false;
        if (found) {
            self->field_0x7c = out;
            if (out)
                self->field_0x88 = true;
            ok = fn_71003845a8(self);
        }
    } else {
        ok = fn_7100383b64(self);
    }
    if (!ok) {
        result = 6;
    } else if (!(self->radius > -1.0f)) {
        result = 2;
    } else if (Within(current->x, old.x) && Within(current->y, old.y) && Within(current->z, old.z)) {
        result = 2;
    } else {
        UnkActor* a = self->owner;
        f32 ax = a->field_0x44;
        f32 ay = a->field_0x54;
        f32 az = a->field_0x64;
        f32 dx = ax - current->x;
        f32 dy = ay - current->y;
        f32 dz = az - current->z;
        result = dx * dx + dy * dy + dz * dz < self->radius * self->radius ? 6 : 2;
    }
    if (!self->field_0x7c)
        return result;
    if (self->field_0x44) {
        CRntString name(g_unk_710162e2ed);
        self->id.Text();
        self->id.Text();
        if (self->id.data->Key() == static_cast<u64>(name.key) || self->field_0x88) {
        } else {
            bool flag = self->field_0x8b;
            if (flag) {
                if (self->field_0x89)
                    return result;
                if (self->id.data == &g_unk_7101d47108->empty)
                    return result;
                if (!self->owner)
                    return result;
                void* anim = self->owner->field_0x220;
                if (!anim)
                    return result;
                base::reflection::UnkReflTypeIdArg mode, mask;
                mode.id = 0;
                mask.id = UnkAll_SpitSend().value;
                if (!fn_7101176690(anim, self->id.data->Key(), 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                                   &g_unk_7172769740, &mask))
                    return result;
                self->field_0x89 = true;
                return 2;
            }
        }
    }
    if (self->field_0x7c)
        result = self->field_0x44 ? 5 : 2;
    return result;
}
