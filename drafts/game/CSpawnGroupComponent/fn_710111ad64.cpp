// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): whether the spawn group's collider touches the
// player's. Without a collider of its own, true unless the game manager's check holds. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "game/CCharacterMovement.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

struct UnkSpawnBounds_11ad64 {
    f32 minX, minY, maxX, maxY;
};

// A collider as this test reads it: its cached bounds, whether they need refreshing and the owner the shape
// tester takes.
struct UnkSpawnCollider_11ad64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void UpdateBounds();  // vf_0x98
    u8 pad_0x8[0x38];
    UnkSpawnBounds_11ad64 bounds;  // 0x40
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    u8 pad_0x61[0xf7];
    void* owner;  // 0x158

    UnkSpawnBounds_11ad64 GetBounds() {
        if (dirty)
            UpdateBounds();
        return bounds;
    }
};
static_assert(__builtin_offsetof(UnkSpawnCollider_11ad64, owner) == 0x158, "UnkSpawnCollider_11ad64 layout");

struct UnkSpawnPlayer_11ad64 {
    u8 pad_0x0[0x200];
    UnkFlagged* movement;  // 0x200
};

struct UnkSpawnGroup_11ad64 {
    u8 pad_0x0[0x1a8];
    UnkSpawnCollider_11ad64* collider;  // 0x1a8
};

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

// Two placed shapes against each other, with no filter.
static inline UnkColliderResult TestShapes(UnkShapeTester_A5_b68* tester, void* owner, const UnkMatrix44& transform,
                                           void* otherOwner, const UnkMatrix44& otherTransform) {
    return fn_7100d51368(tester, owner, const_cast<UnkMatrix44*>(&transform), otherOwner, otherTransform, nullptr, 0);
}

struct UnkSpawnGroupUpdate_ovb0065;

bool fn_710111ad64(UnkSpawnGroupUpdate_ovb0065* group) {
    UnkSpawnGroup_11ad64* self = reinterpret_cast<UnkSpawnGroup_11ad64*>(group);
    UnkSpawnPlayer_11ad64* player = reinterpret_cast<UnkSpawnPlayer_11ad64*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    if (self->collider) {
        UnkSpawnCollider_11ad64* other =
            reinterpret_cast<UnkSpawnCollider_11ad64*>(fn_7100976588(player->movement));
        if (!other)
            return false;
        UnkSpawnBounds_11ad64 mine = self->collider->GetBounds();
        UnkSpawnBounds_11ad64 theirs = other->GetBounds();
        if (!(mine.minX <= theirs.maxX && mine.maxX >= theirs.minX && mine.minY <= theirs.maxY &&
              mine.maxY >= theirs.minY))
            return false;

        UnkShapeTester_A5_b68* tester = Tester();
        UnkSpawnCollider_11ad64* collider = self->collider;
        if (TestShapes(tester, collider->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider)), other->owner,
                       fn_7100d36a2c(reinterpret_cast<UnkShape*>(other)))
                .bits &
            1)
            return true;
    } else if (!fn_7100deabe4(g_unk_7101d47310)) {
        return true;
    }
    return false;
}
