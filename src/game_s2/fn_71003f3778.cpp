// CBigkranXSpitAttack's event handler: always reports the event handled. When it is a type-1
// event named like the first id of the unit initialiser (fn_71003f4220), and the attack's owner has
// the sub-object at +0x220, forwards a request to that.
#include "UnkSpitSend.h"
#include "base/global/CStrId.h"

using base::global::UnkStrIdBase;

extern UnkStrIdBase g_unk_71726c2888[2] __attribute__((visibility("hidden")));

struct UnkEventSource {
    u8 pad_0x0[0x10];
    UnkEventSource* field_0x10;
};

struct UnkSpitEvent {
    s32 field_0x0;
    u8 pad_0x4[4];
    UnkEventSource* field_0x8;
};

struct UnkSpitOwnerObject {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkSpitHandler {
    u8 pad_0x0[0x8];
    UnkSpitOwnerObject* field_0x8;
};

bool fn_71003f3778(UnkSpitHandler* self, UnkSpitEvent* event) {
    const UnkStrIdBase* name = &g_unk_7101d470e0;
    if (event->field_0x8) {
        UnkEventSource* source = event->field_0x8->field_0x10;
        name = source ? reinterpret_cast<const UnkStrIdBase*>(reinterpret_cast<u8*>(source) + 0x38) : name;
    }
    base::global::UnkStringData* id = name->data;
    if (id != &g_unk_7101d47108->empty && event->field_0x0 == 1 && id == g_unk_71726c2888[0].data) {
        if (self->field_0x8) {
            if (void* target = self->field_0x8->field_0x220)
                UnkSpitSend(target, 0x457b169dc49ad046, 0xf4061ff25a2163d6);
        }
    }
    return true;
}
