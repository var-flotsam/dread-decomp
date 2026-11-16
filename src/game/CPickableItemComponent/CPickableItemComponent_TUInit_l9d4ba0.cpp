// CPickableItemComponent's static initialiser (unit 1493): the unit header, the component's id, its
// "Unveiled" and "PickedUp" properties, its registration, the ids the game manager lists the items
// under, three names and two sets of lists. Names and layouts are placeholders.
#include <new>

#include "base/UnkStrIdRegistry.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTUSymbols.h"
#include "types.h"

using base::global::CRntVector;

// The unit's data (its statics, merged into one block).
struct UnkPickableItemTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId items_key;        // 0x20: "PickableItems"
    CStrId powerups_key;     // 0x28: "HiddenPowerups"
    CStrId picked_up;        // 0x30
    CStrId item_complete;    // 0x38
    CStrId event_log_label;  // 0x40
    u8 pad_0x48[0xd8];
    UnkReflType type;  // 0x120
};
static_assert(__builtin_offsetof(UnkPickableItemTU, type) == 0x120, "layout");

// Three lists and a flag; the element types are unknown.
struct UnkPickListSet_l9d4ba0 {
    CRntVector<void*> lists[3];
    bool field_0x78;

    UnkPickListSet_l9d4ba0() : lists{CRntVector<void*>(0xf), CRntVector<void*>(0xf), CRntVector<void*>(0xf)} {
        field_0x78 = false;
    }
};

struct UnkPickLists_l9d4ba0 {
    UnkPickListSet_l9d4ba0 sets[2];
};

extern UnkPickableItemTU g_unk_717273ffa0 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_717273fe00;
extern UnkProperty g_unk_717273fe08;
extern UnkProperty g_unk_717273fe30;
extern char g_unk_717273fe58;
extern u8 g_unk_717273fe60[];
extern char g_unk_7101a89520[];  // the properties' vtable
void fn_71009d3f7c(UnkProperty* p);
void fn_71009ce4c0(void* p);
void fn_71009cef20(void* p);

void fn_71009d4ba0() {
    UnkPickableItemTU& tu = g_unk_717273ffa0;
    InitHeader(tu.header);
    new (&g_unk_717273fe00) CStrId("PICKABLE", true);
    fn_7100000250(fn_710000051c, &g_unk_717273fe00, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_717273fe08, reinterpret_cast<void**>(g_unk_7101a89520), g_unk_71726db788,
                            "Unveiled", refl_TypeOf<fn_710000d228>(), fn_71009d3f7c, fn_71009ce4c0);
    InitProperty(&g_unk_717273fe30, reinterpret_cast<void**>(g_unk_7101a89520), g_unk_71726db788, "PickedUp",
                 refl_TypeOf<fn_710000d228>(), fn_71009d3f7c, fn_71009ce4c0);
    fn_71009ce53c();
    fn_710009ec00(&g_unk_717273fe58, &tu.type);
    new (&tu.items_key) CStrId(fn_7100de8f78(CStrId("PickableItems", true)));
    fn_7100000250(fn_710000051c, &tu.items_key, &g_unk_7101a19000);
    new (&tu.powerups_key) CStrId(fn_7100de8f78(CStrId("HiddenPowerups", true)));
    fn_7100000250(fn_710000051c, &tu.powerups_key, &g_unk_7101a19000);
    new (&tu.picked_up) CStrId("PickedUp", true);
    fn_7100000250(fn_710000051c, &tu.picked_up, &g_unk_7101a19000);
    new (&tu.item_complete) CStrId("item_complete", true);
    fn_7100000250(fn_710000051c, &tu.item_complete, &g_unk_7101a19000);
    new (&tu.event_log_label) CStrId("Event log when all items are obtained", true);
    fn_7100000250(fn_710000051c, &tu.event_log_label, &g_unk_7101a19000);
    new (g_unk_717273fe60) UnkPickLists_l9d4ba0;
    fn_7100000250(fn_71009cef20, g_unk_717273fe60, &g_unk_7101a19000);
}
