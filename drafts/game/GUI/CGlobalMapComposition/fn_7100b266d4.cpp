// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): the composition's event handler. On an event of
// kind 2 the Samus ship marker shows or hides by the profile's "ShowSamusShip" flag, the map is refreshed, a
// pending area mode falls back to mode 6 when its marker cannot be selected, and the composition's state machine
// changes to the mode's state. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CGlobalMapComposition.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

extern UnkPropertyEx<> g_unk_7172759378;  // "ShowSamusShip"

void fn_7100b26ba8(UnkGlobalMapComposition* self);
void fn_7100b27160(UnkGlobalMapComposition* self);

// The event the handler is given: its kind and whether it was handled.
struct UnkMapEvent_lb266d4 {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
    u8 pad_0x1b[0x5];
    u64 kind;  // 0x20
};

// An area marker on the map.
struct UnkMapMarker_lb266d4 {
    u8 pad_0x0[0xb5];
    bool selectable;  // 0xb5
};

struct UnkMapArea_lb266d4 {
    u8 pad_0x0[0x50];
    UnkMapMarker_lb266d4* marker;  // 0x50
};

// The areas by name: parallel arrays of areas and their names.
struct UnkMapAreas_lb266d4 {
    UnkMapArea_lb266d4* values;
    CStrId* keys;
    u32 count;

    UnkMapArea_lb266d4* begin() const { return values; }
    UnkMapArea_lb266d4* end() const { return values + count; }

    // The area named `key`, or end() when there is none.
    UnkMapArea_lb266d4* At(const CStrId& key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return &values[count];
    }
    UnkMapArea_lb266d4* Find(CStrId key) const { return At(key) != end() ? At(key) : nullptr; }
};

// A state of the composition's state machine.
struct UnkMapState_lb266d4 {
    void** vtable;
    u8 pad_0x8[0x8];
    s32 id;  // 0x10

    void Enter(s32 previous) {
        reinterpret_cast<void (*)(UnkMapState_lb266d4*, s32)>(vtable[0x10 / 8])(this, previous);
    }
    void Exit() { reinterpret_cast<void (*)(UnkMapState_lb266d4*)>(vtable[0x18 / 8])(this); }
};

// The state machine: the current and previous states and the states it holds.
struct __attribute__((packed, aligned(4))) UnkMapStateMachine_lb266d4 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkMapState_lb266d4* current;     // 0x10
    UnkMapState_lb266d4* previous;    // 0x18
    u32 count;                        // 0x20
    UnkMapState_lb266d4* states[16];  // 0x24
    s32 requested;                    // 0xa4
    bool locked;                      // 0xa8

    void OnEntered(UnkMapState_lb266d4* from, UnkMapState_lb266d4* to) {
        reinterpret_cast<void (*)(UnkMapStateMachine_lb266d4*, UnkMapState_lb266d4*, UnkMapState_lb266d4*)>(
            vtable[0x20 / 8])(this, from, to);
    }
    void OnExited(UnkMapState_lb266d4* from, UnkMapState_lb266d4* to) {
        reinterpret_cast<void (*)(UnkMapStateMachine_lb266d4*, UnkMapState_lb266d4*, UnkMapState_lb266d4*)>(
            vtable[0x28 / 8])(this, from, to);
    }

    void ChangeState(s32 id) {
        if (current && current->id == id)
            return;
        UnkMapState_lb266d4* next = nullptr;
        for (UnkMapState_lb266d4** it = states; it != states + count; ++it) {
            if ((*it)->id == id) {
                next = *it;
                break;
            }
        }
        if (!next && id != 0)
            return;
        if (current) {
            current->Exit();
            OnExited(current, next);
        }
        previous = current;
        current = next;
        if (next) {
            next->Enter(previous ? previous->id : 0x7fffffff);
            OnEntered(previous, next);
        }
    }
};

