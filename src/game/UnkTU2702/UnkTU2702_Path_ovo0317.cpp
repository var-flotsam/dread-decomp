// Unit 2702 (hint CAIComponent): an AI component's distance to its path's first point and the
// handling of a triggered event (forwarded to the owner's target when accepted). Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/CClass.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkPathOwner_b0143 {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0x1c];
    f32 field_0x64;
};

struct UnkPathPoint_b0143 {
    f32 x, y, z;
};

struct UnkPathSelf_b0143 {
    u8 pad_0x0[0x30];
    UnkPathOwner_b0143* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkPathPoint_b0143* points;  // 0x68
    u32 count;                   // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94
};

static inline const base::math::CVector2D& Pt(const UnkPathPoint_b0143* p) {
    return *reinterpret_cast<const base::math::CVector2D*>(p);
}

// The distance along x from the owner to the path's first point (false when the path has no segment).
bool fn_71010fc5fc(base::reflection::UnkReflObject* component, f32* out) {
    UnkPathSelf_b0143* self = reinterpret_cast<UnkPathSelf_b0143*>(component);
    u32 count = self->count;
    bool open = self->closed == 0;
    if (count == ((count != 0) & open))
        return false;
    UnkPathPoint_b0143* points = self->points;
    UnkSegment2D seg(Pt(points), Pt(count == 1 ? points : points + 1));
    f32 x = seg.start.x;
    f32 owner_x = self->owner->field_0x44;
    *out = __builtin_fabsf(owner_x - x);
    return true;
}

struct UnkEventSelf_b0143;
bool fn_7101105304(UnkEventSelf_b0143* self, CStrId* id, f32 a, bool b, bool c, bool d, f32 e, f32 f, f32 g,
                   f32 h, f32 i, f32 i2, u32 j, u32 k, bool l);

struct UnkEventArg_b0143 {
    u8 pad_0x0[0x10];
    CStrId id;  // 0x10
    f32 field_0x18;
    bool field_0x1c;
    bool field_0x1d;
    bool field_0x1e;
    u8 pad_0x1f;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    bool field_0x44;
};

struct UnkEventTarget_b0143 {
    void** vtable;
    void vf_0x168(CStrId* id, bool a, bool b, u32 c, f32 d) {
        reinterpret_cast<void (*)(UnkEventTarget_b0143*, CStrId*, bool, bool, u32, f32)>(vtable[0x168 / 8])(
            this, id, a, b, c, d);
    }
};

struct UnkEventOwner_b0143 {
    u8 pad_0x0[0x278];
    UnkEventTarget_b0143* field_0x278;
};

struct UnkEventSelf_b0143 {
    u8 pad_0x0[0x30];
    UnkEventOwner_b0143* owner;
};

// Forwards an accepted event to the owner's target.
void fn_7101105750(UnkEventSelf_b0143* self, UnkEventArg_b0143* arg) {
    if (arg->id.data == &g_unk_7101d47108->empty)
        return;
    if (fn_7101105304(self, &arg->id, arg->field_0x18, arg->field_0x1c, arg->field_0x1d, arg->field_0x1e,
                      arg->field_0x20, arg->field_0x24, arg->field_0x2c, arg->field_0x28, arg->field_0x30,
                      arg->field_0x34, arg->field_0x3c, arg->field_0x40, arg->field_0x44))
        self->owner->field_0x278->vf_0x168(&arg->id, true, true, arg->field_0x38, -1.0f);
}
