// CBossAIComponent's unit (tools/tu_map.py): the destructor. Its vtable, then its members in reverse:
// two callback lists with a string id between them, the render item pair, a small fixed list, three
// more callback lists, two string ids and the array of polymorphic entries; then the base AI
// component's destructor. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CBossAIComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU441.h"
#include "types.h"

struct UnkItemBase_71011a91c4;
struct UnkRenderItemPair_b64a2 {
    UnkItemBase_71011a91c4* first;
    UnkItemBase_71011a91c4* second;
};

struct UnkBossCallbacks_l3ac388;
template <>
struct base::UnkCallbackTraits<UnkBossCallbacks_l3ac388> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkBossCallbackList_l3ac388 = base::UnkCallbackListShort<UnkBossCallbacks_l3ac388>;

struct UnkBossEntry_l3ac388 {
    virtual ~UnkBossEntry_l3ac388();
    u8 pad_0x8[0x38];
};

// A fixed-capacity list whose elements need no destruction: clearing it only resets the count.
struct UnkBossFixedList_l3ac388 {
    u8 items[0x2c];
    u32 count;
    ~UnkBossFixedList_l3ac388() { count = 0; }
};

struct UnkBossAI_l3ac388 {
    void** vtable;
    u8 pad_0x8[0x146c0];
    base::global::CRntVector<UnkBossEntry_l3ac388> entries;  // 0x146c8
    base::UnkStrIdMember id_a;                               // 0x146f0
    base::UnkStrIdMember id_b;                               // 0x146f8
    u8 pad_0x14700[0x28];
    UnkBossCallbackList_l3ac388 list_c;  // 0x14728
    UnkBossCallbackList_l3ac388 list_d;  // 0x14768
    UnkBossCallbackList_l3ac388 list_e;  // 0x147a8
    UnkBossFixedList_l3ac388 fixed;      // 0x147e8
    u8 pad_0x14818[0xd8];
    UnkRenderItemPair_b64a2 items;       // 0x148f0
    u8 pad_0x14900[0x10];
    UnkBossCallbackList_l3ac388 list_a;  // 0x14910
    base::UnkStrIdMember id_c;           // 0x14950
    UnkBossCallbackList_l3ac388 list_b;  // 0x14958
};
static_assert(__builtin_offsetof(UnkBossAI_l3ac388, fixed) == 0x147e8, "layout");
static_assert(__builtin_offsetof(UnkBossAI_l3ac388, items) == 0x148f0, "layout");
static_assert(__builtin_offsetof(UnkBossAI_l3ac388, list_b) == 0x14958, "layout");

// Derived classes call it as their base destructor with their own object (game/CBossAIComponent.h).
void fn_71003ac388(void* object) {
    UnkBossAI_l3ac388* self = static_cast<UnkBossAI_l3ac388*>(object);
    self->vtable = g_unk_7101a29438 + 2;
    self->list_b.~UnkBossCallbackList_l3ac388();
    self->id_c.~UnkStrIdMemberOf();
    self->list_a.~UnkBossCallbackList_l3ac388();
    fn_710035d4ec(&self->items);
    self->fixed.~UnkBossFixedList_l3ac388();
    self->list_e.~UnkBossCallbackList_l3ac388();
    self->list_d.~UnkBossCallbackList_l3ac388();
    self->list_c.~UnkBossCallbackList_l3ac388();
    self->id_b.~UnkStrIdMemberOf();
    self->id_a.~UnkStrIdMemberOf();
    self->entries.Free();
    fn_7100369380(self);
}
