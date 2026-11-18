// The map menu's unit (tools/tu_map.py unit 1689, hint GUI::CMapMenuController): the confirm event of a map
// page. With a selected record, toggles its id in the page's list of shown ids (shown again: the list is
// rebuilt from it; already shown: the list is cleared); otherwise clears the list. Then closes the page.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1689.h"
#include "game/UnkTU2566.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;

struct UnkEnumType;
extern UnkEnumType g_unk_71729f01b8;
extern UnkEnumType g_unk_71729f01c0;

struct UnkMapPage_lb45014;

// A menu event: what happened and whether a handler took it.
struct UnkMenuEvent_lb45014 {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
    u8 pad_0x1b[0x5];
    u64 kind;  // 0x20
};

// One record of the map's record list (0x578 bytes): its id is held by pointer, from an offset that is only
// 4-aligned.
struct __attribute__((packed)) UnkMapRecord_lb45014 {
    u32 field_0x0;
    const CStrId* id;  // 0x4
    u8 pad_0xc[0x56c];
};
static_assert(sizeof(UnkMapRecord_lb45014) == 0x578, "UnkMapRecord_lb45014 layout");

struct UnkStateRef_lb45014 {
    u8 pad_0x0[0x10];
    void* state;  // 0x10
};

struct UnkMapMenuPending_lb45014 {
    u8 pad_0x0[0x10];
    s32 entry;  // 0x10
};

struct UnkMapPage_lb45014 {
    u8 pad_0x0[0xb4];
    bool active;  // 0xb4
    u8 pad_0xb5[0x2d3];
    UnkStateRef_lb45014* sound_state;  // 0x388
    u8 pad_0x390[0x118];
    CStrId record_set;  // 0x4a8
    u32 record;  // 0x4b0
    u8 pad_0x4b4[0x1c];
    CRntString title;  // 0x4d0
    u8 pad_0x4d8[0x8b8 - 0x4d0 - sizeof(CRntString)];
    CRntVector<CStrId> ids;  // 0x8b8
    CStrId shown;  // 0x8e0
    u8 pad_0x8e8[0x1];
    bool busy;  // 0x8e9
    u8 pad_0x8ea[0x3e];
    UnkMapMenuPending_lb45014* pending;  // 0x928
    u8 pad_0x930[0x8c];
    s32 close_entry;  // 0x9bc
};
static_assert(__builtin_offsetof(UnkMapPage_lb45014, ids) == 0x8b8, "UnkMapPage_lb45014 layout");
static_assert(__builtin_offsetof(UnkMapPage_lb45014, pending) == 0x928, "UnkMapPage_lb45014 layout");
static_assert(__builtin_offsetof(UnkMapPage_lb45014, close_entry) == 0x9bc, "UnkMapPage_lb45014 layout");

void fn_71011cf758(UnkMapPage_lb45014* self, UnkMenuEvent_lb45014* event);
bool fn_7100b41e84(UnkMapPage_lb45014* self);
void* fn_7100e94428(void* world, base::global::CStrId* id);
void fn_7100ea6304(UnkMapData* map, CStrId* id);
void fn_7100ea6234(UnkMapData* map, CStrId* id);
CStrId fn_7100e68f44(const CStrId* id, CRntVector<CStrId>* ids);

static inline bool Contains(CRntVector<CStrId>& ids, const CStrId& id) {
    for (CStrId& e : ids)
        if (e == id)
            return true;
    return false;
}

// Takes every shown id off the map, then empties the list.
static inline void ClearIds(UnkMapPage_lb45014* self) {
    for (CStrId& e : self->ids)
        fn_7100ea6304(fn_7100e8d8b0(), &e);
    self->ids.Clear();
}

static inline void PlaySound(UnkMapPage_lb45014* self, UnkEnumType* event) {
    UnkStateSink* sink = g_unk_717275c380;
    void* state = self->sound_state ? self->sound_state->state : nullptr;
    fn_7100fc9d78(sink, reinterpret_cast<UnkStateKey*>(event), state);
}

void fn_7100b45014(UnkMapPage_lb45014* self, UnkMenuEvent_lb45014* event) {
    fn_71011cf758(self, event);
    if (!self->active || event->kind != 2)
        return;
    if (self->busy) {
        event->handled = true;
        return;
    }
    if (fn_7100b41e84(self)) {
        UnkMapRecord_lb45014* records = static_cast<UnkMapRecord_lb45014*>(
            fn_7100e94428(g_unk_7101d47310->field_0x2228, &self->record_set));
        CStrId id(*(records ? records[self->record].id : &CStrId::Empty()));
        if (Contains(self->ids, id)) {
            self->shown.Reset();
            ClearIds(self);
        } else {
            ClearIds(self);
            self->shown = fn_7100e68f44(&id, &self->ids);
            for (CStrId& e : self->ids)
                fn_7100ea6234(fn_7100e8d8b0(), &e);
            PlaySound(self, &g_unk_71729f01b8);
        }
        event->handled = true;
    } else if (self->ids.count) {
        ClearIds(self);
        self->shown.Reset();
        fn_710007eaa0(&self->title, &g_unk_7101d58d10);
        PlaySound(self, &g_unk_71729f01c0);
    }
    s32 entry = self->close_entry;
    if (entry == 0x7fffffff)
        entry = self->pending ? self->pending->entry : 0x7fffffff;
    fn_7100b3e5ac(reinterpret_cast<UnkMapMenuControllerClose*>(self), entry, 0);
}
