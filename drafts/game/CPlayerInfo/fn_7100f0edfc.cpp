// CPlayerInfo unit: a reflected function that reads a name from the argument's stream, counts a use of
// each slot (0x38 bytes) of the object up to the one whose key matches the name's, makes that slot the
// Input tunable's current one, and returns the int 0 as a reflected value. Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkSlotClass_lf0edfc {
    u8 pad_0x0[0x30];
    s64 key;
    u8 pad_0x38[0xc];
    s32 uses;
};

struct __attribute__((packed)) UnkSlot_lf0edfc {
    u8 pad_0x0[8];
    UnkSlotClass_lf0edfc* cls;
    u8 pad_0x10[0x28];
};

struct __attribute__((packed)) UnkSlotOwner_lf0edfc {
    u8 pad_0x0[0x10];
    u32 count;
    UnkSlot_lf0edfc slots[1];
};

struct UnkInputTunableView_lf0edfc {
    u8 pad_0x0[0x40];
    s32 slot;
};

extern UnkTunable g_unk_71727298b0;

UnkReflValue fn_7100f0edfc(char* obj, UnkReflValue* const* args) {
    UnkSlotOwner_lf0edfc* owner = *reinterpret_cast<UnkSlotOwner_lf0edfc**>(obj + 8);
    UnkReflValue* arg = args[0];
    void* stream;
    if (!arg->type)
        stream = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        stream = arg->field_0x0;
    else
        stream = arg;
    {
        char name[0x100];
        fn_710007cf30(stream, "s", name);
        base::UnkNamedEntry entry;
        fn_710007e704(&entry, name);
        s64 key = reinterpret_cast<base::global::CRntString*>(&entry)->key;
        UnkSlot_lf0edfc* it = owner->slots;
        u32 i = 0;
        if (u64(owner->count) * sizeof(UnkSlot_lf0edfc)) {
            do {
                ++it->cls->uses;
                if (it->cls->key == key) {
                    reinterpret_cast<UnkInputTunableView_lf0edfc*>(&g_unk_71727298b0)->slot = i;
                    break;
                }
                ++it;
                ++i;
            } while (it != owner->slots + owner->count);
        }
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&entry));
    }
    s32 zero = 0;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &zero, false);
    return r;
}
