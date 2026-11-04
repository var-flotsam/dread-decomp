// CActorComponentDef's unit (tools/tu_map.py unit 343): reads two words from a file and keeps them at
// +0x18 and +0x1c unless they are -1 and -2, then reads the rest through the object's vf_0x38
// (fn_7100253af0). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "types.h"

struct UnkComponentDefRead_b67a1;

struct UnkComponentDefReadVtable_b67a1 {
    void* slots_0x0[7];
    bool (*vf_0x38)(UnkComponentDefRead_b67a1* self, base::global::CRntFile* file);
};

struct UnkComponentDefRead_b67a1 {
    UnkComponentDefReadVtable_b67a1* vtable;
    u8 pad_0x8[0x10];
    s32 field_0x18;
    s32 field_0x1c;
};

bool fn_7100253af0(UnkComponentDefRead_b67a1* self, base::global::CRntFile* file) {
    s32 first = 0;
    s32 second = -1;
    fn_710007c714(file, &first, 4, 1);
    fn_710007c714(file, &second, 4, 1);
    if (first != -1)
        self->field_0x18 = first;
    if (second != -2)
        self->field_0x1c = second;
    return self->vtable->vf_0x38(self, file);
}