struct UnkGlobalMapView_lb266d4 {
    UnkGuiDisplayObject base;
    u8 pad_0x198[0x238 - sizeof(UnkGuiDisplayObject)];
    UnkMapStateMachine_lb266d4 states;  // 0x238
    u8 pad_0x2e4[0x588 - 0x2e4];
    s32 mode;  // 0x588
    u8 pad_0x58c[0x5b0 - 0x58c];
    UnkMapAreas_lb266d4 areas;  // 0x5b0
    u8 pad_0x5c8[0x5d8 - 0x5c8];
    UnkGuiDisplayObject* samus_ship;  // 0x5d8
    u8 pad_0x5e0[0x700 - 0x5e0];
    CStrId area_0x700;  // 0x700
    u8 pad_0x708[0x790 - 0x708];
    CStrId area_0x790;  // 0x790
};
static_assert(sizeof(UnkMapStateMachine_lb266d4) == 0xac, "layout");

// Asks for the state `id`: it is entered at once unless the machine is locked.
static inline void RequestState(UnkMapStateMachine_lb266d4& machine, s32 id) {
    machine.requested = id;
    if (((u32)id <= 6) & !machine.locked) {
        machine.ChangeState(id);
        machine.requested = 0x7fffffff;
    }
}
static_assert(__builtin_offsetof(UnkGlobalMapView_lb266d4, samus_ship) == 0x5d8, "layout");
static_assert(__builtin_offsetof(UnkGlobalMapView_lb266d4, area_0x790) == 0x790, "layout");

// Plays an event of a display object's animation (vtable slot 0x1e8).
static inline void PlayEvent(UnkGuiDisplayObject* object, const char* event, bool flag) {
    (*reinterpret_cast<void (***)(UnkGuiDisplayObject*, const char*, bool)>(object))[0x1e8 / 8](object, event, flag);
}

// vtable slot 0x78.
static inline void SetDepth(UnkGuiDisplayObject* object, s32 depth, bool flag) {
    (*reinterpret_cast<void (***)(UnkGuiDisplayObject*, s32, bool)>(object))[0x78 / 8](object, depth, flag);
}

// The property's qualified id (game/UnkGameBlackboard.h), built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = reinterpret_cast<UnkQualifiedId&>(item.field_0x10);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, reinterpret_cast<UnkIdSource*>(&item), base::global::CStrId::Empty());
    return qid;
}

void fn_7100b266d4(UnkGuiDisplayObject* object, UnkMapEvent_lb266d4* event) {
    UnkGlobalMapView_lb266d4* self = reinterpret_cast<UnkGlobalMapView_lb266d4*>(object);
    fn_71011d99e4(object, reinterpret_cast<UnkGuiEvent_b49*>(event));
    if (event->kind != 2)
        return;

    CBlackboard* profile = &g_unk_7101d47310->field_0x968;
    bool show_ship = false;
    {
        const CStrId& section = GameManagerIdTable().field_0x1150;
        const UnkQualifiedId& name = Qid(g_unk_7172759378);
        if (CBlackboard::CSection* s = fn_7101047270(profile, section))
            fn_710006d11c(s, &name.id, &show_ship);
    }
    if (self->samus_ship) {
        if (show_ship) {
            PlayEvent(self->samus_ship, "SamusShip_OnShow", false);
            SetDepth(self->samus_ship, -50, true);
        } else {
            PlayEvent(self->samus_ship, "SamusShip_OnHide", false);
        }
    }
    fn_7100b26ba8(reinterpret_cast<UnkGlobalMapComposition*>(self));
    fn_7100b27160(reinterpret_cast<UnkGlobalMapComposition*>(self));

    s32 mode = self->mode;
    if (mode == 3) {
        UnkMapArea_lb266d4* area = self->areas.Find(self->area_0x790);
        mode = self->mode = area->marker->selectable ? 3 : 6;
    } else if (mode == 2) {
        UnkMapArea_lb266d4* area = self->areas.Find(self->area_0x700);
        mode = self->mode = area->marker->selectable ? 2 : 6;
    }
    RequestState(self->states, mode);
    self->mode = 1;
    fn_71011cd5a0(object);
    event->handled = true;
}
