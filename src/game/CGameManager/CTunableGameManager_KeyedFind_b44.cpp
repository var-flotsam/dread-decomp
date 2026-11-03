// CGameManager::CTunableGameManager's unit (tools/tu_map.py): whether a string id is a key of the keyed
// array of 0x28-byte values at 0x2450 (shaped like fn_7100e57fd0 in src/clones/). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "types.h"

struct UnkGameManagerKeyed_b44a3;

__attribute__((flatten)) bool fn_7100de9460(char* self, const base::UnkStrIdMember* key) {
    auto* a = reinterpret_cast<
        base::UnkKeyedValueLookup<UnkGameManagerKeyed_b44a3, UnkCloneBlock<UnkGameManagerKeyed_b44a3, 0x28>>*>(
        self + 0x2450);
    return a->Find(*key) != a->End();
}
