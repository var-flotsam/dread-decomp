// From unit 280 of tools/tu_map.py (0x71001fb9ac-0x71001fe698); same shape as fn_71006fa84c
// (src/game/UnkUnit1115/UnkUnit1115_b35.cpp): calls fn_71002027a8 on every entry of the pointer array
// at +0x0 (count at +0x8) with the same argument. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit280Entry;
void fn_71002027a8(UnkUnit280Entry* entry, void* arg);

struct UnkUnit280List {
    UnkUnit280Entry** entries;
    u32 count;
};

void fn_71001fceb4(UnkUnit280List* self, void* arg) {
    for (UnkUnit280Entry **it = self->entries, **end = it + self->count; it != end; ++it)
        fn_71002027a8(*it, arg);
}
