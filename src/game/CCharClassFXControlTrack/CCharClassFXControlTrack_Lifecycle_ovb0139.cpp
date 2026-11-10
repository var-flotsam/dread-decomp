// CCharClassFXControlTrack's unit (tools/tu_map.py): fn_7100cfc7f8 is the destructor (vtable g_unk_7101aa6f40, base fn_710006b5dc):
// it releases two id arrays (+0x50, +0x20) and two ids (+0x48, +0x18); fn_7100cfce30 is the copy constructor, which copies the
// two ids and two id arrays (base::UnkStrIdVec). Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a1a138[];  // the base's vtable
extern void* g_unk_7101aa6f40[];  // the class's vtable
void fn_710006b5dc(void* self);   // the base destructor

struct UnkFXControlTrack_ovb0139 {
    void** vtable;
    u8 pad_0x8[0x10];
    base::UnkStrIdMember name_0x18;
    base::UnkVector<base::UnkStrIdMember> ids_0x20;  // 0x20
    base::UnkStrIdMember name_0x48;
    base::UnkVector<base::UnkStrIdMember> ids_0x50;  // 0x50
};

void fn_7100cfc7f8(UnkFXControlTrack_ovb0139* self) {
    self->vtable = g_unk_7101aa6f40 + 2;
    self->ids_0x50.Free();
    using base::UnkStrIdMember;
    self->name_0x48.~UnkStrIdMember();
    self->ids_0x20.Free();
    self->name_0x18.~UnkStrIdMember();
    fn_710006b5dc(self);
}

struct UnkFXControlTrackData_ovb0139 {
    void** vtable;
    u8 pad_0x8[0x10];
    CStrId name_0x18;
    base::UnkStrIdVec ids_0x20;
    CStrId name_0x48;
    base::UnkStrIdVec ids_0x50;
};

void fn_7100cfce30(UnkFXControlTrackData_ovb0139* self, const UnkFXControlTrackData_ovb0139* other) {
    self->vtable = g_unk_7101a1a138 + 2;
    __builtin_memcpy(&self->pad_0x8, &other->pad_0x8, 0x10);
    self->vtable = g_unk_7101aa6f40 + 2;
    new (&self->name_0x18) CStrId(other->name_0x18);
    new (&self->ids_0x20) base::UnkStrIdVec(other->ids_0x20);
    new (&self->name_0x48) CStrId(other->name_0x48);
    new (&self->ids_0x50) base::UnkStrIdVec(other->ids_0x50);
}
