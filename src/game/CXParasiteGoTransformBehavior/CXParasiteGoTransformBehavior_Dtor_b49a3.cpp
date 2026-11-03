// CXParasiteGoTransformBehavior unit (tools/tu_map.py unit 1228). fn_71007cd87c: a destructor: the
// vtable, the array of 0x40-byte polymorphic entries at +0x60 (each destroyed through its vtable, then
// the array freed), then the member at +0x20 (fn_710023ae38). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a71818[];
void fn_710023ae38(void* self);

struct UnkGoTransformEntry_b49a3 {
    virtual ~UnkGoTransformEntry_b49a3();
    u8 pad_0x8[0x38];
};

struct UnkXParasiteGoTransformBehavior_b49a3 {
    void** vtable;
    u8 pad_0x8[0x18];
    u8 field_0x20[0x40];
    base::UnkVector<UnkGoTransformEntry_b49a3> entries;  // 0x60
};

void fn_71007cd87c(UnkXParasiteGoTransformBehavior_b49a3* self) {
    self->vtable = g_unk_7101a71818 + 2;
    self->entries.Free();
    fn_710023ae38(self->field_0x20);
}
