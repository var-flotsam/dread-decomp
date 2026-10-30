// Unit 2761 (tools/tu_map.py: 0x7101202300-0x710120261c): the GUI event senders build on the stack
// (game/UnkGuiEvent.h). Its destructor is here; names and layouts are placeholders.
#include <new>

#include "../../game_s7/UnkPropBagS7.h"
#include "game/UnkGuiEvent.h"
#include "game_s2/UnkWeakHandle.h"

using unk_prop_bag::UnkPropBag;

struct UnkGuiEventView {
    UnkWeakHandle* handle;      // the display object's weak handle
    s32 kind;                   // 0x8
    u8 unk_0xc[0x4];
    UnkPropBag* bag;  // 0x10
    bool owns_bag;              // 0x18
    bool field_0x19;
    bool field_0x1a;
    u8 unk_0x1b[0x5];
    u64 field_0x20;
};

// Takes the object's weak handle (created on first use; none when the object has no slot), the kind
// and the bag (not owned).
void fn_710120235c(UnkGuiEvent* self, UnkGuiDisplayObject* object, const s32* kind, unk_prop_bag::UnkPropBag* bag) {
    UnkGuiEventView* view = reinterpret_cast<UnkGuiEventView*>(self);
    UnkWeakHandle* handle = GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(object));
    view->handle = nullptr;
    if (handle) {
        ++handle->ref_count;
        if (UnkWeakHandle* old = view->handle) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        view->handle = handle;
    }
    view->kind = *kind;
    view->bag = bag;
    view->owns_bag = false;
    view->field_0x19 = true;
    view->field_0x1a = false;
    view->field_0x20 = 2;
}

// Frees the bag when the event owns it, then drops the handle.
void fn_7101202488(UnkGuiEvent* self) {
    UnkGuiEventView* view = reinterpret_cast<UnkGuiEventView*>(self);
    if (view->owns_bag) {
        if (UnkPropBag* bag = view->bag) {
            unk_prop_bag::fn_710050e8e4(bag);
            operator delete(bag);
            view->bag = nullptr;
        }
    }
    if (UnkWeakHandle* handle = view->handle) {
        if (--handle->ref_count == 0)
            handle->vf_0x10();
    }
}
