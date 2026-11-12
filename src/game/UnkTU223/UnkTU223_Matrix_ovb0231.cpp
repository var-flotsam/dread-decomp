// Unit 223 (tools/tu_map.py): a lazily copied matrix, as src/game_s1/fn_7100196674.cpp: on first use the
// matrix at +0x20 is copied to +0x108 and its y axis scaled by the aspect ratio of `size`
// (fn_710015bf40). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkTU178.h"
#include "types.h"

struct UnkSize_b0231 {
    u8 pad_0x0[0x8];
    u32 width;
    u32 height;
};

struct UnkCachedMatrix_b0231 {
    u8 pad_0x0[0x20];
    UnkMatrix44 source;  // 0x20
    u8 pad_0x60[0xa8];
    UnkMatrix44 matrix;  // 0x108
    u8 pad_0x148[0x3a];
    bool cached;  // 0x182
};
static_assert(__builtin_offsetof(UnkCachedMatrix_b0231, cached) == 0x182, "layout");

UnkMatrix44* fn_710019c3c8(UnkCachedMatrix_b0231* self, const UnkSize_b0231* size) {
    if (!self->cached) {
        self->cached = true;
        UnkCopyMatrix(&self->matrix, self->source);
        fn_710015bf40(&self->matrix, (f32)size->width / (f32)size->height);
    }
    return &self->matrix;
}
