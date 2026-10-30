// Unit 2669 of tools/tu_map.py (0x710109d670-0x710109e534, no class hint): a chained hash map
// (base/UnkChainedMap.h) followed by the block pool its nodes come from (base/UnkCloneHelpers.h).
// The constructor builds both and sizes them to 11 buckets and 256 nodes; the game manager's
// constructors build several. Names are placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/UnkCloneHelpers.h"

struct UnkPooledChainedMap {
    UnkChainedMap map;
    base::UnkBlockPool pool;  // 0x48
};
static_assert(__builtin_offsetof(UnkPooledChainedMap, pool) == 0x48, "UnkPooledChainedMap layout");

// Sizes the buckets and the node pool (base::refl_HashMapInit, a generated clone in src/clones/).
void fn_710109e290(UnkPooledChainedMap* self, u32 buckets, u32 nodes);

void fn_710109d810(UnkPooledChainedMap* self) {
    new (&self->map) UnkChainedMap();
    self->pool.Init();
    self->pool.field_0x38 = 0;
    self->pool.field_0x3c = false;
    fn_710109e290(self, 0xb, 0x100);
}
