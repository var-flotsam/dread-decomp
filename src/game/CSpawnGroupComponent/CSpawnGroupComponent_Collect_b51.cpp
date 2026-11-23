// From unit 2703 of tools/tu_map.py (CSpawnGroupComponent): fills a temporary list (vtable
// g_unk_7101a32680, nodes from the default allocator with flags 0x13) through fn_710111b230 and
// frees it again. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkList_71011199cc.h"
#include "game/UnkList_71011199ccNode.h"
#include "game/UnkTU604.h"
#include "types.h"
#include "base/UnkVtableList.h"

struct UnkComponent_71011199cc;
void fn_710111b230(UnkComponent_71011199cc* self, UnkList_71011199cc* out);

void fn_71011199cc(UnkComponent_71011199cc* self) {
    UnkList_71011199cc list;
    fn_710111b230(self, &list);
}
