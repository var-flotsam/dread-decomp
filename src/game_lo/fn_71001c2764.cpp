// Unless a global flag blocks it, forwards a request keyed by two ids to a sub-object; when it
// succeeds and `remove` is set, moves every matching entry of the object's list past its end
// (swapping it with the last entry and shrinking the count).
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkSlots.h"
#include "types.h"

// The vector as the original copies it: a 16-byte value returned through x8, so not trivially
// copyable.
struct UnkValue_71001c2764 {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    UnkValue_71001c2764(const UnkValue_71001c2764& o);
};
UnkValue_71001c2764 fn_7100030878(const UnkValue_71001c2764* src);

bool fn_71001996e0(void* target, u32 a, u32 b, UnkValue_71001c2764 value, u32 c);

bool fn_71001c2764(UnkSlots* self, u32 a, u32 b, const base::math::CVector4D* value, u32 c, bool remove) {
    if (g_unk_710b372a98->field_0x318367c)
        return false;
    if (!fn_71001996e0(self->info, a, b, fn_7100030878(reinterpret_cast<const UnkValue_71001c2764*>(value)), c))
        return false;
    if (remove) {
        for (u32 i = 0; i < self->count; ++i) {
            UnkSlotEntry* e = self->entries[i];
            if (e->field_0x28 == a && e->field_0x2c == b) {
                --self->count;
                self->entries[i] = self->entries[self->count];
                self->entries[self->count] = e;
            }
        }
    }
    return true;
}
