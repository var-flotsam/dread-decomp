// CArenaManager::CTunableArenaManager's unit (tools/tu_map.py): frees the vector at 0x8 of 0x80-byte
// entries with an out-of-line destructor (a copy of fn_7100ad1f48 in src/clones/, with its own destructor).
// Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "types.h"

void fn_7100e167d4(char*);

struct UnkArenaEntry_b44a3;
template <>
struct UnkCloneDtorTraits<UnkArenaEntry_b44a3> {
    static void destroy(char* p) { fn_7100e167d4(p); }
};

using UnkArenaEntries_b44a3 = base::UnkVector<UnkCloneDtorElem<UnkArenaEntry_b44a3, 0x80>>;

__attribute__((flatten)) void fn_7100e1480c(char* self) {
    reinterpret_cast<UnkArenaEntries_b44a3*>(self + 0x8)->Free();
}
