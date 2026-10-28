// Move constructor of a keyed holder (UnkKeyedHolder.h) whose record is twelve plain words.
#include "../../game_5821e4/UnkKeyedHolder.h"

struct UnkRecord_7100670a50 {
    u32 field_0x0[12];
};

using UnkHolder_7100670a50 = UnkKeyedHolder<u32, UnkRecord_7100670a50>;

void fn_7100670a50(UnkHolder_7100670a50* self, UnkHolder_7100670a50* other) {
    new (self) UnkHolder_7100670a50(std::move(*other));
}
