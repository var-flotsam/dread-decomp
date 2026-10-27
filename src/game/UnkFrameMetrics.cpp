// Two getters of a small unit (tools/tu_map.py: unit 135, initialiser fn_7100147a80) that read
// per-frame values of the object at +0x18 of their owner (the unit at fn_71001bd4e8..fn_71001bd628:
// the current frame, a 16-bit count, and per-frame vectors and floats with bounds checks) and
// scale them by the global factor fn_71001d7320 while the flag of game/UnkGlobal_710b372a98.h at
// +0x318367b is set. Holds fn_7100147960 and fn_71001479dc.
#include "base/math/CVector.h"
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

using base::math::CVector2D;

// The object at +0x18 (only its functions are used here).
struct UnkFrameSource;

u32 fn_71001bd620(UnkFrameSource* self);  // the current frame
f32 fn_71001bd628(UnkFrameSource* self);  // the 16-bit count at +0x9c
void fn_71001bd4e8(UnkFrameSource* self, u32 frame, CVector2D* out);
void fn_71001bd5a8(UnkFrameSource* self, u32 frame, CVector2D* out);
void fn_71001bd548(UnkFrameSource* self, u32 frame, f32* out);
f32 fn_71001d7320(UnkGlobal_710b372a98* self);

struct UnkFrameOwner {
    u8 unk_0x0[0x18];
    // The object embedded at +0x18.
    UnkFrameSource* Source() {
        return reinterpret_cast<UnkFrameSource*>(reinterpret_cast<u8*>(this) + 0x18);
    }
};

// The current frame's float value, scaled while the global flag is set.
u32 fn_7100147960(UnkFrameOwner* self) {
    f32 value = 0.0f;
    fn_71001bd548(self->Source(), fn_71001bd620(self->Source()), &value);
    if (g_unk_710b372a98->pad_0x0[0x318367b])
        value = fn_71001d7320(g_unk_710b372a98) * value;
    return (u32)value;
}

// The count less the current frame's first vector's y plus its second vector's y, scaled while the
// global flag is set.
u32 fn_71001479dc(UnkFrameOwner* self) {
    CVector2D second = {0.0f, 0.0f};
    CVector2D first = {0.0f, 0.0f};
    UnkFrameSource* source = self->Source();
    u32 frame = fn_71001bd620(source);
    f32 value = fn_71001bd628(source);
    fn_71001bd4e8(source, frame, &first);
    fn_71001bd5a8(source, frame, &second);
    value = value - first.y + second.y;
    if (g_unk_710b372a98->pad_0x0[0x318367b])
        value = value * fn_71001d7320(g_unk_710b372a98);
    return (u32)value;
}
