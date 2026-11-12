// CSclawkAIComponent (the name from reflection, tools/tu_map.py): a message handler override. After
// the base AI component's handler (fn_710036974c), the owner's component at +0x280 is asked for the
// part with the hash 0x1aaebf634dea2cd1 (fn_7101152b90). When the message's flag at +8 is set, the
// part's vf_0x58 result is kept at +0x146ee and, when true, the part gets vf_0x40(false); when it
// is clear and the kept flag is set, the part gets vf_0x40(true). Names and layout are placeholders.
#include "game/CBehaviorTreeAIComponent.h"
#include "types.h"

struct UnkObject;
struct UnkComponent;
struct UnkPart;
struct UnkWorld;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);

struct UnkSclawkPart_b44a2 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(bool on);
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual bool vf_0x58();
};

struct UnkSclawkOwner_b44a2 {
    u8 pad_0x0[0x280];
    UnkObject* field_0x280;
};

struct UnkSclawkMessage_b44a2 {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

struct UnkSclawkAI_b44a2 {
    u8 pad_0x0[0x30];
    UnkSclawkOwner_b44a2* owner;
    u8 pad_0x38[0x146ee - 0x38];
    bool field_0x146ee;
};

u32 fn_710067d314(UnkSclawkAI_b44a2* self, UnkSclawkMessage_b44a2* msg) {
    u32 result = fn_710036974c(self, msg);
    if (self->owner && self->owner->field_0x280) {
        auto* part =
            reinterpret_cast<UnkSclawkPart_b44a2*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(self->owner->field_0x280),
                                                                 0x1aaebf634dea2cd1));
        if (part) {
            if (msg->field_0x8) {
                self->field_0x146ee = part->vf_0x58();
                if (self->field_0x146ee)
                    part->vf_0x40(false);
            } else if (self->field_0x146ee) {
                part->vf_0x40(true);
            }
        }
    }
    return result;
}
