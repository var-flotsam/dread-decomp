// Copies of fn_7100670a50 (src/game/CCharClassRodotukAIComponent/fn_7100670a50.cpp), which the binary
// repeats at other addresses: fn_7100670cfc.
// Move constructor of a keyed holder (UnkKeyedHolder.h) whose record is twelve plain words.
#include "../../game_5821e4/UnkKeyedHolder.h"
#include "game/UnkRecord_7100670a50.h"

using UnkHolder_7100670a50 = UnkKeyedHolder<u32, UnkRecord_7100670a50>;

void fn_7100670cfc(UnkHolder_7100670a50* self, UnkHolder_7100670a50* other) {
    new (self) UnkHolder_7100670a50(std::move(*other));
}

