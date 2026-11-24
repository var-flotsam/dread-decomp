// CBarelyFrozenIceInfo's unit: copy constructors of the record of two string ids and a scale (the
// record of fn_71007f1e6c in CBarelyFrozenIceInfo_Ctor_b44.cpp); two identical copies. Names are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CBarelyFrozenIceInfo.h"
#include "types.h"

#include <new>

using base::global::CStrId;

struct UnkFrozenIceRecord_A5_b65 {
    void* vtable;
    CStrId sModel;
    CStrId sNode;
    f32 fScale;

    UnkFrozenIceRecord_A5_b65(const UnkFrozenIceRecord_A5_b65& other)
        : vtable(reinterpret_cast<char*>(g_unk_7101a73e10) + 0x10), sModel(other.sModel),
          sNode(other.sNode), fScale(other.fScale) {}
};

void fn_71007f1edc(UnkFrozenIceRecord_A5_b65* self, const UnkFrozenIceRecord_A5_b65* other) {
    new (self) UnkFrozenIceRecord_A5_b65(*other);
}

void fn_71007f1f40(UnkFrozenIceRecord_A5_b65* self, const UnkFrozenIceRecord_A5_b65* other) {
    new (self) UnkFrozenIceRecord_A5_b65(*other);
}
