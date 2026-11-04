// Unit 2517 of tools/tu_map.py (a box shape, see fn_7100d69570.cpp): loads its two corners from a file, each read
// as one 8-byte element unless the file is at its end (then both stay zero), and stores the segment built from
// them at +0xa8. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

UnkSegment2D* fn_7100125b44(UnkSegment2D*, const UnkSegment2D*);

struct UnkBoxShape2D_A6_b72 {
    u8 pad_0x0[0xa8];
    UnkSegment2D bounds;
};

bool fn_7100d69428(UnkBoxShape2D_A6_b72* self, base::global::CRntFile* file) {
    base::math::CVector2D a = {};
    base::math::CVector2D b = {};
    if (file->position < file->size) {
        fn_710007c714(file, &a, 8, 1);
        fn_710007c714(file, &b, 8, 1);
    }
    UnkSegment2D seg(a, b);
    fn_7100125b44(&self->bounds, &seg);
    return true;
}
