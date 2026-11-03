// CCharClassWaterPoolComponent unit (tools/tu_map.py unit 1396): float getters reading entry +0xcf8 or +0xcfc of
// the per-frame float array at +0xd00 (fn_71000d4a50, the accessor from src/game/UnkFrameSource.cpp), or
// -1 when the array is empty. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

template <class T>
struct UnkFrameVector;

f32* fn_71000d4a50(UnkFrameVector<f32>* v, u32 i);

struct UnkWaterPool1396 {
    u8 pad_0x0[0xcf8];
    u32 index;  // 0xcf8
    u32 index_0xcfc;
    base::UnkVector<f32> values;  // 0xd00
};

f32 fn_7100909ca4(UnkWaterPool1396* self) {
    if (self->values.count == 0)
        return -1.0f;
    return *fn_71000d4a50(reinterpret_cast<UnkFrameVector<f32>*>(&self->values), self->index);
}

f32 fn_7100909e08(UnkWaterPool1396* self) {
    if (self->values.count == 0)
        return -1.0f;
    return *fn_71000d4a50(reinterpret_cast<UnkFrameVector<f32>*>(&self->values), self->index_0xcfc);
}
