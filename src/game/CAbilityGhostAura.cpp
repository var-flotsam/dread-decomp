// Unit 430 (0x7100306910-0x71003162a8), whose reflected class is CAbilityGhostAura (the Flash
// Shift ability). Holds fn_710030c5fc, the lookup of the grapple beam gun among an owner's guns by
// its id. Names and layouts are placeholders inferred from the code.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

// A gun (a reflected object): its id at +0x20.
struct UnkAuraGun : UnkReflObject {
    u8 unk_0x8[0x18];
    CStrId id;
};

// The guns, in two lists of groups.
struct UnkAuraGunOwner {
    u8 unk_0x0[0xc8];
    base::UnkVector<base::UnkVector<UnkAuraGun*>> guns_0xc8;
    base::UnkVector<base::UnkVector<UnkAuraGun*>> guns_0xf0;
};

struct CGrappleBeamGun;

static inline CGrappleBeamGun* AsGrappleBeamGun(UnkAuraGun* gun) {
    if (!gun)
        return nullptr;
    return fn_710009d50c(gun->vf_0x0(), fn_7100832034()) ? reinterpret_cast<CGrappleBeamGun*>(gun) : nullptr;
}

// The last gun of the group with the id, as a grapple beam gun (null if it is another gun).
static inline CGrappleBeamGun* FindIn(const base::UnkVector<UnkAuraGun*>& group, const CStrId& id) {
    CGrappleBeamGun* found = nullptr;
    for (UnkAuraGun* const* it = group.data; it != group.data + group.count; ++it) {
        UnkAuraGun* gun = *it;
        if (gun->id.data == id.data)
            found = AsGrappleBeamGun(gun);
    }
    return found;
}

// The last gun of all groups with the id, as a grapple beam gun.
static inline CGrappleBeamGun* FindInAll(const base::UnkVector<base::UnkVector<UnkAuraGun*>>& groups,
                                         const CStrId& id) {
    CGrappleBeamGun* found = nullptr;
    for (auto* group = groups.data; group != groups.data + groups.count; ++group) {
        for (UnkAuraGun* const* it = group->data; it != group->data + group->count; ++it) {
            UnkAuraGun* gun = *it;
            if (gun->id.data == id.data)
                found = AsGrappleBeamGun(gun);
        }
    }
    return found;
}

// The grapple beam gun with the id: the last match in the first list, else the first group of the
// second list that has one.
// NON_MATCHING(minor): in the second list, a group without guns goes on to the next group directly
// (the original sets the result to null and tests it again) and the result is tested before the
// list's end is reloaded; the first list's empty case skips the same test. Blocks placed in another
// order otherwise.
CGrappleBeamGun* fn_710030c5fc(UnkAuraGunOwner* self, const CStrId& id) {
    if (CGrappleBeamGun* gun = FindInAll(self->guns_0xc8, id))
        return gun;
    for (auto* group = self->guns_0xf0.data; group != self->guns_0xf0.data + self->guns_0xf0.count; ++group) {
        if (CGrappleBeamGun* gun = FindIn(*group, id))
            return gun;
    }
    return nullptr;
}
