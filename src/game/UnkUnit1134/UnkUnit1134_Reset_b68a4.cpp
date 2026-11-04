// Unit 1134 of tools/tu_map.py (no name hint). fn_710070d508: resets the object: notifies the member at
// +0x18 (virtual 0x48), drops the counted reference at +0x28, sets the string id at +0x30 to the global
// empty id, resets the member at +0xb0, stores {0, 1.0f} at +0x160 and finishes with the member at +0x38
// (fn_7100714dd0). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkUnit1134Listener_b68a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
};

struct UnkUnit1134_b68a4 {
    u8 pad_0x0[0x18];
    UnkUnit1134Listener_b68a4* listener;  // 0x18
    u8 pad_0x20[0x8];
    UnkRefObject* ref;  // 0x28
    CStrId id;          // 0x30
    u8 field_0x38[0x78];
    u8 field_0xb0[0xb0];
    f32 field_0x160;
    f32 field_0x164;
};

void fn_710002fa90(void*);
void fn_7100714dd0(void* self);

void fn_710070d508(UnkUnit1134_b68a4* self) {
    if (self->listener)
        self->listener->vf_0x48();
    if (UnkRefObject* p = self->ref) {
        if (--p->ref_count == 0)
            p->vf_0x10();
        self->ref = nullptr;
    }
    self->id = g_unk_7101d470e0;
    fn_710002fa90(self->field_0xb0);
    self->field_0x160 = 0.0f;
    self->field_0x164 = 1.0f;
    fn_7100714dd0(self->field_0x38);
}
