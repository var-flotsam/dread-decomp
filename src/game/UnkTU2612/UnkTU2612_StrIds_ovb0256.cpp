// Unit 2612 of tools/tu_map.py (no name hint): string ids reset to the empty id (one element, and nine in a
// row), each as a move from a freshly built empty id. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdInline;

CStrId* fn_7100fb19c8(CStrId* begin, CStrId* pos) {
    u64 bytes = reinterpret_cast<char*>(pos) - reinterpret_cast<char*>(begin);
    u32 index = (u32)(bytes >> 3);
    CStrId empty(UnkStrIdInline(), nullptr, false);
    CStrId* element = begin + index;
    element->ReleaseAndTake(empty);
    return element;
}

void fn_7100fb1888(CStrId* ids) {
    for (int i = 0; i < 9; ++i)
    {
        CStrId empty(UnkStrIdInline(), nullptr, false);
        ids[i].ReleaseAndTake(empty);
    }
}
