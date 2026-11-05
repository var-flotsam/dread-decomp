// CGrappleBeamGun's unit (tools/tu_map.py), fn_7100833448: a destructor. It stores its vtable, hands the object
// the weak handle at +0x1880 points to (if any) to fn_7100232c68 and drops that handle, drops the one at +0x1888, then
// the members' own releases run and the base destructor (fn_710085733c) follows. Names and layouts are
// placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100232c68(void*);
extern void* g_unk_7101a77768[];

struct UnkWeakRef_A4_b76 {
    UnkWeakHandle* handle;
    ~UnkWeakRef_A4_b76() {
        if (UnkWeakHandle* p = handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkGrappleBeamGunBase_A4_b76 {
    void** vtable;
};
void fn_710085733c(UnkGrappleBeamGunBase_A4_b76* self);

struct UnkGrappleBeamGun_A4_b76 : UnkGrappleBeamGunBase_A4_b76 {
    u8 pad_0x8[0x1880 - 0x8];
    UnkWeakRef_A4_b76 field_0x1880;
    UnkWeakRef_A4_b76 field_0x1888;
};

void fn_7100833448(UnkGrappleBeamGun_A4_b76* self) {
    self->vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101a77768) + 0x10);
    if (UnkWeakHandle* h = self->field_0x1880.handle) {
        if (h->target) {
            fn_7100232c68(h->target);
            ResetRef(&self->field_0x1880.handle);
        }
    }
    ResetRef(&self->field_0x1888.handle);
    // The original reloads +0x1880 here, after the second drop.
    asm("" ::: "memory");
    self->field_0x1880.~UnkWeakRef_A4_b76();
    fn_710085733c(self);
}
