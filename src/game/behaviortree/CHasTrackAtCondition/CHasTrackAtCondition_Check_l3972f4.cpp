// behaviortree::CHasTrackAtCondition (tools/tu_map.py unit 503): true when the type registered under the
// condition's track id (+0x30 of its data at +0x38) is a class and the owner's character class (+0x220)
// has a track of it. Names and layouts are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU83.h"
#include "types.h"

// A registered type, read through its first virtual function (its own type).
struct UnkTrackType_l3972f4 {
    void** vtable;
    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkTrackType_l3972f4*)>(vtable[0])(this);
    }
};

struct UnkTrackData_l3972f4 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkHasTrackAtCondition_l3972f4 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x18];
    UnkTrackData_l3972f4* data;  // 0x38
};

bool CHasTrackAtCondition_Check(UnkHasTrackAtCondition_l3972f4* self) {
    if (UnkActor* owner = self->owner) {
        if (auto* animation = static_cast<UnkAnimationComponent*>(owner->field_0x220)) {
            UnkVersionRegistry* registry = g_unk_7101d4c8d0;
            u64 id = self->data->id;
            auto* type = reinterpret_cast<UnkTrackType_l3972f4*>(fn_71000a5118(registry, &id));
            if (type && fn_710009d50c(type->vf_0x0(), fn_7100096100()) &&
                fn_710117b348(animation, reinterpret_cast<const base::reflection::CClass*>(type)))
                return true;
        }
    }
    return false;
}
