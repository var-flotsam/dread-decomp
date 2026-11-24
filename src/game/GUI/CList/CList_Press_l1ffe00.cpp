// GUI::CList unit: an entry of the list was pressed. Unless the event is already handled, selects the
// entry the event comes from (by name), sends the list's handlers a kind-0x11 event naming the entry and
// "ActivationButton", and marks the event handled. Names and layouts are placeholders.
#include "../../../game_s7/UnkPropBagS7.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkTU1674.h"
#include "types.h"

using base::global::CStrId;

struct UnkListItem_l1ffe00 {
    void** vtable;
    u8 pad_0x8[0x128];
    CStrId name;  // 0x130
};

// Where an event comes from: the entry at +0x10.
struct UnkListEventSource_l1ffe00 {
    u8 pad_0x0[0x10];
    UnkListItem_l1ffe00* item;  // 0x10
};

struct UnkListEvent_l1ffe00 {
    UnkListEventSource_l1ffe00* source;
    u8 pad_0x8[0x12];
    bool handled;  // 0x1a
};

struct UnkListNode_l1ffe00 {
    UnkListItem_l1ffe00* item;
    UnkListNode_l1ffe00* next;
};

struct UnkList_l1ffe00 {
    void** vtable;
    u8 pad_0x8[0xb0];
    u8 handlers[0x110];  // 0xb8
    u32 count;           // 0x1c8
    u8 pad_0x1cc[0x4];
    UnkListNode_l1ffe00* head;  // 0x1d0
    u8 pad_0x1d8[0x6c];
    s32 selected;  // 0x244
    s32 previous;  // 0x248

    // Selects the entry whose name is `name`.
    void SelectByName(CStrId name) {
        s32 i = 0;
        for (UnkListNode_l1ffe00* n = head; n; n = n->next, ++i) {
            if (n->item->name.data == name.data) {
                previous = -1;
                selected = i;
                break;
            }
        }
    }
};
static_assert(__builtin_offsetof(UnkList_l1ffe00, count) == 0x1c8, "count");
static_assert(__builtin_offsetof(UnkList_l1ffe00, previous) == 0x248, "previous");

struct UnkGuiDisplayObject;
struct UnkGuiEventData_ovb0182;

// The id table's id at `index`.
inline const CStrId& TableId(u32 index) {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[index];
}

// An id's text, counting the read.
inline const char* Text(const CStrId& id) {
    ++id.data->reads;
    const char* text = id.data->text;
    return text ? text : g_unk_710162e2ed;
}

void fn_71011cf704(UnkList_l1ffe00* self, UnkListEvent_l1ffe00* event);

void CList_SelectEventSourceItemAndSendActivation(UnkList_l1ffe00* self, UnkListEvent_l1ffe00* event) {
    fn_71011cf704(self, event);
    if (self->count == 0 || event->handled)
        return;
    UnkListItem_l1ffe00* item = event->source->item;
    self->SelectByName(item->name);
    unk_prop_bag::UnkPropBag bag;
    unk_prop_bag::fn_710050e4dc(&bag, TableId(0x3ec), Text(item->name));
    {
        const CStrId& key = TableId(0xcb);
        base::UnkNamedEntry button;
        CRntString_ConstructFromCString(&button, "ActivationButton");
        const char* text = reinterpret_cast<base::global::CRntString*>(&button)->data;
        unk_prop_bag::fn_710050e4dc(&bag, key, text ? text : g_unk_710162e2ed);
        CRntString_Destroy(reinterpret_cast<base::global::CRntString*>(&button));
    }
    s32 kind = 0x11;
    UnkGuiEvent ev;
    fn_710120235c(&ev, reinterpret_cast<UnkGuiDisplayObject*>(self), &kind, &bag);
    DispatchEventToHandlersForKey(self->handlers, reinterpret_cast<UnkGuiEventData_ovb0182*>(&ev));
    event->handled = true;
    fn_7101202488(&ev);
}
