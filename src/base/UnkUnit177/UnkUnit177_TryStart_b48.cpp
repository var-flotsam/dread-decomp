// Unit 177 (tools/tu_map.py): when both singletons (fn_7100162980, fn_7100154470) exist and both
// are active (+0x1c and +0x10), runs fn_7100161250 and fn_7100159ce0 and returns true. Names
// and layouts are placeholders.
#include "game/UnkTU174.h"
#include "types.h"

struct UnkUnit182Singleton {
    u8 pad_0x0[0x1c];
    u32 active;  // 0x1c
};

struct UnkUnit174Singleton {
    u8 pad_0x0[0x10];
    u32 active;  // 0x10
};

UnkUnit182Singleton* fn_7100162980();
void fn_7100161250();
void fn_7100159ce0();

bool fn_7100159c80() {
    UnkUnit182Singleton* a = fn_7100162980();
    UnkUnit174Singleton* b = fn_7100154470();
    if (a == nullptr || b == nullptr)
        return false;
    if (a->active == 0 || b->active == 0)
        return false;
    fn_7100161250();
    fn_7100159ce0();
    return true;
}
