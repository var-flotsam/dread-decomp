// CIngameMenu (unit 1681): the menu's GUI event handler. A state-index event switches the menu to the
// index its property bag carries; the others open a state or pass the event to a virtual handler.
// Names and layouts are placeholders.
#include <algorithm>

#include "../../../game_s7/UnkIter.h"
#include "../../../game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CIngameMenu.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkTU56.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using unk_prop_bag::UnkPropBag;

// The event as the handler reads it.
struct UnkMenuEvent_lb30198 {
    u8 unk_0x0[0x8];
    s32 kind;
    u8 unk_0xc[0xe];
    bool handled;
    u8 unk_0x1b[0x5];
    u64 field_0x20;
};
static_assert(sizeof(UnkMenuEvent_lb30198) == 0x28, "UnkMenuEvent_lb30198");

// A value in the bag: where it is in the bag's stream, the hash of its name and its type.
struct UnkBagValue_lb30198 {
    u64 offset;
    u64 key;
    s32 type;  // 'i' for an integer
};

struct UnkIngameMenu_lb30198 {
    struct VTable {
        void* unk_0x0[34];
        void (*vf_0x110)(UnkIngameMenu_lb30198*, UnkMenuEvent_lb30198*);
        void (*vf_0x118)(UnkIngameMenu_lb30198*, UnkMenuEvent_lb30198*);
        void* unk_0x120[2];
        void (*vf_0x130)(UnkIngameMenu_lb30198*, UnkMenuEvent_lb30198*);
        void (*vf_0x138)(UnkIngameMenu_lb30198*, UnkMenuEvent_lb30198*);
    };
    VTable* vtable;
    u8 unk_0x8[0x179];
    bool active;  // 0x181
    u8 unk_0x182[0x15a];
    bool field_0x2dc;
    u8 unk_0x2dd[0x3];
    s32 state;  // 0x2e0
    s32 field_0x2e4;
};
static_assert(__builtin_offsetof(UnkIngameMenu_lb30198, field_0x2e4) == 0x2e4, "UnkIngameMenu_lb30198");

static inline UnkBagValue_lb30198* Values_lb30198(UnkPropBag* bag) {
    return reinterpret_cast<UnkBagValue_lb30198*>(bag->values.data);
}

// The value whose name hashes to `key`, or the end of the values when there is none.
static inline UnkBagValue_lb30198* FindValue_lb30198(UnkPropBag* bag, u64 key) {
    UnkBagValue_lb30198* values = Values_lb30198(bag);
    UnkIter<UnkBagValue_lb30198> first{values};
    UnkIter<UnkBagValue_lb30198> last{&values[bag->values.count]};
    return std::find_if(first, last, [key](const UnkBagValue_lb30198& v) { return v.key == key; }).p;
}

static inline UnkStateOwner* Owner(UnkIngameMenu_lb30198* self) {
    return reinterpret_cast<UnkStateOwner*>(self);
}

void CIngameMenu_DispatchGuiEvent(UnkIngameMenu_lb30198* self, UnkMenuEvent_lb30198* ev) {
    if (ev->handled || !self->active || ev->field_0x20 == 1)
        return;

    switch (ev->kind) {
    case 4: {
        s32 index = 0;
        UnkPropBag* bag = fn_71012024f4(reinterpret_cast<UnkGuiEvent*>(ev));
        UnkBagValue_lb30198* it = FindValue_lb30198(bag, StringCrc64("StateIdx", 8));
        if (it != &Values_lb30198(bag)[bag->values.count] && it->type == 'i') {
            CRntFile_Seek(&bag->lock, it->offset, 0);
            fn_710007c70c(&bag->lock, &index, 4);
        }
        fn_7100b2eaac(Owner(self), index, self->state == 1, true);
        ev->handled = true;
        break;
    }
    case 18: {
        s32 state = self->field_0x2e4;
        if (state < 0 || state > 9)
            return;
        fn_7100b2eaac(Owner(self), state, state == 1, true);
        if (!self->field_0x2dc)
            fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f01b0), self);
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
    case 33:
        self->vtable->vf_0x110(self, ev);
        ev->handled = true;
        break;
    case 34:
        self->vtable->vf_0x118(self, ev);
        ev->handled = true;
        break;
    }
}
