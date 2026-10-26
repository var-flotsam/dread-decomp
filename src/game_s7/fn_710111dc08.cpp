// Reads or writes one of an object's named settings (mostly strings and flags). Unknown keys go
// to the base handler (fn_71010c4250). Layouts are placeholders.
#include "UnkPropS7.h"
#include "game/UnkComponent.h"

using namespace unk_prop;
using base::global::CRntString;

struct UnkObj_710111dc08 {
    u8 unk_0x0[0x13a];
    bool field_0x13a;
    bool field_0x13b;
    bool field_0x13c;
    u8 unk_0x13d[0x1];
    bool field_0x13e;
    u8 unk_0x13f[0x9];
    CRntString field_0x148;
    u8 unk_0x178[0x8];
    s32 field_0x180;
    u8 unk_0x184[0x8];
    s32 field_0x18c;
    s32 field_0x190;
    s32 field_0x194;
    u8 unk_0x198[0x30];
    CRntString field_0x1c8;
    CRntString field_0x1f8;
    CRntString field_0x228;
    CRntString field_0x258;
    CRntString field_0x288;
    CRntString field_0x2b8;
    CRntString field_0x2e8;
    CRntString field_0x318;
    CRntString field_0x348;
};


u32 fn_710111dc08(UnkObj_710111dc08* self, UnkProp* p) {
    if (Is(p, Key(0xef0)) && p->set)
        return true;
    if (Is(p, Key(0xdf8)) && p->set)
        return true;
    if (Is(p, Key(0x1dd8)) && p->set)
        return true;
    if (Is(p, Key(0x1a88)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0x258, buf);
        return true;
    }
    if (Is(p, Key(0x1ad8)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0x228, buf);
        return true;
    }
    if (Is(p, Key(0x1aa8)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0x1c8, buf);
        return true;
    }
    if (Is(p, Key(0x1a98)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0x1f8, buf);
        return true;
    }
    if (Is(p, Key(0x1af8)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0x288, buf);
        return true;
    }
    if (Is(p, Key(0x1af0)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0x2b8, buf);
        return true;
    }
    if (Is(p, Key(0x1ad0)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0x2e8, buf);
        return true;
    }
    if (Is(p, Key(0x1ac8)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0x318, buf);
        return true;
    }
    if (Is(p, Key(0x1a68)) && p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        fn_710007e77c(&self->field_0x348, buf);
        return true;
    }
    if (!p->set && Is(p, Key(0x1500)))
        return fn_710007cb4c(p->stream, "b", self->field_0x13e) != 0;
    if (!p->set && Is(p, Key(0x1508)))
        return fn_710007cb4c(p->stream, "b", self->field_0x13c) != 0;
    if (!p->set && Is(p, Key(0x1518)))
        return fn_710007cb4c(p->stream, "b", self->field_0x13a) != 0;
    if (p->set && Is(p, Key(0x1518)))
        return fn_710007cf30(p->stream, "b", &self->field_0x13a) != 0;
    if (p->set && Is(p, Key(0x1520)))
        return fn_710007cf30(p->stream, "b", &self->field_0x13b) != 0;
    if (!p->set && Is(p, Key(0x1758)))
        return fn_710007cb4c(p->stream, "i", self->field_0x18c) != 0;
    if (p->set && Is(p, Key(0x1758)))
        return fn_710007cf30(p->stream, "i", &self->field_0x18c) != 0;
    if (!p->set && Is(p, Key(0x19c0)))
        return fn_710007cb4c(p->stream, "i", self->field_0x194) != 0;
    if (p->set && Is(p, Key(0x19c0)))
        return fn_710007cf30(p->stream, "i", &self->field_0x194) != 0;
    if (p->set && Is(p, Key(0x1738)))
        return fn_710007cf30(p->stream, "i", &self->field_0x190) != 0;
    if (p->set && Is(p, Key(0x1180)))
        return fn_710007cf30(p->stream, "i", &self->field_0x180) != 0;
    if (Is(p, Key(0x1f88))) {
        const char* value;
        const char* unused;
        fn_710007cee4(p->stream, &value);
        fn_710007cee4(p->stream, &unused);
        fn_710007e77c(&self->field_0x148, value);
        return true;
    }
    // The base handler takes the component and the property record: the object's first 0x40
    // bytes are the component, UnkProp is the record.
    return fn_71010c4250(reinterpret_cast<UnkComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
