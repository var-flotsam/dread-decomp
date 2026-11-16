// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): counts the composition's timer down and,
// when it runs out, opens the map (state 1: sound, show the area and map containers, then state 2) or
// requests the next state of its state machine (state 2). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkRenderManager.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkStateKey;
struct UnkEnumType;
extern UnkEnumType g_unk_71729f0238;
__attribute__((visibility("hidden"))) extern const f32 g_unk_7172762e5c[2];

// The id the lookup returns: starts as the pool's empty entry (making the pool if there is none yet)
// and is released like a CStrId.
struct UnkMapNameId {
    UnkStringData* data;

    UnkMapNameId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkMapNameId() { reinterpret_cast<CStrId*>(this)->Reset(); }
    // Releases the string held and takes the one `id` holds, leaving `id` empty.
    UnkMapNameId& operator=(CStrId&& id) {
        reinterpret_cast<CStrId*>(this)->ReleaseAndTake(id);
        return *this;
    }
};

UnkMapNameId fn_7100b22e50(const CStrId& scenario);

struct UnkMapState {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previous);
    virtual void Leave(s32 next);
    u64 field_0x8;
    u32 id;
};

typedef UnkMapState* __attribute__((aligned(4))) UnkMapStatePtr;

struct UnkMapStateMachine {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEntered(UnkMapState* previous, UnkMapState* next);
    virtual void OnLeft(UnkMapState* current, UnkMapState* next);

    u64 field_0x8;
    UnkMapState* current;
    UnkMapState* previous;
    u32 count;
    UnkMapStatePtr states[16];
    u32 pending;
    bool busy;

    // Moves to state `id` unless a switch is under way.
    void Request(u32 id) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != id) {
            UnkMapState* next = nullptr;
            UnkMapStatePtr* state = states;
            for (u64 size = count * sizeof(UnkMapStatePtr); size != 0; size -= sizeof(UnkMapStatePtr), ++state) {
                if ((*state)->id == id) {
                    next = *state;
                    break;
                }
            }
            if (next) {
                if (current) {
                    current->Leave(id);
                    OnLeft(current, next);
                }
                previous = current;
                current = next;
                next->Enter(previous ? previous->id : 0x7fffffff);
                OnEntered(previous, next);
            }
        }
        pending = 0x7fffffff;
    }
};

struct UnkGlobalMapView {
    u8 pad_0x0[0x238];
    UnkMapStateMachine machine;  // 0x238
    u8 pad_0x2e8[0x58c - 0x2e8];
    s32 state;    // 0x58c
    f32 timer;    // 0x590
    u8 pad_0x594[0x5e0 - 0x594];
    UnkGuiDisplayObject* root;  // 0x5e0
    u8 pad_0x5e8[0x6f8 - 0x5e8];
    CStrId area_name;  // 0x6f8
    CStrId scenario;   // 0x700
    u8 pad_0x708[0x790 - 0x708];
    CStrId next_scenario;  // 0x790
};
static_assert(__builtin_offsetof(UnkGlobalMapView, state) == 0x58c, "layout");
static_assert(__builtin_offsetof(UnkGlobalMapView, root) == 0x5e0, "layout");
static_assert(__builtin_offsetof(UnkGlobalMapView, next_scenario) == 0x790, "layout");

// `object` if it is a display-object container, else null.
inline UnkGuiDisplayObject* GlobalMapContainer(UnkGuiDisplayObject* object) {
    if (!object)
        return nullptr;
    return fn_710009d50c(object->GetClass(), fn_71011d7b84()) ? object : nullptr;
}

void fn_7100b27e10(UnkGlobalMapView* self, f32 dt) {
    bool elapsed = false;
    if (self->timer > 0.0f) {
        self->timer -= dt;
        elapsed = self->timer <= 0.0f;
    }
    switch (self->state) {
    case 1: {
        if (!elapsed)
            break;
        fn_7100219294(g_unk_7101d47300, 0xe936254db735c615ull, g_unk_7172762e5c, 2);
        fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0238), self);
        UnkGuiDisplayObject* area = GlobalMapContainer(fn_71011d98ec(self->root, self->area_name.data->Key()));
        UnkGuiDisplayObject* map;
        if (self->scenario.data != &g_unk_7101d47108->empty) {
            UnkMapNameId name = fn_7100b22e50(self->scenario);
            map = GlobalMapContainer(fn_71011d98ec(self->root, name.data->Key()));
        } else {
            UnkMapNameId name = fn_7100b22e50(self->next_scenario);
            map = GlobalMapContainer(fn_71011d98ec(self->root, name.data->Key()));
        }
        if (area)
            fn_71011cdc30(area, true);
        if (map)
            fn_71011cdc30(map, true);
        self->state = 2;
        self->timer = 1.5f;
        break;
    }
    case 2:
        if (elapsed) {
            bool empty = self->next_scenario.data == &g_unk_7101d47108->empty;
            self->machine.Request(empty ? 2 : 3);
        }
        break;
    }
}
