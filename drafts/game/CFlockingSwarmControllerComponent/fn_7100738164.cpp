// CFlockingSwarmControllerComponent unit: resizes one of the swarm's base::global::CRntVector lists of
// 12-byte entries (copied word by word, new ones zeroed) to `count` entries.
// Names and layouts are placeholders.
#include <cstring>
#include "base/global/CRntVector.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkWord_l738164;

// A 3-float vector, copied word by word; value-initialised ones are zeroed.
struct UnkSwarmEntry12_l738164 {
    f32 x, y, z;

    UnkSwarmEntry12_l738164() = default;
    UnkSwarmEntry12_l738164(const UnkSwarmEntry12_l738164& o) {
        ((UnkWord_l738164*)this)[0] = ((const UnkWord_l738164*)&o)[0];
        ((UnkWord_l738164*)this)[1] = ((const UnkWord_l738164*)&o)[1];
        ((UnkWord_l738164*)this)[2] = ((const UnkWord_l738164*)&o)[2];
    }
};
static_assert(sizeof(UnkSwarmEntry12_l738164) == 0xc, "layout");

void fn_7100738164(void* list, u32 count) {
    static_cast<base::global::CRntVector<UnkSwarmEntry12_l738164>*>(list)->Resize(count);
}
