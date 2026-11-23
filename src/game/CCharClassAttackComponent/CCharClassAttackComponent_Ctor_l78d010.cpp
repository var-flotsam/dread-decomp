// CCharClassAttackComponent (unit 1185): the constructor. The base constructor fn_71011acd10, the vtable,
// string ids (the empty one, one from a string, and several from the empty text), cleared counters and
// two arrays on the default allocator. Names and layout are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CCharClassComponent.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::global::CStrId;

// An array as this class lays it out: the pointer, the counts, then a step, flags and the allocator.
struct UnkAttackArray_l78d010 {
    void* data;       // 0xa0
    u32 count;        // 0xa8
    u32 capacity;     // 0xac
    u64 field_0xb0;
    u32 step;         // 0xb8
    u32 flags;        // 0xbc
    base::UnkAllocator* allocator;  // 0xc0
};

struct UnkCharClassAttack_l78d010 {
    void** vtable;
    u64 field_0x8;
    u8 pad_0x10[0x8];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    CStrId id_0x58;
    CStrId id_0x60;
    u32 field_0x68;
    bool field_0x6c;
    u32 field_0x70;
    u16 field_0x74;
    u32 field_0x78;
    u8 pad_0x7c[4];
    CStrId id_0x80;
    CStrId id_0x88;
    CStrId id_0x90;
    CStrId id_0x98;
    UnkAttackArray_l78d010 array_0xa0;
    CStrId id_0xc8;
    base::global::CRntVector<void*> array_0xd0;
};

void fn_710078d010(UnkCharClassAttack_l78d010* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = reinterpret_cast<void**>(g_unk_7101a6f348 + 0x10);
    new (&self->id_0x58) CStrId(base::global::CStrId::Empty());
    CStrId_ConstructFromRntString(&self->id_0x60, g_unk_7101d58d10, true);
    self->field_0x68 = 0;
    self->field_0x6c = false;
    self->field_0x70 = 0;
    self->field_0x74 = 0;
    self->field_0x78 = 0x7fffffff;
    new (&self->id_0x80) CStrId("", true);
    new (&self->id_0x88) CStrId("", true);
    new (&self->id_0x90) CStrId("", true);
    new (&self->id_0x98) CStrId("", true);
    self->array_0xa0.data = nullptr;
    self->array_0xa0.count = 0;
    self->array_0xa0.capacity = 0;
    self->array_0xa0.field_0xb0 = 0;
    self->array_0xa0.step = 10;
    self->array_0xa0.flags = 0xe;
    self->array_0xa0.allocator = GetDefaultAllocator();
    new (&self->id_0xc8) CStrId("", true);
    new (&self->array_0xd0) base::global::CRntVector<void*>(0xf);
    self->field_0x18 = 0x1130;
}
