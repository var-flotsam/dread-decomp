// CAutoolAIComponent's unit (tools/tu_map.py unit 570): after the base step, when the owner has a partner
// with a smart object, places the owner at the partner's first link target (its translation, then its
// angles) and records the smart object's value. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflStatics.h"
#include "base/global/CRntVector.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU341.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_71003695b4(void* self);

struct UnkRefl_fn_71003c4a7c;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_71003c4a7c>(void*);

// The object a resolved link points at: its transform at +0x38.
struct UnkAutoolLinkTarget_l3c5d74 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

// The resolved link's handle: its target at +0x10.
struct UnkAutoolLinkHandle_l3c5d74 {
    u8 pad_0x0[0x10];
    UnkAutoolLinkTarget_l3c5d74* target;  // 0x10
};

struct UnkAutoolPartner_l3c5d74 {
    u8 pad_0x0[0x30];
    void* entity;  // 0x30
    u8 pad_0x38[0x358 - 0x38];
    base::global::CRntVector<UnkCallbackSet_7101a20950> links;  // 0x358
};

struct UnkAutoolPartnerLink_l3c5d74 {
    u8 pad_0x0[0x10];
    UnkAutoolPartner_l3c5d74* partner;  // 0x10
};

struct UnkAutoolOwner_l3c5d74 {
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
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void SetPosition(const UnkVec3f& position, bool notify);  // 0xd0
    virtual void SetAngle(const UnkVec3f& angle, bool notify);        // 0xd8
    u8 pad_0x8[0x220 - 0x8];
    void* field_0x220;
    u8 pad_0x228[0x398 - 0x228];
    UnkAutoolPartnerLink_l3c5d74* partner_link;  // 0x398
};

struct UnkAutoolSmartObject_l3c5d74 {
    u8 pad_0x0[0x48];
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u8 pad_0x54[4];
    bool field_0x58;
};

struct UnkAutoolAI_l3c5d74 {
    u8 pad_0x0[0x30];
    UnkAutoolOwner_l3c5d74* owner;  // 0x30
    u8 pad_0x38[0x14770 - 0x38];
    bool field_0x14770;
    bool field_0x14771;
};

// Resolves the link once (while its state is 1) and returns its handle.
static inline UnkAutoolLinkHandle_l3c5d74* AutoolLinkHandle_l3c5d74(UnkCallbackSet_7101a20950& link) {
    if (link.field_0x30 == 1) {
        {
            UnkRefPtr ref = fn_710023b580(&link.callbacks);
            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&link.owner),
                          reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.field_0x30 = 2;
    }
    return reinterpret_cast<UnkAutoolLinkHandle_l3c5d74*>(link.owner);
}

static inline UnkAutoolLinkTarget_l3c5d74* AutoolLinkTarget_l3c5d74(UnkCallbackSet_7101a20950& link) {
    UnkAutoolLinkHandle_l3c5d74* handle = AutoolLinkHandle_l3c5d74(link);
    return handle ? handle->target : nullptr;
}

// The owner's partner, while the owner has a character class.
static inline UnkAutoolPartner_l3c5d74* AutoolPartner_l3c5d74(UnkAutoolOwner_l3c5d74& owner) {
    UnkAutoolPartnerLink_l3c5d74* partner_link = owner.partner_link;
    if (!partner_link)
        return nullptr;
    UnkAutoolPartner_l3c5d74* partner = partner_link->partner;
    if ((partner == nullptr) | (owner.field_0x220 == nullptr))
        return nullptr;
    return partner;
}

void fn_71003c5d74(UnkAutoolAI_l3c5d74* self) {
    fn_71003695b4(self);
    UnkAutoolPartner_l3c5d74* partner = AutoolPartner_l3c5d74(*self->owner);
    if (!partner)
        return;
    UnkAutoolSmartObject_l3c5d74* smart = reinterpret_cast<UnkAutoolSmartObject_l3c5d74*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_71003c4a7c>(partner->entity));
    if (!smart)
        return;
    self->field_0x14771 = true;
    if (!smart->field_0x58 || partner->links.count == 0)
        return;

    UnkCallbackSet_7101a20950 link(partner->links.data[0]);
    self->owner->SetPosition(AutoolLinkHandle_l3c5d74(link)->target->xform.GetTranslation(), true);
    self->owner->SetAngle(AutoolLinkTarget_l3c5d74(link)->xform.GetAngle(), true);
    self->field_0x14770 = true;
    self->field_0x14771 = false;
    smart->field_0x50 = smart->field_0x48;
}
