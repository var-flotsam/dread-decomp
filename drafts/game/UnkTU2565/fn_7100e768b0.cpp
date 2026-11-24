// Unit 2565 (tools/tu_map.py): builds the minimap's character table, a byte vector with the given growth
// step, allocator (the default one when null) and allocation flags, filled from `chars`.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU2565.h"

#include "types.h"

struct UnkCharMap : base::global::CRntVector<u8> {
    UnkCharMap() = delete;
};

void fn_7100e768b0(UnkCharMap* self, const char* chars, u32 count, u32 a, void* b, u32 c) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = a;
    self->field_0x14 = a != 0;
    self->flags = c;
    base::IAllocator* allocator = static_cast<base::IAllocator*>(b);
    if (!allocator)
        allocator = GetDefaultAllocator();
    self->allocator = allocator;
    self->Reserve(count);
    const u8* p = reinterpret_cast<const u8*>(chars);
    for (u64 i = 0; i < count; ++i) {
        u8 ch = p[i];
        self->PushBackCopy(ch);
    }
}
