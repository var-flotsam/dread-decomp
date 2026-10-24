// Copy-assignment of two ids and a 16-byte value after a field that is not copied (as in
// fn_7100c8f87c, src/game_aca57c/fn_7100d3d424.cpp).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkValue_0x10 {
    u64 words[2];
};

struct UnkIdPairCopy_71002e1ccc {
    void* field_0x0;
    CStrId first;
    CStrId second;
    UnkValue_0x10 field_0x18;
};

UnkIdPairCopy_71002e1ccc* fn_71002e1ccc(UnkIdPairCopy_71002e1ccc* self, const UnkIdPairCopy_71002e1ccc* other) {
    if (self != other) {
        self->first = other->first;
        self->second = other->second;
        self->field_0x18 = other->field_0x18;
    }
    return self;
}
