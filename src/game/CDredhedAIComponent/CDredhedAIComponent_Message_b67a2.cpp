// CDredhedAIComponent unit (tools/tu_map.py, the name from reflection): a message handler override. After the
// base AI component's handler (fn_710036974c), whose result it returns: with the message's flag (+8) set and the
// owner's (+0x30) +0x220 object and +0x278 object present, and fn_7101136308 accepting the latter, calls
// fn_7101176be4 on the former. Names and layouts are placeholders.
#include "types.h"

// As src/game/CBigkranXAIComponent/CBigkranXAIComponent_Message_b61.cpp declares them.
struct UnkSclawkAI_b44a2;
struct UnkSclawkMessage_b44a2;
u32 fn_710036974c(UnkSclawkAI_b44a2* self, UnkSclawkMessage_b44a2* msg);
bool fn_7101136308(void* object);
struct UnkDredhedTarget_b67a2;
void fn_7101176be4(UnkDredhedTarget_b67a2* target);

struct UnkDredhedOwner_b67a2 {
    u8 pad_0x0[0x220];
    UnkDredhedTarget_b67a2* field_0x220;
    u8 pad_0x228[0x278 - 0x228];
    void* field_0x278;
};

struct UnkDredhedMessage_b67a2 {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

struct UnkDredhedAI_b67a2 {
    u8 pad_0x0[0x30];
    UnkDredhedOwner_b67a2* owner;

    UnkDredhedOwner_b67a2* GetOwner() { return owner; }
};

u32 fn_71004e7ebc(UnkDredhedAI_b67a2* self, UnkDredhedMessage_b67a2* msg) {
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self), reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    UnkDredhedOwner_b67a2* owner = self->GetOwner();
    if (owner != nullptr) {
        asm("" : "+r"(owner));
        if (owner != nullptr && msg->field_0x8) {
            UnkDredhedTarget_b67a2* target = owner->field_0x220;
            if (target != nullptr && owner->field_0x278 != nullptr && fn_7101136308(owner->field_0x278))
                fn_7101176be4(target);
        }
    }
    return result;
}
