// CCharClassFrozenComponent (the name from reflection): the in-place constructor its reflection descriptor points
// to (fn_0x28). After the char class base constructor (fn_71011acd10) it sets the vtable, clears +0x58, copies the
// global empty string id (g_unk_7101d470e0) into the eight ids at +0x60, then makes the post-construction virtual
// call (slot 3). Defined under the `void()` symbol src/reflection/CCharClassFrozenComponent.cpp declares. Names
// and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkCharClass_A5_b71;
struct UnkStruct_71008bca30;
extern void* g_unk_7101a73dc0[];

struct UnkCharClassFrozenCtor_A5_b71 {
    u64 vtable;  // an integer, so the id stores don't make the slot load reread it
    u8 pad_0x8[0x50];
    u64 field_0x58;
    CStrId ids[8];  // 0x60
};
static_assert(__builtin_offsetof(UnkCharClassFrozenCtor_A5_b71, ids) == 0x60, "layout");

void fn_71007ee89c_A5_b71(UnkCharClassFrozenCtor_A5_b71* self) asm("_Z13fn_71007ee89cv");
void fn_71007ee89c_A5_b71(UnkCharClassFrozenCtor_A5_b71* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = reinterpret_cast<u64>(&g_unk_7101a73dc0[2]);
    self->field_0x58 = 0;
    new (&self->ids[0]) CStrId(g_unk_7101d470e0);
    new (&self->ids[1]) CStrId(g_unk_7101d470e0);
    new (&self->ids[2]) CStrId(g_unk_7101d470e0);
    new (&self->ids[3]) CStrId(g_unk_7101d470e0);
    new (&self->ids[4]) CStrId(g_unk_7101d470e0);
    new (&self->ids[5]) CStrId(g_unk_7101d470e0);
    new (&self->ids[6]) CStrId(g_unk_7101d470e0);
    new (&self->ids[7]) CStrId(g_unk_7101d470e0);
    reinterpret_cast<void (*)(UnkCharClassFrozenCtor_A5_b71*)>(reinterpret_cast<void**>(self->vtable)[3])(self);
}
