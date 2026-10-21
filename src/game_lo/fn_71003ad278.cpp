// Reset: runs the base reset, clears a group of state fields and the array of 0x40-byte
// polymorphic elements at +0x146c8, then (unless the owner at +0x14908 says otherwise)
// re-applies each stored value through the linked object (once with the value, once with
// zero) and marks the object at +0x30.
#include "base/UnkVector.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;

class UnkElem_71003ad278 {
public:
    virtual ~UnkElem_71003ad278();
    u8 pad_0x8[0x38];
};

struct UnkTarget_71003ad278 {
    u8 pad_0x0[0x138];
    void* field_0x138;
};

struct UnkLinkSub_71003ad278 {
    u8 pad_0x0[0x50];
    UnkTarget_71003ad278* target;  // 0x50
};

struct UnkLink_71003ad278 {
    u8 pad_0x0[0x1b8];
    void* field_0x1b8;
    u8 pad_0x1c0[0x228 - 0x1c0];
    UnkLinkSub_71003ad278* field_0x228;
    u8 pad_0x230[0x312 - 0x230];
    bool field_0x312;
};

struct UnkFlags_71003ad278 {
    u8 pad_0x0[0x50c];
    bool field_0x50c;
    u8 pad_0x50d[0x3];
    bool field_0x510;
};

struct UnkEntry_71003ad278 {
    u32 field_0x0;
    u32 id;
    CVector4D value;
};

struct UnkObject_71003ad278 {
    u8 pad_0x0[0x30];
    UnkLink_71003ad278* field_0x30;
    u8 pad_0x38[0x146c8 - 0x38];
    base::UnkVector<UnkElem_71003ad278> elems;  // 0x146c8
    u8 pad_0x146f0[0x147e9 - 0x146f0];
    bool field_0x147e9;
    u8 pad_0x147ea[0x2];
    u32 field_0x147ec;
    u32 field_0x147f0;
    bool field_0x147f4;
    u8 pad_0x147f5[0x3];
    u64 field_0x147f8;
    u32 field_0x14800;
    s32 field_0x14804;
    u8 pad_0x14808[0x4];
    f32 field_0x1480c;
    u32 field_0x14810;
    u32 entry_count;                   // 0x14814
    UnkEntry_71003ad278 entries[8];    // 0x14818
    bool field_0x148d8;
    u8 pad_0x148d9[0x3];
    u32 field_0x148dc;
    bool field_0x148e0;
    u8 pad_0x148e1[0x3];
    u32 field_0x148e4;
    u32 field_0x148e8;
    u8 pad_0x148ec[0x14908 - 0x148ec];
    UnkFlags_71003ad278* field_0x14908;
};

extern const CVector4D unk_71016491b0 __attribute__((visibility("hidden")));  // zero

void fn_71003695b8(UnkObject_71003ad278* self);  // the base reset
bool fn_71001c2764(void* target, u32 id, u32 kind, const CVector4D* value, bool a, bool b);

static inline void Apply(UnkTarget_71003ad278* target, u32 id, u32 kind, const CVector4D* value) {
    fn_71001c2764(target->field_0x138, id, kind, value, true, true);
}

void fn_71003ad278(UnkObject_71003ad278* self) {
    fn_71003695b8(self);
    self->field_0x147e9 = false;
    self->field_0x147ec = 0;
    self->field_0x147f0 = 0;
    self->field_0x147f4 = false;
    self->field_0x147f8 = 0;
    self->field_0x14800 = 0;
    self->field_0x14804 = -1;
    self->field_0x148d8 = false;
    self->field_0x148dc = 0;
    self->field_0x148e0 = false;
    self->elems.Clear();
    self->field_0x148e4 = 0;
    self->field_0x148e8 = 0;
    UnkFlags_71003ad278* flags = self->field_0x14908;
    if (flags->field_0x50c)
        return;
    if (flags->field_0x510) {
        UnkLink_71003ad278* link = self->field_0x30;
        if (link->field_0x228) {
            if (UnkTarget_71003ad278* target = link->field_0x228->target) {
                if (link->field_0x1b8) {
                    self->field_0x1480c = -1.0f;
                    self->field_0x14810 = 0;
                    for (u32 i = 0; i < self->entry_count;) {
                        UnkEntry_71003ad278* e = &self->entries[i++];
                        Apply(target, e->id, 2, &e->value);
                        Apply(target, e->id, 1, &unk_71016491b0);
                    }
                }
            }
        }
    }
    self->field_0x30->field_0x312 = true;
}
