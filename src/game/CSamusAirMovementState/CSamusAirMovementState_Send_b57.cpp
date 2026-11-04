// CSamusAirMovementState unit (tools/tu_map.py): for an event of kind 1 whose object's (+0x8) part at +0x10
// has a zero +0x14, sends the owner's (+0x30) sub-object at +0x220 the id hash of the singleton's
// (fn_7100031894) part at +0xfd0 with mode 0 (UnkSpitSend, src/game_s2/UnkSpitSend.h). Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

struct UnkAirEventPart_b57 {
    u8 pad_0x0[0x14];
    s32 field_0x14;
};

struct UnkAirEventObj_b57 {
    u8 pad_0x0[0x10];
    UnkAirEventPart_b57* field_0x10;
};

struct UnkAirEvent_b57 {
    s32 kind;
    UnkAirEventObj_b57* obj;
};

struct UnkAirSendId_b57 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAirSendOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkAirSend_b57 {
    u8 pad_0x0[0x30];
    UnkAirSendOwner_b57* owner;
};

void fn_710098ce94(UnkAirSend_b57* self, UnkAirEvent_b57* event) {
    if (event->obj && event->kind == 1 && event->obj->field_0x10->field_0x14 == 0) {
        u64 hash = (*reinterpret_cast<UnkAirSendId_b57**>(
                        reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0xfd0))
                       ->hash;
        UnkSpitSend(self->owner->field_0x220, hash, 0xf4061ff25a2163d6);
    }
}
