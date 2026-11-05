// CFrozenAsFrostbiteComponent's unit: a hit message from the frozen owner itself (the sender's object at
// +0x10 of +8 is the owner at +0x30) breaks the ice (state 3, fn_71007f9428) when the owner's component
// at +0x240 allows it (vtable slot 0x4b8), the component is frozen (fn_71007f9f20) and neither
// fn_71007f9fb0 nor fn_71007f9fd0 holds. Always answers false. Names are placeholders.
#include "types.h"

struct UnkFrostbiteComponent;
bool fn_71007f9f20(void*);
bool fn_71007f9fb0(UnkFrostbiteComponent*);
bool fn_71007f9fd0(UnkFrostbiteComponent*);
void fn_71007f9428(UnkFrostbiteComponent* self, u32 state);

struct UnkFrostbiteGate_A5_b65 {
    virtual void vf_0x0();
};

struct UnkFrostbiteOwner_A5_b65 {
    u8 pad_0x0[0x240];
    UnkFrostbiteGate_A5_b65* gate;
};

struct UnkFrostbiteSenderRef_A5_b65 {
    u8 pad_0x0[0x10];
    UnkFrostbiteOwner_A5_b65* object;
};

struct UnkFrostbiteMessage_A5_b65 {
    u64 field_0x0;
    UnkFrostbiteSenderRef_A5_b65* sender;
};

struct UnkFrostbite_A5_b65 {
    u8 pad_0x0[0x30];
    UnkFrostbiteOwner_A5_b65* owner;
};

bool fn_71007f5d24(UnkFrostbite_A5_b65* self, const UnkFrostbiteMessage_A5_b65* msg) {
    UnkFrostbiteOwner_A5_b65* owner = self->owner;
    UnkFrostbiteOwner_A5_b65* from = msg->sender ? msg->sender->object : nullptr;
    if (from != owner)
        return false;
    if (UnkFrostbiteGate_A5_b65* gate = owner->gate) {
        typedef bool (*Fn)(UnkFrostbiteGate_A5_b65*);
        if (!(*reinterpret_cast<Fn**>(gate))[0x4b8 / 8](gate))
            return false;
    }
    UnkFrostbiteComponent* c = reinterpret_cast<UnkFrostbiteComponent*>(self);
    if (!fn_71007f9f20(self) || fn_71007f9fb0(c) || fn_71007f9fd0(c))
        return false;
    fn_71007f9428(c, 3);
    return false;
}
