// CXParasiteAIComponent unit (tools/tu_map.py): when the flag at +0x146ec is set and the chain
// +0x146e0+0xb8 -> +0x10 leads to an object, its +0x240 entry (if any) is handed, offset by 0x15c4, to
// vtable slot 0xd0 of the object at +0x30 with `true`; otherwise state 7 (+0x146e0) runs fn_71006e77b8.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkTU1102.h"
#include "types.h"

struct UnkTarget_xpa {
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
    virtual void vf_0xd0(void* entry, bool flag);
};

struct UnkInner_xpa {
    u8 pad_0x0[0x240];
    u8* entry;  // 0x240
};

struct UnkLink_xpa {
    u8 pad_0x0[0x10];
    UnkInner_xpa* inner;  // 0x10
};

struct UnkXParasite_xpa {
    u8 pad_0x0[0x30];
    UnkTarget_xpa* target;  // 0x30
    u8 pad_0x38[0x146e0 - 0x38];
    u32 state;  // 0x146e0
    u8 pad_0x146e4[8];
    bool flag;  // 0x146ec
    u8 pad_0x146ed[0x146e0 + 0xb8 - 0x146ed];
    UnkLink_xpa* link;  // 0x146e0 + 0xb8
};

void fn_71006e77b8(UnkXParasite_xpa* self);

void fn_71006e7754(UnkXParasite_xpa* self) {
    if (self->flag && self->link && self->link->inner) {
        u8* entry = self->link->inner->entry;
        if (entry)
            self->target->vf_0xd0(entry + 0x15c4, true);
        return;
    }
    if (self->state == 7)
        fn_71006e77b8(self);
}

// Whether the parasite may act: reads the blackboard flag (into +0x146eb) first; false while that flag
// is set or the link chain leads to an object, else unless the state is 2 or 8.
struct UnkXParasiteBB_xpa {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x146e0 - 0x1350];
    u32 state;  // 0x146e0
    u8 pad_0x146e4[7];
    bool flag;  // 0x146eb
    u8 pad_0x146ec[0x146e0 + 0xb8 - 0x146ec];
    UnkLink_xpa* link;  // 0x146e0 + 0xb8
};

extern base::global::CStrId g_unk_71726b7cb8;  // the blackboard section
extern base::global::CStrId g_unk_717270b8e0 __attribute__((visibility("hidden")));  // the property in it

bool fn_71006eb590(UnkXParasiteBB_xpa* self) {
    if (CBlackboard::CSection* section = fn_7101047270(&self->blackboard, g_unk_71726b7cb8))
        fn_710006d11c(section, &g_unk_717270b8e0, &self->flag);
    bool busy = self->flag || (self->link && self->link->inner);
    return !busy && self->state != 2 && self->state != 8;
}

// Ending the grab: when the link at +0x14790 leads to an object, its partner's (+0x398, +0x10) value at
// +0x2c0 is copied to +0x2bc, the object's component at +0x240 is told (vtable slot 0x410), the link is
// dropped and the flag at +0x147b9 set; without one, fn_71006e892c runs instead.
struct UnkGrabPartner_xpa {
    u8 pad_0x0[0x2bc];
    u32 field_0x2bc;
    u32 field_0x2c0;
};

struct UnkGrabPartnerLink_xpa {
    u8 pad_0x0[0x10];
    UnkGrabPartner_xpa* partner;  // 0x10
};

struct UnkGrabListener_xpa {
    void** vtable;
    // Vtable slot 0x410.
    void Notify(void* self) { reinterpret_cast<void (*)(UnkGrabListener_xpa*, void*)>(vtable[0x410 / 8])(this, self); }
};

struct UnkGrabObject_xpa {
    u8 pad_0x0[0x240];
    UnkGrabListener_xpa* listener;  // 0x240
    u8 pad_0x248[0x150];
    UnkGrabPartnerLink_xpa* partner_link;  // 0x398
};

struct UnkGrabLink_xpa {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;  // 0x8
    UnkGrabObject_xpa* object;  // 0x10
};

struct UnkXParasiteGrab_xpa {
    u8 pad_0x0[0x14790];
    UnkGrabLink_xpa* link;  // 0x14790
    u8 pad_0x14798[0x21];
    bool released;  // 0x147b9
};

void fn_71006ea160(UnkXParasiteGrab_xpa* self) {
    UnkGrabLink_xpa* link = self->link;
    if (!link || !link->object) {
        fn_71006e892c(self);
        return;
    }
    UnkGrabObject_xpa* object = link->object;
    if (object->partner_link && object->partner_link->partner) {
        UnkGrabPartner_xpa* partner = object->partner_link->partner;
        // Stored through a may_alias type: the original reloads the link afterwards.
        typedef u32 __attribute__((may_alias)) AliasU32;
        *reinterpret_cast<AliasU32*>(&partner->field_0x2bc) = partner->field_0x2c0;
        object = self->link->object;
    }
    if (object->listener)
        object->listener->Notify(self);
    if (UnkGrabLink_xpa* l = self->link) {
        if (--l->ref_count == 0)
            l->Release();
        self->link = nullptr;
    }
    self->released = true;
}
