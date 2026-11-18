// CCaterzillaAIComponent's unit (tools/tu_map.py unit 602): the update. After the base update, unless the
// owner's +0x1b8 is in state 2: sends the pending request, runs the state-1 step, advances the spawn point's
// timer and, once it has run out, asks the spawn point (or its chain) for a free segment in front of the
// owner; when one is found the component is marked busy, told the result (fn_71003f7488) and the found
// link's actor's mode is started. Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkMatrix44.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/CGameLink.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct CActor;
struct UnkCaterzillaActor_ovb0054;
struct UnkCaterzillaMode_ovb0054;
struct UnkCaterzillaSpawn3_ovb0054;
struct UnkCaterzillaAI_l3f7034;

struct UnkEntry_b41a1 {
    virtual ~UnkEntry_b41a1();
    u8 pad_0x8[0x38];
};

void fn_7100246a94(base::global::CRntVector<UnkEntry_b41a1>* self, const base::global::CRntVector<UnkEntry_b41a1>* other);
void fn_71003f7034(UnkCaterzillaAI_l3f7034* self, bool first, bool second);
UnkCaterzillaMode_ovb0054* fn_71003f7fa0(UnkCaterzillaActor_ovb0054* actor);
bool fn_7100a17628(UnkCaterzillaSpawn3_ovb0054* self, const CVector2D* pos, u32 index, const CVector2D* dir);

struct UnkCaterzillaAI_l3f78cc;
struct UnkCaterzillaSpawn_l3f78cc;

void fn_71003f7e80(UnkCaterzillaAI_l3f78cc* self);
void fn_71003f7488(UnkCaterzillaAI_l3f78cc* self, bool blocked, f32 value);
bool fn_7100a16c50(UnkCaterzillaSpawn_l3f78cc* self, const CVector2D* pos, const CVector2D* front,
                   const CVector2D* up, f32* value);
s32 fn_7100a17190(UnkCaterzillaSpawn_l3f78cc* self, const CVector2D* offset, const CVector2D* front,
                  const CVector2D* up, f32* value);
void fn_7100a1d23c(UnkCaterzillaMode_ovb0054* mode);

// A copy of a spawn point's links (fn_7100246a94), freed when it goes.
struct UnkCaterzillaLinksCopy_l3f78cc {
    UnkEntry_b41a1* data;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    base::UnkAllocator* allocator;  // 0x20

    explicit UnkCaterzillaLinksCopy_l3f78cc(const void* other) {
        fn_7100246a94(reinterpret_cast<base::global::CRntVector<UnkEntry_b41a1>*>(this),
                      static_cast<const base::global::CRntVector<UnkEntry_b41a1>*>(other));
    }
    ~UnkCaterzillaLinksCopy_l3f78cc() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~UnkEntry_b41a1();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }

    CGameLink<CActor>* At(u32 index) {
        return reinterpret_cast<CGameLink<CActor>*>(
            fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(this), index));
    }
};
static_assert(sizeof(UnkCaterzillaLinksCopy_l3f78cc) == 0x28, "links layout");

struct UnkCaterzillaSpawn_l3f78cc {
    u8 pad_0x0[0x3b8];
    s32 field_0x3b8;
    u8 pad_0x3bc[0x3d1 - 0x3bc];
    bool field_0x3d1;
    u8 pad_0x3d2[0x420 - 0x3d2];
    UnkCaterzillaSpawn_l3f78cc* next;  // 0x420
    u8 pad_0x428[0x438 - 0x428];
    u8 links[0x28];  // 0x438
    bool field_0x460;
};
static_assert(__builtin_offsetof(UnkCaterzillaSpawn_l3f78cc, field_0x460) == 0x460, "spawn layout");

struct UnkCaterzillaOwnerState_l3f78cc {
    u8 pad_0x0[0x70];
    s32 state;
};

// The movement's sub-object (+0x8e8 of the movement when it is of kind 1).
struct UnkCaterzillaMovement_l3f78cc {
    u8 pad_0x0[0x8c];
    CVector2D offset;  // 0x8c
    u8 pad_0x94[0xa8 - 0x94];
    u8 field_0xa8;
};

struct UnkCaterzillaMover_l3f78cc {
    u8 pad_0x0[0x8e8];
    UnkCaterzillaMovement_l3f78cc movement;  // 0x8e8
};

struct UnkCaterzillaOwner_l3f78cc {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x1b8 - 0x78];
    UnkCaterzillaOwnerState_l3f78cc* field_0x1b8;
    u8 pad_0x1c0[0x218 - 0x1c0];
    UnkCaterzillaMover_l3f78cc* field_0x218;
};

struct UnkCaterzillaAI_l3f78cc {
    u8 pad_0x0[0x30];
    UnkCaterzillaOwner_l3f78cc* owner;  // 0x30
    u8 pad_0x38[0x146d8 - 0x38];
    s32 field_0x146d8;
    u8 pad_0x146dc[0x146f0 - 0x146dc];
    UnkCaterzillaSpawn_l3f78cc* spawn;  // 0x146f0
    bool busy;                          // 0x146f8
    u8 pad_0x146f9[0x14710 - 0x146f9];
    f32 delay;  // 0x14710
    f32 timer;  // 0x14714
    u8 ready;  // 0x14718
    u8 pad_0x14719[0x1471d - 0x14719];
    bool field_0x1471d;
    u8 pad_0x1471e[0x14720 - 0x1471e];
    f32 field_0x14720;
};
static_assert(__builtin_offsetof(UnkCaterzillaAI_l3f78cc, field_0x14720) == 0x14720, "AI layout");

