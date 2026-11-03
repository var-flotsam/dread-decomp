// CWeightActivableMovablePlatformComponent unit (unit 1432 of tools/tu_map.py): the destructor,
// fn_7100952fec. The vtable (g_unk_7101a849b8) is stored, the string-id table at +0x290
// (base/UnkStrIdTable.h) reset and its arrays freed, the string ids at +0x288 and +0x270 released,
// then the base destructor (fn_71009516e0) runs. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a849b8[];  // vtable

struct UnkWeightActivablePlatform_b52;
void fn_71009516e0(UnkWeightActivablePlatform_b52* self);  // base destructor

struct UnkWeightActivablePlatform_b52 {
    void** vtable;
    u8 unk_0x8[0x268];
    base::UnkStrIdMember field_0x270;
    u8 unk_0x278[0x10];
    base::UnkStrIdMember field_0x288;
    UnkStrIdTable field_0x290;
};

void fn_7100952fec(UnkWeightActivablePlatform_b52* self) {
    self->vtable = g_unk_7101a849b8 + 2;
    fn_71004f11f8(&self->field_0x290);
    if (self->field_0x290.keys)
        self->field_0x290.allocator->vf_0x48(self->field_0x290.keys);
    if (self->field_0x290.values)
        self->field_0x290.allocator->vf_0x48(self->field_0x290.values);
    self->field_0x288.~UnkStrIdMemberOf();
    self->field_0x270.~UnkStrIdMemberOf();
    fn_71009516e0(self);
}
