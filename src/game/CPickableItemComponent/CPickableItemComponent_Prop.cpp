// CPickableItemComponent's unit (tools/tu_map.py): the property handler. Writes the item's
// string id (+0x98) as text and a float (+0xa8), reads a float into +0x50; anything else goes to
// the base handler (fn_71009cd1b4). Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace unk_prop;
using base::global::CRntFile;
using base::global::CRntString;
using base::global::UnkStringData;

// A string id's data seen with its text (+0x10) and the use count bumped when it is read (+0x44).
struct UnkNamedData_9cf8f8 {
    u8 unk_0x0[0x10];
    CRntString text;
    u8 unk_0x40[0x4];
    union {
        u32 use_count;
    };
};

struct UnkPickableItem_9cf8f8 {
    u8 unk_0x0[0x50];
    f32 field_0x50;
    u8 unk_0x54[0x44];
    UnkStringData* name;  // a CStrId's data
    u8 unk_0xa0[0x8];
    f32 field_0xa8;

    // The id's text, counting a use (the count is not given back).
    const CRntString& NameText() const {
        if (name == &g_unk_7101d47108->empty)
            return g_unk_7101d58d10;
        UnkNamedData_9cf8f8* d = reinterpret_cast<UnkNamedData_9cf8f8*>(name);
        ++d->use_count;
        return reinterpret_cast<UnkNamedData_9cf8f8*>(name)->text;
    }
};

u32 fn_71009cd1b4(UnkPickableItem_9cf8f8* self, UnkProp* p);

u32 fn_71009cf8f8(UnkPickableItem_9cf8f8* self, UnkProp* p) {
    if (Is(p, Key(0x1198)) && !p->set) {
        fn_710007c00c(reinterpret_cast<CRntFile*>(p->stream), 0, 0);
        const char* text = self->NameText().data;
        return fn_710007cb4c(p->stream, "s", text ? text : "") != 0;
    }
    if (Is(p, Key(0x1190)) && !p->set) {
        fn_710007c00c(reinterpret_cast<CRntFile*>(p->stream), 0, 0);
        return fn_710007cb4c(p->stream, "f", (f64)self->field_0xa8) != 0;
    }
    if (p->set && Is(p, Key(0x1658)))
        return fn_710007cf30(p->stream, "f", &self->field_0x50) != 0;
    return fn_71009cd1b4(self, p);
}
