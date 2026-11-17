// CAutsniperAIComponent's unit: a message handler. Runs the behavior-tree base's (fn_710036974c), then sends
// the owner's character class (+0x220) one of two tracks with mode 0 by the message's flag (+0x8) and, when
// set, calls slot 0x178 of the owner's +0x278 object. Returns the base's answer. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "types.h"

struct UnkAutsniperPart_l3daf00 {
    void** vtable;

    void vf_0x178() { reinterpret_cast<void (*)(UnkAutsniperPart_l3daf00*)>(vtable[0x178 / 8])(this); }
};

struct UnkAutsniperOwner_l3daf00 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x278 - 0x228];
    UnkAutsniperPart_l3daf00* field_0x278;
};

struct UnkAutsniperAI_l3daf00 {
    u8 pad_0x0[0x30];
    UnkAutsniperOwner_l3daf00* owner;
};

struct UnkAutsniperMessage_l3daf00 {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

u32 fn_71003daf00(UnkAutsniperAI_l3daf00* self, UnkAutsniperMessage_l3daf00* msg) {
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self),
                               reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    void* target = self->owner->field_0x220;
    switch (static_cast<int>(msg->field_0x8)) {
    case 1:
        base::reflection::UnkReflTypeIdArg mode_a;
        base::reflection::UnkReflTypeIdArg mask_a;
        mode_a.id = 0;
        mask_a.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0x67ec2589f81c5c09, 0xf4061ff25a2163d6, &mode_a, &g_unk_710164b958,
                      &g_unk_7172769740, &mask_a);
        self->owner->field_0x278->vf_0x178();
        break;
    default:
        base::reflection::UnkReflTypeIdArg mode_b;
        base::reflection::UnkReflTypeIdArg mask_b;
        mode_b.id = 0;
        mask_b.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0x2e2502a76ba1a042, 0xf4061ff25a2163d6, &mode_b, &g_unk_710164b958,
                      &g_unk_7172769740, &mask_b);
        break;
    }
    return result;
}
