// Unit 361 of tools/tu_map.py (base/UnkIdKindKey.h): the hash of an id and kind pair, the kind in bits
// 16-23 above the id. Names are placeholders.
#include "types.h"

u32 fn_71002874e0(u16 a, u8 b) {
    return (static_cast<u32>(b) << 16) | a;
}
