// CGoliathXBurstProjectionBombsAttack's unit (tools/tu_map.py, unit 835): ends the attack: after the base
// step, stops the owner's navigation when its current move is of kind 1, kills every burst-projection bomb
// in the sphere list, clears track 0x40 of the owner's animation and drops the weak handle at +0x88.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CWeaponMovement.h"
#include "game/UnkAttack.h"
#include "game/UnkShakernautId.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];

// The parameter block of the vtable-0x7101a25b38 kind with every option left unset; its reflected value
// (+0x70) is destroyed afterwards.
struct UnkBombsParams_l5848f0 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[0x7];
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u8 pad_0x38[0x4];
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    u8 pad_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue value;  // 0x70
    bool field_0x88;

    UnkBombsParams_l5848f0() {
        field_0x1c = false;
        vtable = g_unk_7101a25b38 + 2;
        field_0x3c = false;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        __builtin_memset(field_0x8, 0, sizeof(field_0x8));
        field_0x6a = false;
        field_0x58 = false;
        field_0x88 = false;
        field_0x34 = 1;
        field_0x30 = true;
        __builtin_memset(field_0x48, 0, 16);
        __builtin_memset(field_0x20, 0, 16);
    }
    ~UnkBombsParams_l5848f0() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }
};

struct UnkSphereEntry {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkSphereList {
    UnkSphereEntry** data;
    u32 count;
};

struct UnkBombMovement_l5848f0 : UnkReflObject {
    u8 pad_0x8[0x28];
    void* owner;  // 0x30
};

// The navigation's current move: its kind at +0x10 of the object at +0x10.
struct UnkMoveKind_l5848f0 {
    u8 pad_0x0[0x10];
    s32 kind;
};

struct UnkMove_l5848f0 {
    u8 pad_0x0[0x10];
    UnkMoveKind_l5848f0* info;
};

struct UnkMoveSlots_l5848f0 {
    UnkMove_l5848f0* move;
    void* target;
};

struct UnkNavigation_l5848f0 {
    u8 pad_0x0[0xb0];
    UnkMoveSlots_l5848f0* slots;
};

struct UnkBombsOwner_l5848f0 {
    u8 pad_0x0[0x220];
    UnkShakernautCtrl_b53* animation;  // 0x220
    u8 pad_0x228[0x70];
    UnkNavigation_l5848f0* navigation;  // 0x298
};

struct UnkBombsAttack_l5848f0 {
    void** vtable;
    UnkBombsOwner_l5848f0* owner;  // 0x8
    u8 pad_0x10[0x4];
    bool field_0x14;
    u8 pad_0x15[0x5b];
    u8 link[0x18];          // 0x70
    UnkWeakHandle* handle;  // 0x88
};

static inline UnkBombMovement_l5848f0* AsBomb(UnkSphereEntry* entry) {
    UnkReflObject* object = entry ? entry->object : nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_7100589df4()))
        return static_cast<UnkBombMovement_l5848f0*>(object);
    return nullptr;
}

void fn_71005848f0(UnkBombsAttack_l5848f0* self) {
    UnkShakernautId_b53 track;  // the animation track cleared at the end
    fn_71003609b8(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x14 = false;
    if (UnkNavigation_l5848f0* navigation = self->owner->navigation) {
        UnkMoveSlots_l5848f0* slots = navigation->slots;
        if (slots->move && slots->move->info && slots->move->info->kind == 1 && slots->target) {
            UnkBombsParams_l5848f0 params;
            fn_71006f4b0c(navigation, &params, 0);
        }
    }
    UnkSphereList* list = fn_7100ac73b0();
    for (UnkSphereEntry **it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (UnkBombMovement_l5848f0* bomb = AsBomb(*it))
            fn_7100232c68(bomb->owner);
    }
    if (UnkShakernautCtrl_b53* animation = self->owner->animation) {
        track.value = 0x40;
        fn_71011785a8(animation, &track, true, false);
    }
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target) {
            fn_710117b3f0(handle->target, self->link);
            handle = self->handle;
            if (!handle)
                return;
        }
        if (--handle->ref_count == 0)
            handle->vf_0x10();
        self->handle = nullptr;
    }
}
