// Reads or writes one of an object's named settings. Unknown keys go to the base handler
// (fn_71010c4250). Layouts are placeholders.
#include "UnkPropS7.h"

using namespace unk_prop;
using base::global::CRntString;

struct UnkObj_7101129240 {
    u8 unk_0x0[0x48];
    CStrId field_0x48;
    u8 unk_0x50[0x50];
    // UnkStrId: the temporary moved into it folds away only with UnkStrId's destructor.
    base::global::UnkStrId field_0xa0;
    CRntString field_0xa8;
    CRntString field_0xd8;
    bool field_0x108;
    u8 unk_0x109[0x7];
    s32 field_0x110;
    u8 unk_0x114[0x274];
    s32 field_0x388;
};

void fn_7101129b08(UnkObj_7101129240* self, bool flag);
u32 fn_71010c4250(UnkObj_7101129240* self, UnkProp* p);

u32 fn_7101129240(UnkObj_7101129240* self, UnkProp* p) {
    if (!p->set && Is(p, Key(0x1758)))
        return fn_710007cb4c(p->stream, "i", self->field_0x110) != 0;
    if (p->set && Is(p, Key(0x1758)))
        return fn_710007cf30(p->stream, "i", &self->field_0x110) != 0;
    if (!p->set && Is(p, Key(0x19c0)))
        return fn_710007cb4c(p->stream, "i", self->field_0x388) != 0;
    if (p->set && Is(p, Key(0x19c0)))
        return fn_710007cf30(p->stream, "i", &self->field_0x388) != 0;
    if (Is(p, Key(0xaa8)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        CStrId id(buf, true);
        self->field_0x48 = id;
        return true;
    }
    if (Is(p, Key(0x21c8)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        self->field_0xa0 = base::global::UnkStrId(buf, true);
        return true;
    }
    if (Is(p, Key(0x2148)) && p->set) {
        bool value;
        fn_710007cf30(p->stream, "b", &value);
        self->field_0x108 = value;
        return true;
    }
    if (Is(p, Key(0x1a80)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0xa8, buf);
        return true;
    }
    if (Is(p, Key(0x1ad8)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0xd8, buf);
        return true;
    }
    if (Is(p, Key(0x1178)) && p->set) {
        fn_7101129b08(self, false);
        return true;
    }
    return fn_71010c4250(self, p);
}
