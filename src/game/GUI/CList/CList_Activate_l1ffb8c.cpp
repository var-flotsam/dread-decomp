// GUI::CList unit: activating the list. Unless the event is already handled, hands it to the selected
// entry (playing the selection sound when the list is not quiet) and, if the entry leaves it unhandled,
// sends the list's handlers a kind-0x11 event naming the entry and "ActivationButton"; then marks the
// event handled. Names and layouts are placeholders.
#include "../../../game_s7/UnkPropBagS7.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/GUI/CList.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU1674.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CStrId;

struct UnkListEvent_l1ffb8c {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
};

struct UnkListItem_l1ffb8c {
    void** vtable;
    u8 pad_0x8[0x128];
    CStrId name;  // 0x130

    void vf_0xf0(UnkListEvent_l1ffb8c* event) {
        reinterpret_cast<void (*)(UnkListItem_l1ffb8c*, UnkListEvent_l1ffb8c*)>(vtable[0xf0 / 8])(this, event);
    }
};

struct UnkListNode_l1ffb8c {
    UnkListItem_l1ffb8c* item;
    UnkListNode_l1ffb8c* next;
};

struct UnkList_l1ffb8c {
    void** vtable;
    u8 pad_0x8[0xb0];
    u8 handlers[0x110];  // 0xb8
    u32 count;           // 0x1c8
    u8 pad_0x1cc[0x4];
    UnkListNode_l1ffb8c* head;  // 0x1d0
    u8 pad_0x1d8[0x6c];
    s32 selected;  // 0x244
    u8 pad_0x248[0x18];
    bool quiet;  // 0x260

    // The node at `index`, or null past the end.
    UnkListNode_l1ffb8c* NodeAt(u32 index) {
        u32 n = count;
        if (index < n) {
            UnkListNode_l1ffb8c* const* link = &head;
            for (;;) {
                UnkListNode_l1ffb8c* node = *link;
                if (index == 0)
                    return node;
                link = &node->next;
                --n;
                --index;
                if (n == 0)
                    break;
            }
        }
        return nullptr;
    }
};
static_assert(__builtin_offsetof(UnkList_l1ffb8c, count) == 0x1c8, "count");
static_assert(__builtin_offsetof(UnkList_l1ffb8c, quiet) == 0x260, "quiet");

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

void CList_ActivateSelectedItem(UnkList_l1ffb8c* self, UnkListEvent_l1ffb8c* event) {
    fn_71011cf70c(reinterpret_cast<UnkGallery_A5_b75*>(self));
    if (self->count == 0 || event->handled)
        return;
    if (UnkListNode_l1ffb8c* node = self->NodeAt(self->selected)) {
        UnkListItem_l1ffb8c* item = node->item;
        if (!self->quiet)
            fn_710002fd10(&g_unk_717271e8f8->field_0xca8, &g_unk_7172a10df0);
        item->vf_0xf0(event);
        if (!event->handled) {
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
            fn_7100b1e3fc(self->handlers, reinterpret_cast<UnkGuiEventData_ovb0182*>(&ev));
            fn_7101202488(&ev);
        }
    }
    event->handled = true;
}
