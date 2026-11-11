// CCharClassGrabComponent's unit (tools/tu_map.py unit 1190): the constructor (fn_71007905b8): the base
// constructor fn_71011acd10, the two vtables (at 0 and +0x58), a cleared block with the default allocator
// and flags 0x13, an empty string id at +0x88, the default mask (the complement of a guarded zero's low
// byte, both guarded statics) at +0x90 and 0x10cc at +0x18. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/CCharClassGrabComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71008bca30;

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkGrabClassStaticZero_ovb0120 {
    u32 value;
    UnkGrabClassStaticZero_ovb0120() : value(0) {}
};

inline const UnkGrabClassStaticZero_ovb0120& UnkGrabClassStaticZero() {
    static UnkGrabClassStaticZero_ovb0120 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

// The complement of that zero's low eight bits.
inline u32 UnkGrabClassDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkGrabClassStaticZero().value & 0xff;
    return s_mask;
}

struct UnkCharClassGrab_ovb0120 {
    void** vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    void** vtable_0x58;
    u32 field_0x60;
    u64 field_0x68;
    u64 field_0x70;
    base::UnkAllocator* allocator;  // 0x78
    u32 field_0x80;
    u8 pad_0x84[4];
    CStrId field_0x88;
    u32 field_0x90;
};
static_assert(__builtin_offsetof(UnkCharClassGrab_ovb0120, field_0x90) == 0x90, "layout");

void fn_71007905b8(UnkCharClassGrab_ovb0120* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = &g_unk_7101a6f508[2];
    self->vtable_0x58 = &g_unk_7101a6f558[2];
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0x70 = 0;
    self->allocator = fn_7100083990();
    self->field_0x80 = 0x13;
    new (&self->field_0x88) CStrId(g_unk_710162e2ed, true);
    self->field_0x90 = UnkGrabClassDefaultMask();
    self->field_0x18 = 0x10cc;
}
