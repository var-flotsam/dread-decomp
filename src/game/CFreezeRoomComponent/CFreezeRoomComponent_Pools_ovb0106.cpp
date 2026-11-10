// CFreezeRoomComponent's unit (tools/tu_map.py unit 1537). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkMagmaArea_b50;
bool fn_7100a47e00(UnkMagmaArea_b50*, const CVector2D*);

struct UnkFreezeOwner_ovb0106 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

// A water pool as the freeze room sees it: its owner and the freeze setter (slot 0x218).
struct UnkFreezePool_ovb0106 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkFreezeOwner_ovb0106* owner;  // 0x30

    void SetFrozen(bool frozen) {
        using Fn = void (*)(UnkFreezePool_ovb0106*, bool);
        (*reinterpret_cast<Fn**>(this))[0x218 / 8](this, frozen);
    }
};

struct UnkFreezeHandle_ovb0106 {
    u8 pad_0x0[0x10];
    UnkReflObject* target;
};

struct UnkFreezeList_ovb0106 {
    UnkFreezeHandle_ovb0106** data;
    u32 count;
};

struct UnkFreezeRoom_ovb0106 {
    u8 pad_0x0[0x20];
    bool frozen;  // 0x20
};

UnkFreezeList_ovb0106* fn_71009088a0(UnkFreezeRoom_ovb0106* self);

// Sets the room's state on every water pool whose owner's position is inside the room.
void fn_7100a238f4(UnkFreezeRoom_ovb0106* self) {
    UnkFreezeList_ovb0106* list = fn_71009088a0(self);
    for (UnkFreezeHandle_ovb0106 **it = list->data, **end = list->data + list->count; it != end; ++it) {
        UnkFreezeHandle_ovb0106* handle = *it;
        if (!handle)
            continue;
        UnkReflObject* target = handle->target;
        if (!target || !fn_710009d50c(target->vf_0x0(), fn_7100906bcc()))
            continue;
        UnkFreezePool_ovb0106* pool = static_cast<UnkFreezePool_ovb0106*>(target);
        UnkFreezeOwner_ovb0106* owner = pool->owner;
        f32 x = owner->x;
        f32 y = owner->y;
        f32 z = owner->z;
        CVector3D pos;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        CVector2D point;
        fn_710007b430(&point, &pos);
        if (fn_7100a47e00(reinterpret_cast<UnkMagmaArea_b50*>(self), &point))
            pool->SetFrozen(self->frozen);
    }
}

struct UnkEnvironmentManager;
extern void* g_unk_7101d47368;
void fn_7100d8ced8(void*);
void fn_7100d8e32c(UnkEnvironmentManager*, const CStrId*, bool, bool);
const CStrId* fn_7100d8efc8(UnkEnvironmentManager*);
struct UnkLiquidPoolEnter_ovb0075;
struct UnkLiquidPoolActor_ovb0075;
void fn_7100a45f08(UnkLiquidPoolEnter_ovb0075*, UnkLiquidPoolActor_ovb0075*, const UnkVec3f*);
void fn_7100f0e98c(void*, void*, void*, void*);

// The environment manager, created on first use.
static inline UnkEnvironmentManager* EnvironmentManager() {
    if (!g_unk_7101d47368) {
        void* m = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(m);
        g_unk_7101d47368 = m;
    }
    return static_cast<UnkEnvironmentManager*>(g_unk_7101d47368);
}

struct UnkFreezeRoomOwner_ovb0106 {
    u8 pad_0x0[0x28];
    UnkReflObject* char_class;  // 0x28
    u8 field_0x30[8];
};

struct UnkFreezeRoomEnter_ovb0106 {
    u8 pad_0x0[0x30];
    UnkFreezeRoomOwner_ovb0106* owner;  // 0x30
    u8 pad_0x38[0x980];
    s32 field_0x9b8;
    u8 pad_0x9bc[0x27c];
    base::UnkStrIdMember environment;           // 0xc38
    base::UnkStrIdMember previous_environment;  // 0xc40
    bool sets_environment;                      // 0xc48
};
static_assert(__builtin_offsetof(UnkFreezeRoomEnter_ovb0106, sets_environment) == 0xc48, "layout");

struct UnkFreezeActor_ovb0106 {
    u8 pad_0x0[0x30];
    u8 field_0x30[8];
};

// The base handler first; then, when the room isn't frozen (+0x9b8 clear) and set to change the
// environment, switches the manager back to the kept environment while the room's is current; a
// tracked actor (fn_7100df5c38) is also handed to fn_7100f0e98c with the owner's character class.
void fn_7100a23fd0(UnkFreezeRoomEnter_ovb0106* self, UnkFreezeActor_ovb0106* actor, const UnkVec3f* pos) {
    fn_7100a45f08(reinterpret_cast<UnkLiquidPoolEnter_ovb0075*>(self),
                  reinterpret_cast<UnkLiquidPoolActor_ovb0075*>(actor), pos);
    if (self->field_0x9b8 == 0 && self->sets_environment &&
        self->previous_environment.data != &g_unk_7101d47108->empty) {
        if (fn_7100d8efc8(EnvironmentManager())->data == self->environment.data)
            fn_7100d8e32c(EnvironmentManager(), reinterpret_cast<const CStrId*>(&self->previous_environment),
                          false, false);
    }
    if (!fn_7100df5c38(g_unk_7101d47310, actor))
        return;
    u8* manager_0x38 = reinterpret_cast<u8*>(g_unk_7101d47310) + 0x38;
    UnkReflObject* c = self->owner->char_class;
    c = c && fn_710009d50c(c->vf_0x0(), fn_71011abbd4()) ? c : nullptr;
    fn_7100f0e98c(manager_0x38, reinterpret_cast<u8*>(c) + 0x98, self->owner->field_0x30, actor->field_0x30);
}
