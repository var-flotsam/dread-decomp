// CCharClassFrozenComponent's unit (tools/tu_map.py unit 1257): the constructor the frozen-ice infos
// build on (src/game/CBarelyFrozenIceInfo/CBarelyFrozenIceInfo_Ctor_b44.cpp calls it): the char class
// base constructor (fn_71011acd10), the vtable, +0x58 cleared and eight copies of the empty string id
// at +0x60; the same as the component constructor fn_71007ee89c without its virtual call. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkFrozenIceBase_b44a2;
struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30* self);
extern void* g_unk_7101a73dc0[];

struct UnkCharClassFrozen_ov110 {
    u64 vtable;  // an integer, so the id stores don't make its store move (as fn_71007ee89c)
    u8 pad_0x8[0x50];
    u64 field_0x58;
    CStrId ids[8];  // 0x60
};
static_assert(__builtin_offsetof(UnkCharClassFrozen_ov110, ids) == 0x60, "layout");

void fn_71007ee4c8(UnkFrozenIceBase_b44a2* base) {
    UnkCharClassFrozen_ov110* self = reinterpret_cast<UnkCharClassFrozen_ov110*>(base);
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
}
