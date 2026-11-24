// CSamusGunComponent unit (unit 1315 of tools/tu_map.py): the gun's per-frame update. With the screw attack it
// applies a pending beam change; a pending change or reset then either hands the owner's contacts to their
// listeners in reverse or is applied; last it keeps the animation in step with two states and a timer.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCameraComponent.h"
#include "game/CGunComponent.h"
#include "game/CInventoryComponent.h"
#include "game/CSamusGunComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

#include <new>

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;  // reached through the GOT

extern char g_unk_7101a795e0[];  // the contact list's vtable
extern char g_unk_7101a6ba70[];  // its base's vtable

struct UnkGunBeamInfo_ovb0070 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkGunBeam_ovb0070 {
    UnkGunBeamInfo_ovb0070* info;
};

struct UnkSamusGun_ovb0070;
struct UnkGun_ovb0091;
struct UnkEnergyGun_b52;
struct UnkEnergyGun_A4_b76;
struct UnkSamusGun_l858818;
void fn_710083db50(UnkGun_ovb0091* self, f32 dt);
void fn_7100859544(UnkSamusGun_l858818* self);
u64 CSamusGunComponent_GetPrimaryWeaponKey(UnkSamusGun_l858818* self);
u64 CSamusGunComponent_GetSecondaryWeaponSelectKey(UnkEnergyGun_b52* self);
u64 fn_7100859a20(UnkEnergyGun_A4_b76* self);
bool fn_7100d5bfbc(void* tracker, u32 mask, UnkCollisionObject* object, void* arg, u32);

struct UnkVec3_l858818 {
    f32 x;
    f32 y;
    f32 z;
};

// What a contact touched: its +0x10 is the reflected component it belongs to.
struct UnkContactShape_l858818;
struct UnkContactShapeVtable_l858818 {
    void* slots[0xb];
    u32 (*vf_0x58)(UnkContactShape_l858818* self);  // flags
};
struct UnkContactShape_l858818 {
    UnkContactShapeVtable_l858818* vtable;
    u8 pad_0x8[0x8];
    base::reflection::UnkReflObject* component;
};

struct UnkContactBody_l858818 : UnkRefObject {
    UnkContactShape_l858818* shape;
};

// A counted reference to a contact body; assignment is out of line (fn_71002a5d1c).
struct UnkContactRef_l858818 {
    UnkContactBody_l858818* ptr = nullptr;

