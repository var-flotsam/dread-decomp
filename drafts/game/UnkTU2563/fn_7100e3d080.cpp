// Unit 2563 (CMinimapData?): fills a layout entry from an object: its position, the one or two
// part ids chosen by the object's type id, and the rectangles those parts cover.
// Names and layouts are placeholders.

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkNameHolder_b0161;
struct UnkVec3;
UnkNameHolder_b0161* fn_7100232fcc(void* object);
void fn_710007b444(void* dst, const UnkVec3* value);

struct UnkPos_le3d080 {
    u32 x, y, z;
};

struct UnkRect_le3d080 {
    f32 left, top, right, bottom;
};

struct UnkObject_le3d080 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkLayout_le3d080 {
    f32 field_0x0;
    f32 field_0x4;
    UnkRect_le3d080 field_0x8;
    UnkRect_le3d080 field_0x18;
    CStrId field_0x28;
    CStrId field_0x30;
};

#define HIDDEN __attribute__((visibility("hidden"))) extern
HIDDEN CStrId g_unk_71727c7198;
HIDDEN CStrId g_unk_71727c71a0;
HIDDEN CStrId g_unk_71727c71a8;
HIDDEN CStrId g_unk_71727c71b0;
HIDDEN CStrId g_unk_71727c71b8;
HIDDEN CStrId g_unk_71727c71c0;
HIDDEN CStrId g_unk_71727c71c8;
HIDDEN CStrId g_unk_71727c71d0;
HIDDEN CStrId g_unk_71727c71d8;
HIDDEN CStrId g_unk_71727c71e0;
HIDDEN CStrId g_unk_71727c71e8;
HIDDEN CStrId g_unk_71727c71f0;
HIDDEN CStrId g_unk_71727c71f8;
HIDDEN CStrId g_unk_71727c7200;
HIDDEN CStrId g_unk_71727c7208;
HIDDEN CStrId g_unk_71727c7210;
HIDDEN CStrId g_unk_71727c7218;
HIDDEN CStrId g_unk_71727c7220;
#undef HIDDEN

extern CStrId g_unk_71727c8108;
extern CStrId g_unk_71727c8110;
extern CStrId g_unk_71727c8118;
extern CStrId g_unk_71727c8120;
extern CStrId g_unk_71727c8138;
extern CStrId g_unk_71727c8140;
extern CStrId g_unk_71727c8158;
extern CStrId g_unk_71727c8160;
extern CStrId g_unk_71727c8178;
extern CStrId g_unk_71727c8180;
extern CStrId g_unk_71727c8198;
extern CStrId g_unk_71727c81a0;
extern CStrId g_unk_71727c81a8;
extern CStrId g_unk_71727c81b0;
extern CStrId g_unk_71727c81c0;
extern CStrId g_unk_71727c81d0;
extern CStrId g_unk_71727c81e0;

static inline const CStrId& TypeOf(UnkObject_le3d080* object) {
    return *reinterpret_cast<const CStrId*>(fn_7100232fcc(object));
}

// Per-case bodies are macros so each type keeps its own assignments.
#define ONE_PART(a) \
    { out->field_0x28 = (a); }
#define TWO_PARTS(a, b) \
    {                   \
        out->field_0x28 = (a); \
        out->field_0x30 = (b); \
    }

void fn_7100e3d080(UnkObject_le3d080* self, UnkLayout_le3d080* out) {
    CStrId type(TypeOf(self));

    UnkPos_le3d080 pos = {self->field_0x44, self->field_0x54, self->field_0x64};
    fn_710007b444(out, reinterpret_cast<const UnkVec3*>(&pos));

    if (type.data == g_unk_71727c7208.data)
        ONE_PART(g_unk_71727c81b0)
    else if (TypeOf(self).data == g_unk_71727c7200.data)
        TWO_PARTS(g_unk_71727c8108, g_unk_71727c8110)
    else if (type.data == g_unk_71727c7198.data)
        ONE_PART(g_unk_71727c81e0)
    else if (type.data == g_unk_71727c71b8.data)
        TWO_PARTS(g_unk_71727c8138, g_unk_71727c8140)
    else if (type.data == g_unk_71727c71c8.data)
        TWO_PARTS(g_unk_71727c8138, g_unk_71727c8120)
    else if (type.data == g_unk_71727c71c0.data)
        TWO_PARTS(g_unk_71727c8118, g_unk_71727c8140)
    else if (type.data == g_unk_71727c71a0.data)
        TWO_PARTS(g_unk_71727c8158, g_unk_71727c8160)
    else if (type.data == g_unk_71727c71a8.data)
        TWO_PARTS(g_unk_71727c8158, g_unk_71727c8120)
    else if (type.data == g_unk_71727c71b0.data)
        TWO_PARTS(g_unk_71727c8118, g_unk_71727c8160)
    else if (type.data == g_unk_71727c71d0.data)
        TWO_PARTS(g_unk_71727c8198, g_unk_71727c81a0)
    else if (type.data == g_unk_71727c71d8.data)
        TWO_PARTS(g_unk_71727c8198, g_unk_71727c8110)
    else if (type.data == g_unk_71727c71e0.data)
        TWO_PARTS(g_unk_71727c8108, g_unk_71727c81a0)
    else if (type.data == g_unk_71727c71e8.data)
        TWO_PARTS(g_unk_71727c8178, g_unk_71727c8180)
    else if (type.data == g_unk_71727c71f0.data)
        TWO_PARTS(g_unk_71727c8178, g_unk_71727c8120)
    else if (type.data == g_unk_71727c71f8.data)
        TWO_PARTS(g_unk_71727c8118, g_unk_71727c8180)
    else if (type.data == g_unk_71727c7210.data)
        ONE_PART(g_unk_71727c81c0)
    else if (type.data == g_unk_71727c7220.data)
        ONE_PART(g_unk_71727c81d0)
    else if (type.data == g_unk_71727c7218.data)
        ONE_PART(g_unk_71727c81a8)

    if (out->field_0x30.data != &g_unk_7101d47108->empty) {
        bool wide = out->field_0x28.data == g_unk_71727c8198.data;
        f32 x = out->field_0x0;
        f32 y = out->field_0x4;
        out->field_0x8.left = x + (wide ? -400.0f : -150.0f);
        out->field_0x8.top = y + 0.0f;
        out->field_0x8.right = x + (wide ? 0.0f : -50.0f);
        out->field_0x8.bottom = y + 300.0f;
        if (out->field_0x30.data == g_unk_71727c81a0.data) {
            out->field_0x18.left = x + 0.0f;
            out->field_0x18.top = y + 0.0f;
            out->field_0x18.right = x + 400.0f;
            out->field_0x18.bottom = y + 300.0f;
        } else {
            out->field_0x18.left = x + 50.0f;
            out->field_0x18.top = y + 0.0f;
            out->field_0x18.right = x + 150.0f;
            out->field_0x18.bottom = y + 300.0f;
        }
    } else if (out->field_0x28.data == g_unk_71727c81d0.data) {
        f32 x = out->field_0x0;
        f32 y = out->field_0x4;
        out->field_0x8.left = x + -200.0f;
        out->field_0x8.top = y + -50.0f;
        out->field_0x8.right = x + 200.0f;
        out->field_0x8.bottom = y + 50.0f;
    } else {
        f32 x = out->field_0x0;
        f32 y = out->field_0x4;
        out->field_0x8.left = x + -100.0f;
        out->field_0x8.top = y;
        out->field_0x8.right = x + 100.0f;
        out->field_0x8.bottom = y + (out->field_0x28.data == g_unk_71727c81e0.data ? 300.0f : 400.0f);
    }
}
