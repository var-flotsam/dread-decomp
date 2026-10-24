// Updates the flag byte at 0x1a of `other`: a clear flag stays clear, and it is cleared while the
// owner's byte at 0x14b54 is set. Otherwise it becomes the inverse of a flag that the object the global
// manager's entry at +0x968 names by the string table's id at 0x1150 (the table is a function-local
// static) updates; the named id at 0x717275ac18+0x10 is set to the default id on first use.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkOwner {
    u8 pad_0x0[0x14b54];
    u8 field_0x14b54;
};

struct UnkOther {
    u8 pad_0x0[0x1a];
    u8 field_0x1a;
};

struct UnkNamedId {
    u8 pad_0x0[0x10];
    CStrId field_0x10;
};

struct UnkTarget;

extern UnkNamedId g_unk_7172759178;
void fn_7100ae9a10(CStrId* id, UnkNamedId* named, const CStrId* default_id);
UnkTarget* fn_7101047270(void* entry, const CStrId* id);
void fn_710006d11c(UnkTarget* target, const CStrId* id, u8* flag);

void fn_71003e7bf8(UnkOwner* self, UnkOther* other) {
    u32 value = other->field_0x1a;
    if (value) {
        if (self->field_0x14b54) {
            value = 0;
        } else {
            u8 flag = 0;
            void* entry = g_unk_7101d47310->field_0x968;
            const CStrId& id = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x22a];
            if (g_unk_7172759178.field_0x10.data == &g_unk_7101d47108->empty)
                fn_7100ae9a10(&g_unk_7172759178.field_0x10, &g_unk_7172759178, &g_unk_7101d470e0);
            if (UnkTarget* target = fn_7101047270(entry, &id))
                fn_710006d11c(target, &g_unk_7172759178.field_0x10, &flag);
            value = flag ^ 1;
        }
    }
    other->field_0x1a = value;
}
