// Destructors (complete and deleting) of an object in the CAIGotoPointNavigationState::STarget unit
// (vtable 0x7101a6bb48): its member at +0x140 (fn_71007068ec), the sub-object at +0xb0 (vtable
// 0x7101a25b38) with a reflected value at +0x120, then the base destructor fn_7100703878. Names
// and layouts are placeholders.
#include <new>

#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkGotoTargetBase;
void fn_7100703878(UnkGotoTargetBase* self);
void fn_71007068ec(void* self);
extern void* g_unk_7101a6bb48[];
extern void* g_unk_7101a25b38[];

struct UnkGotoTarget {
    void** vtable;
    u8 pad_0x8[0xa8];
    void** field_0xb0;
    u8 pad_0xb8[0x68];
    base::reflection::UnkReflValue field_0x120;
    u8 pad_0x138[0x8];
    u8 field_0x140[0x8];

    __attribute__((always_inline)) ~UnkGotoTarget() {
        vtable = g_unk_7101a6bb48 + 2;
        fn_71007068ec(field_0x140);
        field_0xb0 = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&field_0x120);
        fn_7100703878(reinterpret_cast<UnkGotoTargetBase*>(this));
    }
};

void fn_71007021e8(UnkGotoTarget* self) { self->~UnkGotoTarget(); }
void fn_7100702238(UnkGotoTarget* self) {
    self->~UnkGotoTarget();
    operator delete(self);
}
