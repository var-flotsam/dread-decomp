// Returns whether any listed object's class name contains the given class's name (optionally skipping objects that report busy).
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkClsStr_710117af10 {
    const char* str;
};

struct UnkCls_710117af10 {
    u8 unk_0x0[0x10];
    UnkClsStr_710117af10 name;
    u8 unk_0x18[0x2c];
    union {
        s32 field_0x44;
    };
};

struct UnkObj_710117af10;

struct UnkElem_710117af10 {
    u8 unk_0x0[0x70];
    UnkObj_710117af10* field_0x70;
};

struct UnkList_710117af10 {
    u8 unk_0x0[0xf0];
    UnkElem_710117af10** items;
    u8 unk_0xf8[0x8];
    u32 count;
};

struct UnkStruct_710117af10 {
    u8 unk_0x0[0x40];
    UnkList_710117af10* field_0x40;
};

UnkCls_710117af10** fn_710070d7f8(UnkObj_710117af10* obj);
bool fn_710070d830(UnkObj_710117af10* obj);

bool fn_710117af10(UnkStruct_710117af10* self, UnkCls_710117af10** name, bool skipBusy) {
    for (UnkElem_710117af10** it = self->field_0x40->items;
         it != self->field_0x40->items + self->field_0x40->count; ++it) {
        UnkObj_710117af10* obj = (*it)->field_0x70;
        UnkCls_710117af10** cls = fn_710070d7f8(obj);
        (*cls)->field_0x44++;
        UnkCls_710117af10* c = *cls;
        if (c == reinterpret_cast<UnkCls_710117af10*>(&g_unk_7101d47108->empty))
            continue;
        const UnkClsStr_710117af10* str = &c->name;
        (*name)->field_0x44++;
        const char* n = (*name)->name.str ? (*name)->name.str : "";
        if (!fn_710007fb9c(reinterpret_cast<const base::global::CRntString*>(str), n))
            continue;
        if (skipBusy && fn_710070d830(obj))
            continue;
        return true;
    }
    return false;
}
