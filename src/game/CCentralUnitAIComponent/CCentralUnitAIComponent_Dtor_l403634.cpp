// CCentralUnitAIComponent's unit (tools/tu_map.py unit 614): the destructor. Its vtable, the two
// callback lists at +0x14790 and +0x14750, the render item pair at +0x14738, the array of polymorphic
// entries at +0x146d0, then the base AI component's destructor. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU441.h"
#include "types.h"

struct UnkItemBase_71011a91c4;
struct UnkRenderItemPair_b64a2 {
    UnkItemBase_71011a91c4* first;
    UnkItemBase_71011a91c4* second;
};

struct UnkCentralUnitCallbacks_l403634;
template <>
struct base::UnkCallbackTraits<UnkCentralUnitCallbacks_l403634> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkCentralUnitCallbackList_l403634 = base::UnkCallbackListShort<UnkCentralUnitCallbacks_l403634>;

struct UnkCentralUnitEntry_l403634 {
    virtual ~UnkCentralUnitEntry_l403634();
    u8 pad_0x8[0x38];
};

struct UnkCentralUnitAI_l403634 {
    void** vtable;
    u8 pad_0x8[0x146c8];
    base::global::CRntVector<UnkCentralUnitEntry_l403634> entries;  // 0x146d0
    u8 pad_0x146f8[0x40];
    UnkRenderItemPair_b64a2 items;                // 0x14738
    u8 pad_0x14748[0x8];
    UnkCentralUnitCallbackList_l403634 list_a;    // 0x14750
    UnkCentralUnitCallbackList_l403634 list_b;    // 0x14790
};
static_assert(__builtin_offsetof(UnkCentralUnitAI_l403634, list_b) == 0x14790, "layout");

void fn_7100403634(UnkCentralUnitAI_l403634* self) {
    self->vtable = g_unk_7101a337d8 + 2;
    self->list_b.~UnkCentralUnitCallbackList_l403634();
    self->list_a.~UnkCentralUnitCallbackList_l403634();
    fn_710035d4ec(&self->items);
    self->entries.Free();
    fn_7100369380(self);
}
