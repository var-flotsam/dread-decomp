// behaviortree::CAutoolGoToRepairPointAction's unit (tools/tu_map.py): when the owner's (+0x18) component at
// +0x240 is a CAutoolAIComponent and its sub-object at +0x220 exists, sends that sub-object the hash at
// +0x3a8 (UnkSpitSend), re-links the listener at +0x380 to the sub-object's weak handle, reads the
// "v3WaypointPos" value (fn_710039525c) and hands it to the component (fn_71003c6a5c). Names and layouts
// are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkPointB56 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkAutoolB56;
bool fn_71003c6a5c(UnkAutoolB56* self, const UnkPointB56& pos);

void fn_71003c29e8();

struct UnkAutoolOwner_l3c1e04 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
    u8 pad_0x228[0x18];
    UnkReflObject* field_0x240;
};

struct UnkAutoolListener_l3c1e04 {
    void** vtable;
    void* object;  // 0x8
    void (*callback)();  // 0x10
    UnkWeakHandle* handle;  // 0x18
};

struct UnkAutoolGoToRepairPoint_l3c1e04 {
    u8 pad_0x0[0x18];
    UnkAutoolOwner_l3c1e04* owner;
    u8 pad_0x20[0x380 - 0x20];
    UnkAutoolListener_l3c1e04 listener;  // 0x380
    u8 pad_0x3a0[0x8];
    u64 field_0x3a8;
};
static_assert(__builtin_offsetof(UnkAutoolGoToRepairPoint_l3c1e04, field_0x3a8) == 0x3a8, "layout");

void fn_71003c1e04(UnkAutoolGoToRepairPoint_l3c1e04* self) {
    UnkReflObject* component = self->owner->field_0x240;
    bool isAutool = component ? fn_710009d50c(component->vf_0x0(), fn_71003c2a94()) : false;
    UnkAutoolB56* ai = isAutool ? reinterpret_cast<UnkAutoolB56*>(component) : nullptr;
    if (!isAutool)
        return;
    UnkWeakTarget* target = self->owner->field_0x220;
    if (!target)
        return;

    UnkPointB56 pos;  // set below, before it is read
    UnkSpitSend(target, self->field_0x3a8, 0xf4061ff25a2163d6);

    UnkAutoolListener_l3c1e04& listener = self->listener;
    listener.object = self;
    listener.callback = fn_71003c29e8;
    if (UnkWeakHandle* handle = listener.handle) {
        if (UnkWeakTarget* old = handle->target)
            fn_710117b3f0(old, &listener);
        ResetRef(&listener.handle);
    }
    fn_7100316018(target, &listener.handle);
    fn_710117b3e8(listener.handle ? listener.handle->target : nullptr, &listener);

    pos = {0.0f, 0.0f, 0.0f};
    {
        CStrId key("v3WaypointPos", true);
        fn_710039525c(reinterpret_cast<UnkSelf*>(self), &key, reinterpret_cast<base::math::CVector3D*>(&pos));
    }
    fn_71003c6a5c(ai, pos);
}
