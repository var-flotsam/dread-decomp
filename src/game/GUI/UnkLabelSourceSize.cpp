// Unit 1722 of tools/tu_map.py (0x7100b81620-0x7100b84da4; its reflection registrations are
// declared in src/game/CChangeStageNavMeshItemComponent/fn_71009ba9f0.cpp): the size of a label's
// source in the frame `frame` names. src/game/GUI/CLabel.cpp lays the source out; names are
// placeholders.
#include "base/math/CVector.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"

using base::math::CVector2D;

typedef u32 __attribute__((may_alias)) UnkAliasWord_7100b82b18;

// The image a source shows: its width and height in pixels at +0x10.
struct UnkLabelImage {
    u8 unk_0x0[0x10];
    u32 width;
    u32 height;
};

struct UnkLabelSource {
    u8 unk_0x0[0x8];
    CVector2D pos;
    CVector2D size;  // a fraction of the image
    UnkLabelImage* field_0x18;
};

// The source's size in pixels (its fraction of the image), scaled for the frame; zero when the image
// is empty.
UnkLabelVec2 fn_7100b82b18(const UnkLabelSource* source, s32 frame) {
    UnkLabelVec2 r(0.0f, 0.0f);
    const UnkLabelImage* image = source->field_0x18;
    u32 width = image->width;
    u32 height = image->height;
    if (width != 0 && height != 0) {
        CVector2D size = source->size;
        f32 h = (f32)height;
        f32 w = (f32)width;
        CVector2D v = {size.x * w, size.y * h};
        UnkLabelVec2 t = fn_71011b7fc8(g_unk_717271e8f8, frame, &v);
        reinterpret_cast<UnkAliasWord_7100b82b18*>(&r)[0] = reinterpret_cast<UnkAliasWord_7100b82b18*>(&t)[0];
        reinterpret_cast<UnkAliasWord_7100b82b18*>(&r)[1] = reinterpret_cast<UnkAliasWord_7100b82b18*>(&t)[1];
    }
    return r;
}
