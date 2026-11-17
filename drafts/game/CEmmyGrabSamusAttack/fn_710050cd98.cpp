// CEmmyGrabSamusAttack unit (tools/tu_map.py): resets the four floats at 0x6b4..0x6c0 (0, -1, -1, -1),
// sends the string id for the current kind (field_0x38's +0x10, or 14 without one) unless it is the
// empty id, releases it and plays the grab rumble (fn_710050ed80). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// fn_71005077f8's id type (src/game/CEmmyGrabSamusAttack/fn_71005077f8.cpp): a data pointer with a
// user copy constructor, returned through x8.
struct UnkStrId_71005077f8 {
    UnkStringData* data;
    UnkStrId_71005077f8(const UnkStrId_71005077f8& other);
};

struct UnkKindSource_l50cd98 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkGrabAttack_l50cd98 {
    u8 pad_0x0[0x38];
    UnkKindSource_l50cd98* field_0x38;
    u8 pad_0x40[0x6b4 - 0x40];
    f32 field_0x6b4;
    f32 field_0x6b8;
    f32 field_0x6bc;
    f32 field_0x6c0;
};

struct UnkEmmyGrabAttack_l50ed80;
void fn_710050ed80(UnkEmmyGrabAttack_l50ed80* self);

void fn_710050cd98(UnkSelf* self_) {
    auto* self = reinterpret_cast<UnkGrabAttack_l50cd98*>(self_);
    self->field_0x6b4 = 0.0f;
    self->field_0x6b8 = -1.0f;
    self->field_0x6bc = -1.0f;
    self->field_0x6c0 = -1.0f;
    u32 kind = self->field_0x38 ? self->field_0x38->field_0x10 : 14;
    UnkStrId_71005077f8 id = fn_71005077f8(reinterpret_cast<UnkStruct_71005077f8*>(self), kind);
    UnkStringData* data = id.data;
    if (data != &g_unk_7101d47108->empty)
        fn_710050a500(reinterpret_cast<UnkGrabAttack_b59a3*>(self), reinterpret_cast<const UnkGrabId_b59a3*>(&id));
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (data && data != &pool->empty) {
            if (--data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47108;
            }
        }
        id.data = &pool->empty;
    }
    fn_710050ed80(reinterpret_cast<UnkEmmyGrabAttack_l50ed80*>(self));
}
