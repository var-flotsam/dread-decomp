// CMorphBallMovement's unit (hint only): whether the ball may leave the ground. Not while +0x200 is above
// +0x22ac with +0xf9 set, not on a steep contact fn_71010d9638 accepts, not while turning with +0x2650 set,
// not (in mode 1 with no contact) over a floor a downward probe finds and fn_71010d9638 accepts, and not
// in the game manager's state of fn_7100df5a4c. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "types.h"

using base::math::CVector3D;

struct UnkInventory;
struct UnkMorphBallDamp;
bool fn_71009438e8(UnkMorphBallDamp* ball);
struct UnkCharacterMovement;
struct UnkCollisionHit;
bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);

// A counted reference released through the object's virtual 0x10.
struct UnkJumpRef_l94a4c8 {
    base::UnkRefCounted* ptr;

    UnkJumpRef_l94a4c8() : ptr(nullptr) {}
    ~UnkJumpRef_l94a4c8() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkJumpLink_l94a4c8 {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

// A collision hit (0x48 bytes): the link to what was hit, then the hit's id and values.
struct UnkJumpHit_l94a4c8 {
    UnkJumpRef_l94a4c8 link;
    UnkJumpRef_l94a4c8 field_0x8;
    s32 id;
    f32 values[12];

    UnkJumpHit_l94a4c8() : id(-1), values() {}

};
static_assert(sizeof(UnkJumpHit_l94a4c8) == 0x48, "UnkJumpHit_l94a4c8 layout");

struct UnkJumpOwner_l94a4c8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x270 - 0x68];
    UnkInventory* inventory;  // 0x270
};

struct UnkJumpBall_l94a4c8 {
    u8 pad_0x0[0x30];
    UnkJumpOwner_l94a4c8* owner;  // 0x30
    u8 pad_0x38[0xf9 - 0x38];
    u8 field_0xf9;
    u8 pad_0xfa[0x128 - 0xfa];
    UnkJumpHit_l94a4c8 contact;  // 0x128
    u8 pad_0x170[0x200 - 0x170];
    f32 field_0x200;
    u8 pad_0x204[0x3cc - 0x204];
    u32 mask;  // 0x3cc
    u8 pad_0x3d0[0x2288 - 0x3d0];
    s32 mode;  // 0x2288
    u8 pad_0x228c[0x22ac - 0x228c];
    f32 field_0x22ac;
    u8 pad_0x22b0[0x2650 - 0x22b0];
    f32 field_0x2650;
};
static_assert(__builtin_offsetof(UnkJumpBall_l94a4c8, field_0x2650) == 0x2650, "layout");

static inline const UnkQualifiedId& GetQualifiedId_l94a4c8(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Whether the ball's contact links to an object.
static inline bool HasContact(const UnkJumpBall_l94a4c8* self) {
    UnkJumpLink_l94a4c8* link = reinterpret_cast<UnkJumpLink_l94a4c8*>(self->contact.link.ptr);
    return link && link->target;
}

// Whether fn_71010d9638 accepts the hit for the ball.
inline bool Accepts(UnkJumpBall_l94a4c8* self, UnkJumpHit_l94a4c8* hit) {
    u64 unused = 0;
    return fn_71010d9638(reinterpret_cast<UnkMove_b152*>(self), reinterpret_cast<UnkHitInfo_b152*>(hit), &unused);
}

bool fn_710094a4c8(UnkJumpBall_l94a4c8* self) {
    // The spring ball test's result is not used.
    if (UnkInventory* inventory = self->owner->inventory)
        fn_71011428bc(inventory, GetQualifiedId_l94a4c8(g_unk_717275a998));  // ITEM_SPRING_BALL

    if (self->field_0xf9 && !(self->field_0x200 <= self->field_0x22ac))
        return false;
    if (HasContact(self) && self->contact.values[4] < -0.98f && Accepts(self, &self->contact))
        return false;
    if (fn_71009438e8(reinterpret_cast<UnkMorphBallDamp*>(self)) && self->field_0x2650 != 0.0f)
        return false;

    if (!HasContact(self) && self->mode == 1) {
        UnkJumpHit_l94a4c8 hit;
        UnkCharacterMovement* movement =
            reinterpret_cast<UnkCharacterMovement*>(fn_7100976588(reinterpret_cast<UnkFlagged*>(self)));
        UnkJumpOwner_l94a4c8* owner = self->owner;
        f32 x = owner->x;
        f32 y = owner->y;
        f32 z = owner->z;
        CVector3D pos = {x, y, z};
        CVector3D down;
        down.x = 0.0f;
        down.y = -1.0f;
        down.z = 0.0f;
        bool grounded = fn_7100977e50(movement, -0.3f, 20.0f, 3.4028235e38f, 0.0f, self->mask, &pos, nullptr,
                                      reinterpret_cast<UnkCollisionHit*>(&hit), &down) &&
                        Accepts(self, &hit);
        if (grounded)
            return false;
    }
    return !fn_7100df5a4c(g_unk_7101d47310);
}
