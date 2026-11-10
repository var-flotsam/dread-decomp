// querysystem::CQuerySystemDef's unit (tools/tu_map.py): the destructor body of an object holding two
// lists of owned objects (0x8 and 0x30). Names and layouts are placeholders.
#include "game_s2/UnkOwnedList.h"
#include "types.h"

extern void* g_unk_7101a21518[];

struct UnkQueryLists_ovb0120 {
    void* vtable;
    UnkOwnedList first;  // 0x8
    UnkOwnedList second;  // 0x30

    __attribute__((always_inline)) ~UnkQueryLists_ovb0120() { vtable = &g_unk_7101a21518[2]; }
};
static_assert(sizeof(UnkQueryLists_ovb0120) == 0x58, "UnkQueryLists_ovb0120 layout");

void fn_71002c6cdc(UnkQueryLists_ovb0120* self) { self->~UnkQueryLists_ovb0120(); }