    UnkContactRef_l858818() = default;
    UnkContactRef_l858818(const UnkContactRef_l858818& other) {
        if (UnkContactBody_l858818* p = other.ptr) {
            ++p->ref_count;
            ptr = p;
        }
    }
    ~UnkContactRef_l858818() {
        if (UnkContactBody_l858818* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
    UnkContactRef_l858818& operator=(const UnkContactRef_l858818& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

// One contact between two bodies: the points on each and how far along the move it happened.
struct UnkContact_l858818 {
    UnkContactRef_l858818 body;
    UnkContactRef_l858818 other;
    s32 field_0x10 = -1;
    UnkVec3f normal = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkVec3f point = UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkVec3f other_point = UnkVec3f(0.0f, 0.0f, 0.0f);
    f32 fraction = 0;
    u32 field_0x3c = 0;
    u32 field_0x40 = 0;

    // The same contact seen from the other body.
    void Invert() {
        UnkContactRef_l858818 tmp(body);
        body = other;
        other = tmp;
        UnkVec3f p = point;
        point = other_point;
        other_point = p;
        fraction = 1.0f - fraction;
    }
};
static_assert(sizeof(UnkContact_l858818) == 0x48, "UnkContact_l858818 layout");

// Up to ten contacts, kept in place, and the point the query starts from.
struct UnkContactList_l858818 {
    void* vtable;
    u32 count;
    u8 storage[10 * sizeof(UnkContact_l858818)];
    UnkVec3_l858818 origin;

    explicit UnkContactList_l858818(const UnkVec3_l858818& from) : vtable(g_unk_7101a795e0 + 0x10), count(0), origin(from) {}
    ~UnkContactList_l858818() {
        vtable = g_unk_7101a6ba70 + 0x10;
        for (u64 i = 0; i < count; ++i)
            items()[i].~UnkContact_l858818();
    }

    UnkContact_l858818* items() { return reinterpret_cast<UnkContact_l858818*>(storage); }

    // The contact at `i`, adding empty ones up to it.
    UnkContact_l858818& At(u64 i) {
        if (i >= count) {
            for (u64 j = count; j < i + 1; ++j)
                new (&items()[j]) UnkContact_l858818;
            count = i + 1;
        }
        return items()[i];
    }
};
static_assert(__builtin_offsetof(UnkContactList_l858818, origin) == 0x2dc, "layout");

struct UnkContactTarget_l858818;
struct UnkGunOwner_l858818;
struct UnkContactTargetVtable_l858818 {
    void* slots[0x33];
    void (*vf_0x198)(UnkContactTarget_l858818* self, UnkGunOwner_l858818* by, UnkContact_l858818* contact,
                     UnkContactShape_l858818* shape, bool flag);
};
struct UnkContactTarget_l858818 {
    UnkContactTargetVtable_l858818* vtable;
};

struct UnkWeight_l858818 {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};

struct UnkGunOwner_l858818 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
    u8 pad_0x1b6[0x2];
    UnkWeight_l858818* field_0x1b8;
    u8 pad_0x1c0[0x48];
    UnkFlagged* field_0x208;
    u8 pad_0x210[0x10];
    base::reflection::UnkReflObject* field_0x220;  // the animation component
    u8 pad_0x228[0x30];
    UnkContactTarget_l858818* field_0x258;
    u8 pad_0x260[0x10];
    UnkInventory* field_0x270;
    u8 pad_0x278[0x28];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkCollisionComponent_l858818 {
    u8 pad_0x0[0x30];
    UnkGunOwner_l858818* owner;
};

struct UnkCollisionShape_l858818;
struct UnkCollisionShapeVtable_l858818 {
    void* slots[0xe];
    const f32 (*(*vf_0x70)(UnkCollisionShape_l858818* self))[4];  // its 3x4 transform
};
struct UnkCollisionShape_l858818 {
    UnkCollisionShapeVtable_l858818* vtable;
};

struct UnkAimCameraCtrl_l858818 {
    u8 pad_0x0[0x240];
    bool field_0x240;
};

// An entry of the array at +0x348: a counted reference and two words.
struct UnkGunShot_l858818 {
    UnkRefObject* ref;
    u64 field_0x8;
    u32 field_0x10;

    ~UnkGunShot_l858818() {
        if (UnkRefObject* p = ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};
static_assert(sizeof(UnkGunShot_l858818) == 0x18, "UnkGunShot_l858818 layout");

struct UnkSamusGun_l858818 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkGunOwner_l858818* owner;  // 0x30
    u8 pad_0x38[0xf0];
    bool field_0x128;
    u8 pad_0x129[0x207];
    bool field_0x330;  // a beam change is pending
    u8 pad_0x331[0xf];
    bool field_0x340;  // a reset is pending
    f32 field_0x344;   // how long it has been pending
    UnkGunShot_l858818* shots;
    u32 shot_count;
    u8 pad_0x354[0x2c];
    bool field_0x380;
    f32 field_0x384;   // a timer
    bool field_0x388;  // the owner has the central unit energy
    bool field_0x389;  // the autofocus aim camera is in control
};
static_assert(__builtin_offsetof(UnkSamusGun_l858818, field_0x384) == 0x384, "layout");

static inline bool HasItem_l858818(UnkInventory* inventory, UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return fn_71011428bc(inventory, qid);
}

static inline UnkActor* OwnerActor(UnkSamusGun_l858818* self) {
    return reinterpret_cast<UnkActor*>(self->owner);
}

// A pending beam change (+0x330) removes the owner's timeline that goes with the current beam.
static inline void ApplyBeamChange_l858818(UnkSamusGun_l858818* self) {
    if (self->field_0x330) {
        self->field_0x330 = false;
        if (self->owner) {
            switch (fn_7100840828(reinterpret_cast<UnkSamusGun_ovb0070*>(self))->info->id) {
            case 0x6803e9b3d5c26ba0:
            case 0xb23e2ea9edd95fb7:
                fn_7100d7c94c(OwnerActor(self), &g_unk_710164ad48);
                break;
            case 0xa87817d815624dd3:
                fn_7100d7c94c(OwnerActor(self), &g_unk_710164ad50);
                break;
            case 0xa7b8a8ac9c26afaa:
                fn_7100d7c94c(OwnerActor(self), &g_unk_710164ad58);
                break;
            }
        }
    }
}

// A pending reset (+0x340) drops the entries and removes two timelines.
static inline void ApplyReset_l858818(UnkSamusGun_l858818* self) {
    if (self->field_0x340) {
        self->field_0x340 = false;
        self->field_0x344 = 0;
        for (u64 i = 0; i < self->shot_count; ++i)
            self->shots[i].~UnkGunShot_l858818();
        self->shot_count = 0;
        if (self->owner) {
            fn_7100d7c94c(OwnerActor(self), &g_unk_710164ad68);
            fn_7100d7c94c(OwnerActor(self), &g_unk_710164ad60);
        }
    }
}

// The owner's contacts, each handed in reverse to the actor it touched.
static inline void SendContacts_l858818(UnkSamusGun_l858818* self) {
    void* tracker = g_unk_7101d47310->field_0x608->field_0x820;
    UnkCollisionContainer* container = self->owner->field_0x2a0;
    // Needs the slot as a member of include/game/UnkGameManagerIdTable.h: base::global::CStrId field_0x2180.
    UnkCollisionObject* object = fn_7100864194(container, &GameManagerIdTable().field_0x2180);
    const f32(*transform)[4] = reinterpret_cast<UnkCollisionShape_l858818*>(object)->vtable->vf_0x70(
        reinterpret_cast<UnkCollisionShape_l858818*>(object));

    UnkVec3_l858818 position = {transform[0][3], transform[1][3], transform[2][3]};
    UnkContactList_l858818 contacts(position);
    if (!fn_7100d5bfbc(tracker, 0x1080, object, &contacts, 0))
        return;

    for (u64 i = 0; i < contacts.count; ++i) {
        UnkContact_l858818& contact = contacts.items()[i];
        if (!contact.body.ptr || !contact.body.ptr->shape)
            continue;
        UnkContactShape_l858818* shape = contact.body.ptr->shape;
        if (shape->vtable->vf_0x58(shape) & (1 << 26))
            continue;

        // The original tests the component's class and goes on whatever the answer (an assertion).
        base::reflection::UnkReflObject* component = contacts.At(i).body.ptr->shape->component;
        fn_710009d50c(component->vf_0x0(), fn_710085fc44());
        UnkGunOwner_l858818* actor = reinterpret_cast<UnkCollisionComponent_l858818*>(component)->owner;
        if (actor->field_0x1b5)
            continue;
        if (actor->field_0x1b8 && actor->field_0x1b8->field_0x70 == 2)
            continue;
        UnkContactTarget_l858818* target = actor->field_0x258;
        if (!target)
            continue;

        UnkContact_l858818 reversed = contacts.At(i);
        reversed.Invert();
        target->vtable->vf_0x198(target, self->owner, &reversed,
                                 reversed.other.ptr ? reversed.other.ptr->shape : nullptr, false);
    }
}

static inline void SendAnimation_l858818(base::reflection::UnkReflObject* animation, u64 action) {
    base::reflection::refl_EmitU32(animation, action, 0x5d523009b8f43e5f, 0x80, &g_unk_710164b958,
                                   &g_unk_7172769740);
}

static inline u64 StateAction_l858818(UnkSamusGun_l858818* self) {
    if (self->field_0x128)
        return CSamusGunComponent_GetPrimaryWeaponKey(self);
    return CSamusGunComponent_GetSecondaryWeaponSelectKey(reinterpret_cast<UnkEnergyGun_b52*>(self));
}

void fn_7100858818(UnkSamusGun_l858818* self, f32 dt) {
    fn_710083db50(reinterpret_cast<UnkGun_ovb0091*>(self), dt);
    if (self->field_0x340)
        self->field_0x344 += dt;

    UnkInventory* inventory = self->owner->field_0x270;
    if (inventory && HasItem_l858818(inventory, g_unk_717275a918)) {
        ApplyBeamChange_l858818(self);
        if (self->owner && self->owner->field_0x208 && fn_71009e75e8(self->owner->field_0x208))
            fn_7100859544(self);
    }

    if ((self->field_0x330 || self->field_0x340) && self->owner && self->owner->field_0x2a0) {
        if (self->owner->field_0x208 && fn_71009e75e8(self->owner->field_0x208)) {
            SendContacts_l858818(self);
        } else {
            ApplyBeamChange_l858818(self);
            ApplyReset_l858818(self);
        }
    }

    base::reflection::UnkReflObject* animation = self->owner->field_0x220;
    if (!animation || !fn_710009d50c(animation->vf_0x0(), fn_7100713bb0()))
        return;

    bool had_energy = self->field_0x388;
    UnkInventory* owner_inventory = self->owner->field_0x270;
    bool has_energy = owner_inventory ? HasItem_l858818(owner_inventory, g_unk_717275ac18) : false;
    self->field_0x388 = has_energy;
    if (had_energy != has_energy)
        SendAnimation_l858818(animation, StateAction_l858818(self));

    bool was_aiming = self->field_0x389;
    bool aiming = false;
    if (void* camera = fn_7100defcec(g_unk_7101d47310)) {
        if (base::reflection::UnkReflObject* ctrl = fn_71010e37f4(camera)) {
            if (fn_710009d50c(ctrl->vf_0x0(), fn_7100720464()))
                aiming = !reinterpret_cast<UnkAimCameraCtrl_l858818*>(ctrl)->field_0x240;
        }
    }
    self->field_0x389 = aiming;
    if (was_aiming != aiming) {
        if (aiming) {
            self->field_0x380 = false;
            fn_7100859938(reinterpret_cast<UnkSamusGun_b58*>(self));
        } else if (self->field_0x384 <= 0.0f) {
            self->field_0x380 = true;
            fn_7100859938(reinterpret_cast<UnkSamusGun_b58*>(self));
        }
        SendAnimation_l858818(animation, StateAction_l858818(self));
    }

    if (self->field_0x384 != 0.0f) {
        self->field_0x384 -= dt;
        if (self->field_0x384 <= 0.0f) {
            self->field_0x384 = 0.0f;
            if (!self->field_0x128)
                SendAnimation_l858818(animation, fn_7100859a20(reinterpret_cast<UnkEnergyGun_A4_b76*>(self)));
        }
    }
}
