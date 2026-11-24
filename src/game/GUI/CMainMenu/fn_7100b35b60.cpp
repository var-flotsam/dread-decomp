// CMainMenu (unit 1687): the menu's GUI event handler. A state-index event switches the menu to the index its
// property bag carries; a confirm opens the selected state when it allows it; two kinds go to virtual handlers;
// and on the main composition a list selection sets the slot options' mode, switching to state 5 when it is new.
// Names and layouts are placeholders.
#include <algorithm>

#include "../../../game_s7/UnkIter.h"
#include "../../../game_s7/UnkPropBagS7.h"
#include "UnkClone.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CMainMenu.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkTU56.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct UnkClone_fn_7100b34974;
using UnkMenuStateSlot_lb35b60 = UnkCloneElem<UnkClone_fn_7100b34974, 8>;
extern template UnkMenuStateSlot_lb35b60& base::global::CRntVector<UnkMenuStateSlot_lb35b60>::GrowAt(u32);

// As src/game/GUI/CSlotOptionsComposition/CSlotOptionsComposition_SetMode_A5_b69.cpp defines it.
struct UnkSlotOptions_A5_b69;
void fn_7100b7c648(UnkSlotOptions_A5_b69* self, u32 mode);
bool fn_7100b7c598(UnkSlotOptions_A5_b69* self, u32 mode);

// The event as the handler reads it.
struct UnkMenuEvent_lb35b60 {
    u8 unk_0x0[0x8];
    s32 kind;
    u8 unk_0xc[0xe];
    bool handled;
    u8 unk_0x1b[0x5];
    u64 field_0x20;
};
static_assert(sizeof(UnkMenuEvent_lb35b60) == 0x28, "UnkMenuEvent_lb35b60");

// A value in the bag: where it is in the bag's stream, the hash of its name and its type.
struct UnkBagValue_lb35b60 {
    u64 offset;
    u64 key;
    s32 type;  // 'i' for an integer
};

// A menu state: whether it can be opened now (vf_0x30).
struct UnkMenuState_lb35b60 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool CanOpen();
};

// A display object by the id at +0x130.
struct UnkNamedObject_lb35b60 {
    u8 pad_0x0[0x130];
    CStrId name;  // 0x130
};

// The list the main composition holds: its selected entry.
struct UnkMenuList_lb35b60 : UnkGuiDisplayObject {
    u8 pad_0x198[0x244 - 0x198];
    u32 selected;  // 0x244
};

struct UnkMenuLock_lb35b60 {
    u8 pad_0x0[0xb4];
    bool locked;  // 0xb4
};

struct UnkMainMenu_lb35b60 {
    struct VTable {
        void* unk_0x0[38];
        void (*vf_0x130)(UnkMainMenu_lb35b60*, UnkMenuEvent_lb35b60*);
        void (*vf_0x138)(UnkMainMenu_lb35b60*, UnkMenuEvent_lb35b60*);
    };
    VTable* vtable;
    u8 unk_0x8[0x179];
    bool active;  // 0x181
    u8 unk_0x182[0x268 - 0x182];
    UnkGuiDisplayObject* composition;  // 0x268
    u8 unk_0x270[0x278 - 0x270];
    u32 selectedSlot;  // 0x278
    u32 selected;  // 0x27c
    u8 unk_0x280[0x2b8 - 0x280];
    base::global::CRntVector<UnkMenuStateSlot_lb35b60> states;  // 0x2b8
    u8 unk_0x2e0[0x3b8 - 0x2e0];
    UnkGuiDisplayObject* slotOptions;  // 0x3b8
    u8 unk_0x3c0[0x540 - 0x3c0];
    UnkMenuLock_lb35b60* lock;  // 0x540
};
static_assert(__builtin_offsetof(UnkMainMenu_lb35b60, lock) == 0x540, "UnkMainMenu_lb35b60");

static inline UnkBagValue_lb35b60* Values_lb35b60(UnkPropBag* bag) {
    return reinterpret_cast<UnkBagValue_lb35b60*>(bag->values.data);
}

// The value whose name hashes to `key`, or the end of the values when there is none.
static inline UnkBagValue_lb35b60* FindValue_lb35b60(UnkPropBag* bag, u64 key) {
    UnkBagValue_lb35b60* values = Values_lb35b60(bag);
    UnkIter<UnkBagValue_lb35b60> first{values};
    UnkIter<UnkBagValue_lb35b60> last{&values[bag->values.count]};
    return std::find_if(first, last, [key](const UnkBagValue_lb35b60& v) { return v.key == key; }).p;
}

static inline base::reflection::UnkReflObject* Owner(UnkMainMenu_lb35b60* self) {
    return reinterpret_cast<base::reflection::UnkReflObject*>(self);
}

void CMainMenu_HandleGuiEvent(UnkMainMenu_lb35b60* self, UnkMenuEvent_lb35b60* ev) {
    if (ev->handled || !self->active || ev->field_0x20 == 1 || self->lock->locked)
        return;

    switch (ev->kind) {
    case 4: {
        s32 index = 0;
        UnkPropBag* bag = fn_71012024f4(reinterpret_cast<UnkGuiEvent*>(ev));
        UnkBagValue_lb35b60* it = FindValue_lb35b60(bag, StringCrc64("StateIdx", 8));
        if (it != &Values_lb35b60(bag)[bag->values.count] && it->type == 'i') {
            CRntFile_Seek(&bag->lock, it->offset, 0);
            fn_710007c70c(&bag->lock, &index, 4);
        }
        fn_7100b35704(Owner(self), index, true);
        ev->handled = true;
        break;
    }
    case 18: {
        if (self->selected > 13)
            return;
        UnkMenuState_lb35b60* state =
            *reinterpret_cast<UnkMenuState_lb35b60**>(&self->states.GrowAt(self->selectedSlot));
        if (!state->CanOpen())
            return;
        fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f01b0), self->composition);
        fn_7100b35704(Owner(self), self->selected, true);
        ev->handled = true;
        break;
    }
    case 19:
        self->vtable->vf_0x130(self, ev);
        ev->handled = true;
        break;
    case 20:
        self->vtable->vf_0x138(self, ev);
        ev->handled = true;
        break;
    case 34: {
        UnkNamedObject_lb35b60* composition = reinterpret_cast<UnkNamedObject_lb35b60*>(self->composition);
        if (!(composition->name == CStrId("mainmenucomposition", true)))
            return;
        UnkGuiDisplayObject* found = fn_71011d9508(self->composition, &self->composition->field_0x190);
        if (!found || !fn_710009d50c(found->GetClass(), fn_71011fe904()))
            return;
        UnkGuiDisplayObject* slotOptions = self->slotOptions;
        if (!slotOptions || !fn_710009d50c(slotOptions->GetClass(), fn_7100b7bc24()))
            return;
        UnkMenuList_lb35b60* list = static_cast<UnkMenuList_lb35b60*>(found);
        UnkSlotOptions_A5_b69* options = reinterpret_cast<UnkSlotOptions_A5_b69*>(slotOptions);
        if (!fn_7100b7c598(options, list->selected)) {
            fn_7100b7c648(options, list->selected);
            fn_7100b35704(Owner(self), 5, true);
        }
        ev->handled = true;
        break;
    }
    }
}
