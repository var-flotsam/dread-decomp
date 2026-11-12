// CDamageComponent's unit (tools/tu_map.py unit 1209): reads one entry from a text stream: two kind names,
// each turned into its kind (fn_7100d4bc00) when present, and a string id. The parameter types are those
// of src/clones/clones_02.cpp's declaration (the object and the stream). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2507.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStrId;

// As src/game/CAnimationComponent/CAnimationComponent_Tracks_ovb0009.cpp and
// src/game/CCollisionComponent/fn_71008626bc.cpp declare them.

struct UnkDamageEntry_ovb0122 {
    u8 pad_0x0[0x40];
    UnkStrId name;  // 0x40
    u32 kind_a;     // 0x48
    u32 kind_b;     // 0x4c
};

bool fn_71007aed6c(char* obj, const void* arg) {
    UnkDamageEntry_ovb0122* self = reinterpret_cast<UnkDamageEntry_ovb0122*>(obj);
    char a[0x80];
    char b[0x80];
    char c[0x80];
    fn_710007cf30(const_cast<void*>(arg), "sss", a, b, c);
    if (a[0]) {
        CRntString text(a);
        self->kind_a = fn_7100d4bc00(&text);
    }
    if (b[0]) {
        CRntString text(b);
        self->kind_b = fn_7100d4bc00(&text);
    }
    if (c[0])
        self->name = UnkStrId(c, true);
    return true;
}
