// CInputComponent (tools/tu_map.py unit 1326): the destructor. Unregisters the member at +0x40 from the
// item list singleton, empties and frees the three arrays at +0xc70 (the first one's entries point to
// slots that are cleared), destroys the members from +0xbc0 down to +0x40, then the component base.
// Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/UnkSList.h"
#include "base/UnkVtableList.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CInputComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU2732.h"
#include "game/UnkTU39.h"
#include "types.h"

extern void* g_unk_7101a7a568[];
extern void* g_unk_7101a7a710[];
extern char g_unk_7101a7a730[];
struct UnkOuterList_A4_b75;
void fn_710086f394(UnkOuterList_A4_b75* self);

struct UnkInputActionTag_l871dd4;
using UnkInputActionNode_l871dd4 = base::UnkSListNode<UnkInputActionTag_l871dd4>;

// A named table whose entries are a plain array.
struct UnkInputTable_l871dd4 {
    void** vtable;
    base::UnkNamedEntry name;  // 0x8
    u64 capacity;              // 0x38
    u8* entries;               // 0x40
};

struct UnkInputComponent_l871dd4 {
    void** vtable;
    u8 pad_0x8[0x38];
    u8 item[0x9d0];                                             // 0x40
    UnkAbilityInputListener listener;                           // 0xa10
    UnkInputTable_l871dd4 table;                                // 0xa58
    base::UnkVtableList<UnkInputActionNode_l871dd4> actions;   // 0xaa0
    u8 field_0xad0[0x40];
    u8 field_0xb10[0xb0];
    u8 field_0xbc0[0xb0];
    base::global::CRntVector<void**> lists[3];  // 0xc70
};
static_assert(__builtin_offsetof(UnkInputComponent_l871dd4, actions) == 0xaa0, "layout");
static_assert(__builtin_offsetof(UnkInputComponent_l871dd4, lists) == 0xc70, "layout");

void fn_7100871dd4(UnkInputComponent_l871dd4* self) {
    self->vtable = &g_unk_7101a7a568[2];
    if (!g_unk_7172729910) {
        void* p = fn_71000839a0(0x2208, 0x3f);
        fn_710119aee8(p);
        g_unk_7172729910 = p;
    }
    fn_710119ae24(g_unk_7172729910, self->item, true);
    base::global::CRntVector<void**>& links = self->lists[0];
    for (void*** it = links.data, ***end = links.data + links.count; it != end; ++it)
        **it = nullptr;
    for (base::global::CRntVector<void**>& list : self->lists)
        list.Clear();
    self->lists[2].Free();
    self->lists[1].Free();
    self->lists[0].Free();
    fn_710002f67c(self->field_0xbc0);
    fn_710002f67c(self->field_0xb10);
    fn_710086f394(reinterpret_cast<UnkOuterList_A4_b75*>(self->field_0xad0));
    self->actions.vtable = g_unk_7101a7a730 + 0x10;
    self->actions.Clear();
    self->table.vtable = &g_unk_7101a7a710[2];
    if (self->table.entries) {
        fn_7100083c30(self->table.entries);
        self->table.entries = nullptr;
    }
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&self->table.name));
    self->listener.vtable = &g_unk_7101a22878[2];
    if (self->listener.entries) {
        fn_7100083c30(static_cast<u64*>(self->listener.entries) - 1);
        self->listener.entries = nullptr;
    }
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&self->listener.name));
    fn_710119ad3c(self->item);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
