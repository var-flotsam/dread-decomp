// Unit 368: tests whether an object's query kind matches the first kind selected in a bit mask.
// Names and layouts are placeholders.
#include "types.h"
#include "game/CBreakableTileGroup.h"

#define KIND_CASE(bit, kind)                         \
    if (mask & (1u << (bit))) {                      \
        if (fn_7100c9e960(obj) == (kind))            \
            return 1;                                \
    } else

u32 fn_71002b1270(UnkQueryObj* obj, u32 mask) {
    if (obj != nullptr) {
        KIND_CASE(2, 1)
        KIND_CASE(3, 2)
        KIND_CASE(4, 3)
        KIND_CASE(5, 4)
        KIND_CASE(6, 5)
        KIND_CASE(7, 6)
        KIND_CASE(8, 7)
        KIND_CASE(9, 8)
        KIND_CASE(10, 9) {}
    }
    return 0;
}
