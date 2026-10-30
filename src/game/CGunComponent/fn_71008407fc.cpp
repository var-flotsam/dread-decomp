// In CGunComponent's unit (unit 1304 of tools/tu_map.py): the id (+0x20) of the gun's current
// object, the one at +0x118 while the flag at +0x128 is set and the one at +0x120 otherwise; the
// global empty id when there is none. Names and layouts are placeholders.
#include "base/global/CStrId.h"

struct UnkGunObject {
    u8 pad_0x0[0x20];
    base::global::CStrId id;
};

struct UnkGunObjectA : UnkGunObject {};
struct UnkGunObjectB : UnkGunObject {};

struct UnkGunComponent {
    u8 pad_0x0[0x118];
    UnkGunObjectA* field_0x118;
    UnkGunObjectB* field_0x120;
    bool field_0x128;
};

// NON_MATCHING(minor): the final select's operands are in other registers; same behaviour.
const base::global::CStrId* fn_71008407fc(UnkGunComponent* self) {
    UnkGunObject* object;
    if (!self->field_0x128) {
        object = self->field_0x120;
    } else {
        object = self->field_0x118;
        // Keeps the two loads apart (Clang would otherwise load through a select of the offsets).
        asm("" : "+r"(object));
    }
    return !object ? &g_unk_7101d470e0 : &object->id;
}
