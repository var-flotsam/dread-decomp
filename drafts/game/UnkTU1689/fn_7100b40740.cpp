// The map menu's unit (tools/tu_map.py unit 1689, hint GUI::CMapMenuController): the controller's per-frame
// update: steps its state machine (at most ten state changes a frame), updates its parts, and keeps its two
// looping sounds playing, the first following the map's zoom, the second the scroll speed. Names and layouts
// are placeholders.
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2566.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

struct UnkEnumType;
extern UnkEnumType g_unk_71729f0200;
extern UnkEnumType g_unk_71729f0208;
extern f32 g_unk_7172763d10 __attribute__((visibility("hidden")));  // the scroll sound's base volume

struct UnkMapMenu_lb40740;
void fn_7100b40434(UnkMapMenu_lb40740* self, bool instant, f32 dt);
void fn_7100b40f2c(UnkMapMenu_lb40740* self, bool instant, f32 dt);

static const s32 kNoState = 0x7fffffff;

struct UnkMapState_lb40740 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Enter(s32 previous);
    virtual void Exit(s32 next);
    virtual void Update(f32 dt);
    u8 pad_0x8[0x8];
    s32 id;  // 0x10
};

// The states, held packed: the count and then the states, from an offset that is only 4-aligned.
struct __attribute__((packed)) UnkMapStateList_lb40740 {
    u32 count;
    UnkMapState_lb40740* items[16];
};

struct UnkMapStateMachine_lb40740 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void OnEnter(UnkMapState_lb40740* previous, UnkMapState_lb40740* state);
    virtual void OnExit(UnkMapState_lb40740* current, UnkMapState_lb40740* state);
    u8 pad_0x8[0x8];
    UnkMapState_lb40740* current;   // 0x10
    UnkMapState_lb40740* previous;  // 0x18
    UnkMapStateList_lb40740 states;  // 0x20
    s32 next;  // 0xa4
    bool updating;  // 0xa8

    UnkMapState_lb40740* Find(s32 id) {
        UnkMapState_lb40740** end = states.items + states.count;
        for (UnkMapState_lb40740** it = states.items; it != end; ++it)
            if ((*it)->id == id)
                return *it;
        return nullptr;
    }

    void SetState(s32 id) {
        if (current && current->id == id)
            return;
        UnkMapState_lb40740* state = Find(id);
        bool entering = state != nullptr;
        if (!entering && id != 0)
            return;
        if (current) {
            current->Exit(id);
            OnExit(current, state);
        }
        previous = current;
        current = state;
        if (entering) {
            state->Enter(previous ? previous->id : kNoState);
            OnEnter(previous, state);
        }
    }

    void ApplyNext() {
        if (next >= 0 && next < 5) {
            SetState(next);
            next = kNoState;
        }
    }
};

struct UnkMapMenuPart_lb40740 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void Update(f32 dt);
};

struct UnkSoundVoice_lb40740 {
    u8 pad_0x0[0xec];
    f32 volume;  // 0xec
};

struct UnkSoundRef_lb40740 {
    UnkSoundVoice_lb40740* sound;
    s64 serial;
};

struct UnkMapSound_lb40740 {
    UnkSoundRef_lb40740* ref;
    s64 serial;

    bool Playing() const { return ref && ref->serial == serial && serial != -1 && ref->sound; }
    UnkSoundVoice_lb40740* Get() const { return ref && ref->serial == serial && serial != -1 ? ref->sound : nullptr; }
    UnkPlayingSound_ovb0074* Playback() const { return reinterpret_cast<UnkPlayingSound_ovb0074*>(Get()); }
    UnkInterp_7100105d34* Interp() const { return reinterpret_cast<UnkInterp_7100105d34*>(Get()); }
    void Play(UnkEnumType* event, void* owner) {
        UnkSoundHandle handle = fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(event), owner);
        ref = reinterpret_cast<UnkSoundRef_lb40740*>(handle.ref);
        serial = handle.serial;
    }
};

struct UnkMapData_lb40740 {
    u8 pad_0x0[0xbc];
    f32 zoom;  // 0xbc
};

struct UnkMapMenu_lb40740 {
    u8 pad_0x0[0xb4];
    bool active;  // 0xb4
    u8 pad_0xb5[0x17c];
    bool fast;  // 0x231
    u8 pad_0x232[0x296];
    f32 scroll_speed;  // 0x4c8
    u8 pad_0x4cc[0x44c];
    UnkMapStateMachine_lb40740 machine;  // 0x918
    u8 pad_0x9c8[0x7e0];
    UnkMapMenuPart_lb40740* part;  // 0x11a8
    UnkMapSound_lb40740 zoom_sound;  // 0x11b0
    UnkMapSound_lb40740 scroll_sound;  // 0x11c0
    f32 last_zoom;  // 0x11d0
};
static_assert(__builtin_offsetof(UnkMapMenu_lb40740, machine) == 0x918, "UnkMapMenu_lb40740 layout");
static_assert(__builtin_offsetof(UnkMapMenu_lb40740, part) == 0x11a8, "UnkMapMenu_lb40740 layout");
static_assert(__builtin_offsetof(UnkMapMenu_lb40740, last_zoom) == 0x11d0, "UnkMapMenu_lb40740 layout");

void fn_7100b40740(UnkMapMenu_lb40740* self, f32 dt) {
    fn_71011d8fe4(reinterpret_cast<UnkNewGameScreen*>(self), dt);
    if (!self->active)
        return;
    // Steps the state machine: at most ten state changes a frame.
    UnkMapStateMachine_lb40740& machine = self->machine;
    machine.updating = true;
    machine.ApplyNext();
    for (s32 i = 0; i < 10 && machine.current; ++i) {
        machine.current->Update(dt);
        if (machine.next == kNoState)
            break;
        machine.ApplyNext();
    }
    machine.updating = false;
    if (self->part)
        self->part->Update(dt);
    fn_7100b40434(self, false, dt);
    fn_7100b40f2c(self, false, dt);

    if (!self->zoom_sound.Playing())
        self->zoom_sound.Play(&g_unk_71729f0200, self);
    if (self->zoom_sound.Playing()) {
        f32 zoom = reinterpret_cast<UnkMapData_lb40740*>(fn_7100e8d8b0())->zoom;
        if (zoom == self->last_zoom) {
            fn_7100105bac(self->zoom_sound.Playback(), 0.03f);
        } else {
            fn_7100105c00(self->zoom_sound.Playback(), 0.2f);
            SetSoundVoicePitchOverDuration(self->zoom_sound.Interp(), zoom > self->last_zoom ? 0.9f : 1.0f, 0.2f);
            self->last_zoom = zoom;
        }
    }

    if (!self->scroll_sound.Playing()) {
        self->scroll_sound.Play(&g_unk_71729f0208, self);
        g_unk_7172763d10 = self->scroll_sound.Playing() ? self->scroll_sound.Get()->volume : 0.0f;
    }
    if (self->scroll_sound.Playing()) {
        if (self->scroll_speed > 0.16f) {
            fn_7100105c00(self->scroll_sound.Playback(), 0.0f);
            fn_7100105d34(self->scroll_sound.Interp(),
                          self->fast ? g_unk_7172763d10 : g_unk_7172763d10 * 0.8f, false, 0.2f);
            SetSoundVoicePitchOverDuration(self->scroll_sound.Interp(), self->fast ? 1.0f : self->scroll_speed * 0.1f + 0.8f,
                          0.0f);
        } else {
            fn_7100105bac(self->scroll_sound.Playback(), 0.1f);
        }
    }
}
