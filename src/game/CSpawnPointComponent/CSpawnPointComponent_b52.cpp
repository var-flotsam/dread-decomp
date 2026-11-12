// CSpawnPointComponent unit (tools/tu_map.py, the name from reflection). Names and layouts are placeholders.
#include "base/UnkResKey.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/operator_new.h"
#include "types.h"

struct UnkActorDefFactory;

// A counted reference like UnkResRef (base/UnkResKey.h) whose destructor reads the pointer once.
struct UnkSpawnDefRef_b52 {
    UnkCountedResource* ptr;

    UnkSpawnDefRef_b52& operator=(const UnkSpawnDefRef_b52& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkSpawnDefRef_b52() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

struct UnkSpawnDefHolder_b52 {
    u8 pad_0x0[0x48];
    base::global::CStrId type;  // 0x48
    UnkSpawnDefRef_b52 def;     // 0x50
};

// Loads the actor definition `type` names (under the default allocator lock) when its id is set.
void fn_710112b96c(UnkSpawnDefHolder_b52* self) {
    if (self->type.data == &g_unk_7101d47108->empty)
        return;
    fn_71000839c0(true);
    self->def = fn_71011938bc(g_unk_71726a0220, &self->type, &g_unk_7101d470e8);
    fn_7100083a10();
}
