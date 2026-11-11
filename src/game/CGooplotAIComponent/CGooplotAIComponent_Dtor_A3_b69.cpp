// CGooplotAIComponent's unit (tools/tu_map.py unit 851): the component's destructor (vtable 0x7101a4d0c8).
// It empties the node owner at +0x14800 (base/UnkNodeOwner.h; nodes with the next link at +0x10 and nothing
// to destroy), destroys it (vtables 0x7101a4d898 and 0x7101a4d8b8), then the members at +0x14768
// (fn_710059688c) and +0x146e0 (fn_7100594370) and the base (fn_7100369380). Names are placeholders.
#include "base/UnkNodeOwner.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CGooplotAIComponent.h"
#include "types.h"

extern void* g_unk_7101a4d898[];
char* fn_710059688c(char* self);
void fn_7100594370(char* self);

__attribute__((always_inline)) inline void UnkDestroyNothing_A3_b69(UnkListNode<0x10>*) {}

using UnkGooplotNodes_A3_b69 = UnkNodeOwner<UnkListNode<0x10>, UnkDestroyNothing_A3_b69>;

void fn_7100599ad8(char* self) {
    *reinterpret_cast<void**>(self) = reinterpret_cast<char*>(g_unk_7101a4d0c8) + 0x10;
    auto* nodes = reinterpret_cast<UnkGooplotNodes_A3_b69*>(self + 0x14800);
    nodes->FreeNodes();
    nodes->field_0x8 = 0;
    nodes->field_0x10 = nullptr;
    nodes->field_0x18 = nullptr;
    nodes->Destroy(g_unk_7101a4d898, g_unk_7101a4d8b8);
    fn_710059688c(self + 0x14768);
    fn_7100594370(self + 0x146e0);
    fn_7100369380(self);
}
