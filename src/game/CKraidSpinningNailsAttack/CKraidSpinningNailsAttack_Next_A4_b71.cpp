// CKraidSpinningNailsAttack's unit (tools/tu_map.py unit 918): takes the next entry of the current group
// (+0x50) of the definition's group arrays (+0x60; +0xf0 or +0x118 by the Kraid component's phase at
// +0x1140), limited to two unless the component's +0x1170 is set: stores the entry's pointer at +0x58 and
// advances the counter at +0x54, or clears +0x58 when done. Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

// The out-of-line accessor of the groups' vectors (src/clones/clones_02.cpp).
struct UnkClone_fn_71005edc34;
using UnkKraidNailElem_A4_b71 = UnkCloneOwner<UnkClone_fn_71005edc34>;
extern template UnkKraidNailElem_A4_b71& base::UnkVector<UnkKraidNailElem_A4_b71>::GrowAt(u32);

struct UnkKraidComp_A4_b71 {
    void** vtable;
    u8 pad_0x8[0x1140 - 0x8];
    u32 phase;  // 0x1140
    u8 pad_0x1144[0x1170 - 0x1144];
    u32 field_0x1170;
};

struct UnkKraidOwner_A4_b71 {
    u8 pad_0x0[0x240];
    UnkKraidComp_A4_b71* ai;  // 0x240
};

struct UnkKraidNailDef_A4_b71 {
    u8 pad_0x0[0xf0];
    base::UnkVector<UnkKraidNailElem_A4_b71>* groups0;  // 0xf0
    u8 pad_0xf8[0x118 - 0xf8];
    base::UnkVector<UnkKraidNailElem_A4_b71>* groups1;  // 0x118
};

struct UnkKraidNailsAttack_A4_b71 {
    u8 pad_0x0[0x8];
    UnkKraidOwner_A4_b71* owner;
    u8 pad_0x10[0x40];
    u32 group;   // 0x50
    u32 next;    // 0x54
    void* entry;  // 0x58
    UnkKraidNailDef_A4_b71* def;  // 0x60
};

void fn_71005f67cc(UnkKraidNailsAttack_A4_b71* self) {
    UnkKraidComp_A4_b71* obj = self->owner->ai;
    UnkKraidComp_A4_b71* kraid =
        fn_710009d50c(reinterpret_cast<const CClass* (*)(UnkKraidComp_A4_b71*)>(obj->vtable[0])(obj), fn_71005fac74())
            ? obj
            : nullptr;
    UnkKraidNailDef_A4_b71* def = self->def;
    base::UnkVector<UnkKraidNailElem_A4_b71>& g = (kraid->phase != 0 ? def->groups1 : def->groups0)[self->group];
    u32 n = g.count;
    u32 limit = kraid->field_0x1170 == 0 ? (n < 2 ? n : 2) : n;
    if (self->next < limit) {
        asm("" : "+r"(def));
        self->entry = (kraid->phase == 0 ? def->groups0 : def->groups1)[self->group].GrowAt(self->next).ptr;
        ++self->next;
    } else {
        self->entry = nullptr;
    }
}
