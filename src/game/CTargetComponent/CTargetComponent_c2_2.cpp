// Reads one of an object's named settings from the property's stream (three keys). Other keys go
// to the base handler (fn_71010c4250). Layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CRntFile.h"
#include "game/UnkComponent.h"

using namespace unk_prop;
using base::global::CRntFile;
using base::global::CRntString;

// Reads a string from the stream into `buf`.
void fn_710007ca94(CRntFile* file, char* buf);

struct UnkObj_71011823a4 {
    u8 unk_0x0[0x40];
    u32 field_0x40;
    u32 field_0x44;
    u8 field_0x48;
    bool field_0x49;
    u8 unk_0x4a[0x6];
    CRntString field_0x50;
};

u32 fn_71011823a4(UnkObj_71011823a4* self, UnkProp* p) {
    if (Is(p, Key(0xc28)) && p->set) {
        CRntFile* file = reinterpret_cast<CRntFile*>(p->stream);
        char buf[0x200] = {};
        fn_710007c714(file, &self->field_0x40, 4, 1);
        fn_710007c714(file, &self->field_0x48, 1, 1);
        fn_710007ca94(file, buf);
        fn_710007e77c(&self->field_0x50, buf);
        return true;
    }
    if (Is(p, Key(0x21e0)) && p->set) {
        if (!self->field_0x49) {
            self->field_0x49 = true;
            self->field_0x44 = 0;
        }
        return true;
    }
    if (Is(p, Key(0x2230)) && p->set) {
        if (self->field_0x49)
            self->field_0x49 = false;
        return true;
    }
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
