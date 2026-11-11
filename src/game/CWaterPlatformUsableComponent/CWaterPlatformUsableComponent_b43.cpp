// CWaterPlatformUsableComponent's unit (tools/tu_map.py): the component's constructor, on its base
// (fn_7100a9e244): its vtable, two cleared runs (0x24 bytes at +0x53c, 0x18 at +0x564), an empty
// string, three cleared words, the puzzle member built from the "watervalevepuzzle" id with 800 and 5
// (fn_71002b2afc), an empty box (min at +FLT_MAX, max at -FLT_MAX) and a set flag. Names and layouts
// are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CUsableComponent.h"
#include "types.h"

#include <new>

using base::global::CRntString;
using base::global::CStrId;

extern char g_unk_7101a94758[];  // the component's vtable

struct UnkWaterPuzzle_b43a3 {
    u8 pad_0x0[0x14];
    bool field_0x14;
    u8 pad_0x15[0x2b];
};
void fn_71002b2afc(UnkWaterPuzzle_b43a3* self, const CStrId& id, f32 a, f32 b);

struct UnkWaterPlatformUsable_b43a3 {
    char* vtable;
    u8 pad_0x8[0x534];
    u8 field_0x53c[0x24];
    u8 pad_0x560[0x4];
    u8 field_0x564[0x18];
    u8 pad_0x57c[0x4];
    CRntString name;  // 0x580
    u32 field_0x5b0;
    u64 field_0x5b8;
    u64 field_0x5c0;
    UnkWaterPuzzle_b43a3 puzzle;  // 0x5c8
    f32 min[2];  // 0x608
    f32 max[2];  // 0x610
};
static_assert(__builtin_offsetof(UnkWaterPlatformUsable_b43a3, min) == 0x608, "UnkWaterPlatformUsable_b43a3 layout");

void fn_7100aa5338(UnkWaterPlatformUsable_b43a3* self) {
    fn_7100a9e244(self);
    self->vtable = g_unk_7101a94758 + 0x10;
    __builtin_memset(self->field_0x53c, 0, sizeof(self->field_0x53c));
    __builtin_memset(self->field_0x564, 0, sizeof(self->field_0x564));
    new (&self->name) CRntString("");
    self->field_0x5b0 = 0;
    self->field_0x5b8 = 0;
    self->field_0x5c0 = 0;
    fn_71002b2afc(&self->puzzle, CStrId("watervalevepuzzle", true), 800.0f, 5.0f);
    self->min[0] = 3.4028235e38f;
    self->min[1] = 3.4028235e38f;
    self->max[0] = -3.4028235e38f;
    self->max[1] = -3.4028235e38f;
    self->puzzle.field_0x14 = true;
}
