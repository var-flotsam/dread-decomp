// CAIGrapplePointComponent unit (tools/tu_map.py). fn_7100827978: the shape of fn_71008261e0
// (CActionSwitcherOnPullGrapplePointComponent_b50.cpp): after the base test (fn_710082cb24), the
// collision object its definition (+0x90) names (+0x78) in the owner's (+0x30) collision container
// (+0x2a0) must be enabled (+0x6a) with flag 0x80 of its vf_0x58 set, and the pull must come from
// that grapple point. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

using base::global::CStrId;

struct UnkGrappleObject_b50 {
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
    virtual u32 vf_0x58();
    u8 pad_0x8[0x62];
    bool enabled;  // 0x6a
};

struct UnkAIPullSource_b50 {
    u8 pad_0x0[0x20];
    void* field_0x20;
};

struct UnkAIPullSender_b50 {
    u8 pad_0x0[0x10];
    UnkAIPullSource_b50* field_0x10;
};

struct UnkAIPullMessage_b50 {
    UnkAIPullSender_b50* sender;
};

struct UnkAIPullDef_b50 {
    u8 pad_0x0[0x78];
    CStrId field_0x78;
};

struct UnkAIPullOwner_b50 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkAIPull_b50 {
    u8 pad_0x0[0x30];
    UnkAIPullOwner_b50* owner;
    u8 pad_0x38[0x58];
    UnkAIPullDef_b50* def;  // 0x90
};

// The base test (both pull components share it).
bool fn_710082cb24(void* self, void* arg, void* msg);

bool fn_7100827978(UnkAIPull_b50* self, void* arg, UnkAIPullMessage_b50* msg) {
    if (!fn_710082cb24(self, arg, msg))
        return false;
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return false;
    UnkGrappleObject_b50* object =
        reinterpret_cast<UnkGrappleObject_b50*>(fn_71008640a4(collision, &self->def->field_0x78));
    if (!object)
        return false;
    if (!object->enabled || !(object->vf_0x58() & 0x80))
        return false;
    UnkAIPullSource_b50* source = msg->sender->field_0x10;
    return source->field_0x20 == *reinterpret_cast<void**>(&self->def->field_0x78);
}