// A copy of the link `index` of the spawn point's links, taken from a copy of the links: an empty link
// given the other's handle, state and callbacks.
static inline CGameLink<CActor> LinkAt(UnkCaterzillaSpawn_l3f78cc* spawn, u32 index) {
    UnkCaterzillaLinksCopy_l3f78cc links(spawn->links);
    const CGameLink<CActor>& other = *links.At(index);
    CGameLink<CActor> link;
    link.state = 0;
    link.handle.ptr = nullptr;
    link.handle.Set(other.handle.ptr);
    link.state = other.state;
    fn_710023b29c(&link.callbacks, &other.callbacks);
    return link;
}

static inline UnkCaterzillaMovement_l3f78cc* Movement(UnkCaterzillaAI_l3f78cc* self) {
    UnkCaterzillaMover_l3f78cc* mover = self->owner->field_0x218;
    return fn_71010cc3a4(mover) == 1 ? &mover->movement : nullptr;
}

// The vector's projection on the plane (fn_710007b430).
static inline void Planar(CVector2D* out, const CVector3D& v) {
    fn_710007b430(out, &v);
}

static inline CVector3D Position(UnkCaterzillaOwner_l3f78cc* owner) {
    return CVector3D{owner->xform.m[3], owner->xform.m[7], owner->xform.m[11]};
}

static inline CVector3D Front(UnkCaterzillaOwner_l3f78cc* owner) {
    const UnkMatrix44& m = fn_710002614c(&owner->xform);
    return CVector3D{m.m[2], m.m[6], m.m[10]};
}

static inline CVector3D Up(UnkCaterzillaOwner_l3f78cc* owner) {
    const UnkMatrix44& m = fn_710002614c(&owner->xform);
    return CVector3D{m.m[1], m.m[5], m.m[9]};
}

void fn_71003f78cc(UnkCaterzillaAI_l3f78cc* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    if (self->owner->field_0x1b8->state == 2)
        return;

    if (self->field_0x1471d) {
        UnkCaterzillaSpawn_l3f78cc* spawn = self->spawn;
        self->field_0x1471d = false;
        if (spawn)
            fn_71003f7034(reinterpret_cast<UnkCaterzillaAI_l3f7034*>(self), spawn->field_0x460,
                          spawn->field_0x3b8 == 0);
    }
    if (self->field_0x146d8 == 1)
        fn_71003f7e80(self);
    if (self->spawn && self->spawn->field_0x3d1) {
        self->timer += dt;
        if (self->timer >= self->delay)
            self->ready = true;
    }

    UnkCaterzillaMovement_l3f78cc* movement = Movement(self);
    bool locked = fn_71010cc3a4(self->owner->field_0x218) == 1 && Movement(self)->field_0xa8;
    if (!movement)
        return;
    UnkCaterzillaSpawn_l3f78cc* spawn = self->spawn;
    if (!spawn || self->busy || (locked | (self->ready == 0)))
        return;

    f32 value = self->field_0x14720;
    if (spawn->next || spawn->field_0x3d1) {
        CVector2D pos, front, up;
        Planar(&pos, Position(self->owner));
        Planar(&front, Front(self->owner));
        CVector3D up_axis = Up(self->owner);
        Planar(&up, up_axis);
        if (fn_7100a16c50(spawn, &pos, &front, &up, &value)) {
            self->busy = true;
            UnkCaterzillaSpawn_l3f78cc* last = self->spawn->next ? self->spawn->next : self->spawn;
            fn_71003f7488(self, last->field_0x3b8 == 0, value);
        }
        return;
    }

    CVector2D offset = movement->offset;
    CVector2D front, up;
    Planar(&front, Front(self->owner));
    Planar(&up, Up(self->owner));
    s32 index = fn_7100a17190(spawn, &offset, &front, &up, &value);
    if (index == -1)
        return;
    self->busy = true;
    UnkCaterzillaSpawn_l3f78cc* current = self->spawn;
    CVector2D pos, dir;
    Planar(&pos, Position(self->owner));
    Planar(&dir, Front(self->owner));
    bool blocked = fn_7100a17628(reinterpret_cast<UnkCaterzillaSpawn3_ovb0054*>(current), &pos, index, &dir);
    fn_71003f7488(self, !blocked, value);

    CGameLink<CActor> link = LinkAt(self->spawn, index);
    if (link.state == 1) {
        {
            UnkRefPtr ref = fn_710023b580(&link.callbacks);
            fn_7100234f78(&link.handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.state = 2;
    }
    if (UnkCaterzillaMode_ovb0054* mode = fn_71003f7fa0(reinterpret_cast<UnkCaterzillaActor_ovb0054*>(link.Target())))
        fn_7100a1d23c(mode);
}
