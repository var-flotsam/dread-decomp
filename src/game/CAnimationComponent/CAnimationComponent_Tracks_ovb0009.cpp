// CAnimationComponent's unit (tools/tu_map.py): the component's track queries and requests. The
// component's tracks are the object at +0x40 (game/UnkAnimationComponent.h): every track at +0xc8,
// the tracks now playing at +0xf0. Requests to a track go through fn_7101177044 with a mode, a key
// and time, a blend descriptor and a guarded all-bits mask (the statics of
// src/game_s2/UnkSpitSend.h). The listener lists at +0x438 are CAnimationComponent_b52.cpp's.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkVector.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CActorComponent.h"
#include "game/CAnimationComponent.h"
#include "game/CComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU1134.h"
#include "game/UnkTU1643.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU56.h"
#include "game/UnkTU83.h"
#include "game/UnkXformNode.h"
#include "game/animtree/CAnimTreeElement.h"
#include "game/animtree/CAnimatedNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::refl_ArgValue;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

// ---- Shared declarations --------------------------------------------------------------------

struct UnkStruct_7101176ea8;
struct UnkAnimActionSetsOwner;
struct UnkAnimTiming;
struct UnkCutsceneTimeline;
struct UnkObj_710117af10;
struct UnkCls_710117af10;
struct UnkTrackBlend;
struct UnkTrackAction_1444;

bool fn_7100c97930(UnkTrackAction_1444* action, const base::reflection::CClass* cls, f32 value);

extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT

// A request's mode (passed by value, through memory).
struct UnkFlags_7101176ea8 {
    u32 value;
    UnkFlags_7101176ea8(u32 v) : value(v) {}
    UnkFlags_7101176ea8(const UnkFlags_7101176ea8& o) : value(o.value) {}
    UnkFlags_7101176ea8& operator=(const UnkFlags_7101176ea8& o) {
        value = o.value;
        return *this;
    }
};

// A time on an animation's clock (src/game/UnkAnimTime.cpp): unit 1 is seconds.
struct UnkAnimTime_ovb0009 {
    f32 value;
    s32 unit;
};

// Built out of line in the original (returned in a register), so it is stored as one word.
static inline UnkAnimTime_ovb0009 Seconds(f32 value) {
    UnkAnimTime_ovb0009 t;
    t.value = value;
    t.unit = 1;
    return t;
}

// A key and a time.
struct UnkS16 {
    u64 key;
    UnkAnimTime_ovb0009 time;
};

// A blend descriptor (g_unk_7172769740, g_unk_7172769770).
struct UnkS12 {
    u32 a, b, c;
};

static inline const UnkS16& DefaultTime() { return reinterpret_cast<const UnkS16&>(g_unk_710164b968); }
static inline const UnkS12& DefaultBlend() { return reinterpret_cast<const UnkS12&>(g_unk_7172769740); }
static inline const UnkS12& SyncBlend() { return reinterpret_cast<const UnkS12&>(g_unk_7172769770); }

bool fn_7101177044(UnkStruct_7101176ea8* self, void* r, s32 idx, void* data, UnkFlags_7101176ea8 flags,
                   const UnkS16& a, const UnkS12& b, UnkFlags_7101176ea8 c);

// The track of the entry whose name hash is `key`, or null.
static inline UnkTrack* FindTrack(UnkAnimationComponent* self, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return nullptr;
    UnkTrackEntry* entry = *it;
    if (!entry)
        return nullptr;
    return entry->field_0x70;
}

// The same as the untyped data fn_7101177044 is given.
static inline void* TrackData(UnkAnimationComponent* self, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    return (it != set->field_0xc8.End() && *it) ? (*it)->field_0x70 : nullptr;
}

// The class name of a track's action, as fn_710070d7f8 gives it: its hash is at +0x30.
struct UnkActionName_ovb0009 {
    u8 pad_0x0[0x30];
    u64 hash;
};

static inline u64 TrackName(UnkObj_710117af10* track) {
    return reinterpret_cast<UnkActionName_ovb0009*>(*fn_710070d7f8(track))->hash;
}

// The name hash of the actor's main track.
static const u64 kMainTrack = 0xf4061ff25a2163d6;

// ---- Requests -------------------------------------------------------------------------------

// What fn_710070df40 and fn_710070dffc are given: the action, its mode, a key and time, a blend,
// a variant index and a mask; the defaults are the global time and blend and the all-bits mask.
struct UnkAnimRequest {
    void* action;
    UnkFlags_7101176ea8 flags;
    UnkS16 a;
    UnkS12 b;
    s32 idx;
    UnkFlags_7101176ea8 mask;

    UnkAnimRequest()
        : action(nullptr), flags(0), a(DefaultTime()), b(DefaultBlend()), idx(-1), mask(UnkAll_SpitSend().value) {}
};

bool fn_710070df40(UnkTrack* track, UnkAnimRequest* request);
bool fn_710070dffc(UnkTrack* track, UnkAnimRequest* request);

struct UnkAnimComponent_ovb0009 {
    u8 pad_0x0[0x40];
    UnkTrackSet* tracks;
    u8 pad_0x48[0x472];
    bool field_0x4ba;  // requests are refused while set
};

// Sends a request to `data`'s track: refused while the component's flag is set or without a
// track; bit 0x2000 is added to the mode unless fn_710024fbc8 holds.
bool fn_7101177044(UnkStruct_7101176ea8* self, void* r, s32 idx, void* data, UnkFlags_7101176ea8 flags,
                   const UnkS16& a, const UnkS12& b, UnkFlags_7101176ea8 c) {
    UnkAnimComponent_ovb0009* component = reinterpret_cast<UnkAnimComponent_ovb0009*>(self);
    if (component->field_0x4ba)
        return false;
    UnkAnimRequest request;
    request.action = r;
    request.flags = flags;
    request.a = a;
    request.b = b;
    request.idx = idx;
    request.mask = c;
    if (!fn_710024fbc8(component))
        request.flags.value |= 0x2000;
    UnkTrack* track = static_cast<UnkTrack*>(data);
    if (!track || !fn_710070df40(track, &request))
        return false;
    return fn_710070dffc(track, &request);
}

// The action `id` names, with the mode at `flags`, to the track whose name hash is `key`, through
// fn_710070df40 alone.
bool fn_7101178394(UnkAnimComponent_ovb0009* self, u64 key, void* id, const u32* flags) {
    if (self->field_0x4ba)
        return false;
    void* action = fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(self), id);
    UnkAnimRequest request;
    request.action = action;
    request.flags = *flags;
    return fn_710070df40((*self->tracks->field_0xc8.Find(key))->field_0x70, &request);
}

// fn_7101177044 with the track whose name hash is `key`.
bool fn_71011773e4(UnkAnimationComponent* self, void* r, s32 idx, u64 key, const u32& flags, const UnkS16& a,
                   const UnkS12& b, const u32& c) {
    return fn_7101177044(reinterpret_cast<UnkStruct_7101176ea8*>(self), r, idx, TrackData(self, key), flags, a, b,
                         c);
}

// An action's variants (0x120 bytes each) by key.
struct UnkActionVariant {
    u8 data[0x120];
};

struct UnkActionVariants {
    u8 pad_0x0[0x80];
    UnkActionVariant* variants;
    u64* keys;
    u32 count;

    UnkActionVariant* End() { return &variants[count]; }
    UnkActionVariant* Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &variants[i];
        return End();
    }
    s32 IndexOf(u64 key) {
        UnkActionVariant* it = Find(key);
        return it == End() ? -1 : static_cast<s32>(it - variants);
    }
};

// fn_71011773e4 with the action `id` names and the index of its variant `variant` (-1 without).
bool fn_7101177258(UnkAnimationComponent* self, void* id, u64 variant, u64 key, const u32& flags, const UnkS16& a,
                   const UnkS12& b, const u32& c) {
    UnkActionVariants* action =
        static_cast<UnkActionVariants*>(fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(self), id));
    s32 idx = action ? action->IndexOf(variant) : -1;
    return fn_71011773e4(self, action, idx, key, flags, a, b, c);
}

// fn_7101176690 inlined: the action `id` names to the track whose name hash is `key`.
static inline bool SendAction(UnkAnimationComponent* self, u64 id, u64 key, const u32& flags, const UnkS16& a,
                              const UnkS12& b, const u32& mask) {
    UnkStruct_7101176ea8* target = reinterpret_cast<UnkStruct_7101176ea8*>(self);
    void* r = fn_7101176d34(target, reinterpret_cast<void*>(id));
    return fn_7101177044(target, r, -1, TrackData(self, key), flags, a, b, mask);
}

// Script binding: reads a name and a flag, sends the action the name's hash names to the main
// track with the flag as its mode.
s32 fn_710117b3f8(UnkAnimationComponent* self, void* stream) {
    char name[0x80];
    bool flag;
    fn_710007cf30(stream, "sb", name, &flag);
    u32 mode = flag;
    u64 id;
    fn_710006663c(&id, name);
    u32 mask = UnkAll_SpitSend().value;
    SendAction(self, id, kMainTrack, mode, reinterpret_cast<const UnkS16&>(g_unk_710164b958), DefaultBlend(),
               mask);
    return 0;
}

// Restarts every track with the action of its own name, at `time` seconds, mode 0x21; `sync` picks
// the blend.
void fn_7101178c88(UnkAnimationComponent* self, bool sync, f32 time) {
    UnkTrackSet* set = self->field_0x40;
    for (UnkTrackEntry** it = set->field_0xc8.values; it != self->field_0x40->field_0xc8.End(); ++it) {
        UnkObj_710117af10* track = reinterpret_cast<UnkObj_710117af10*>((*it)->field_0x70);
        void* r = fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(self), reinterpret_cast<void*>(TrackName(track)));
        fn_7101177044(reinterpret_cast<UnkStruct_7101176ea8*>(self), r, -1, track, 0x21, UnkS16{~0ull, Seconds(time)},
                      sync ? SyncBlend() : DefaultBlend(), UnkAll_SpitSend().value);
    }
}

// The same, skipping the tracks fn_710070d830 reports busy.
void fn_7101178b00(UnkAnimationComponent* self, bool sync, f32 time) {
    UnkTrackSet* set = self->field_0x40;
    for (UnkTrackEntry** it = set->field_0xc8.values; it != self->field_0x40->field_0xc8.End(); ++it) {
        UnkObj_710117af10* track = reinterpret_cast<UnkObj_710117af10*>((*it)->field_0x70);
        void* r = fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(self), reinterpret_cast<void*>(TrackName(track)));
        if (fn_710070d830(track))
            continue;
        fn_7101177044(reinterpret_cast<UnkStruct_7101176ea8*>(self), r, -1, track, 0x21, UnkS16{~0ull, Seconds(time)},
                      sync ? SyncBlend() : DefaultBlend(), UnkAll_SpitSend().value);
    }
}

// ---- Track queries --------------------------------------------------------------------------

// fn_710070d890 of the track whose name hash is `key` when it plays an action; 0 otherwise.
f32 fn_710117808c(UnkAnimationComponent* self, u64 key) {
    UnkTrack* track = FindTrack(self, key);
    if (track && fn_710070d500(track))
        return fn_710070d890(track);
    return 0.0f;
}

// What is left of the track whose name hash is `key` (fn_710070d850 minus fn_710070d890), or 0.
f32 fn_7101178134(UnkAnimationComponent* self, u64 key) {
    UnkTrack* track = FindTrack(self, key);
    if (!track)
        return 0.0f;
    f32 end = fn_710070d850(track);
    return end - fn_710070d890(track);
}

struct UnkTimelineId {
    u64 value;
};

struct UnkTimelineHeader {
    u8 pad_0x0[0x40];
    UnkTimelineId id;
};

extern UnkTimelineId g_unk_7101647910 __attribute__((visibility("hidden")));

// The id of the timeline of the track whose name hash is `key`: -1 without the track, the default
// (g_unk_7101647910) without a timeline.
UnkTimelineId fn_7101177be0(UnkAnimationComponent* self, u64 key) {
    UnkTrack* track = FindTrack(self, key);
    if (!track)
        return UnkTimelineId{~0ull};
    return fn_710070d4e8(track) ? reinterpret_cast<UnkTimelineHeader*>(fn_710070d4e8(track))->id
                                : g_unk_7101647910;
}

struct UnkTimelineEvents {
    u8 pad_0x0[0x120];
    UnkReflObject** events;
    u32 count;
};

// Whether the first track's timeline has a CCharClassInvertRotationEvent.
bool fn_7101178e74(UnkAnimationComponent* self) {
    UnkTrack* track = self->field_0x40->field_0xc8.values[0]->field_0x70;
    UnkTimelineEvents* timeline = reinterpret_cast<UnkTimelineEvents*>(fn_710070d4e8(track));
    if (!timeline)
        return false;
    for (UnkReflObject** it = timeline->events; it < timeline->events + timeline->count; ++it) {
        if (fn_710009d50c((*it)->vf_0x0(), fn_7100cc4f78()))
            return true;
    }
    return false;
}

struct UnkSyncTag {};

struct UnkTrackActionB0 {
    u8 pad_0x0[0xb0];
    void* field_0xb0;
};

void fn_7101177490(UnkTrackAction* action, UnkSyncTag tag, UnkTrack* other, void* data, f32 time);

// When the main track plays an action, lines every other track up with it (fn_7101177490 with the
// main track's time).
void fn_710117774c(UnkAnimationComponent* self) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** found = set->field_0xc8.Find(kMainTrack);
    UnkTrack* track = (found != set->field_0xc8.End() && *found) ? (*found)->field_0x70 : nullptr;
    if (!fn_710070d500(track))
        return;
    set = self->field_0x40;
    if (set->field_0xc8.count == 0)
        return;
    UnkTrackEntry** value = set->field_0xc8.values;
    for (u64* key = set->field_0xc8.keys; key != set->field_0xc8.keys + set->field_0xc8.count; ++key, ++value) {
        if (*key == kMainTrack) {
            set = self->field_0x40;
            continue;
        }
        fn_710070d4e8(track);
        UnkTrack* other = (*value)->field_0x70;
        f32 time = fn_710070d89c(track);
        UnkTrackAction* action = fn_710070d500(track);
        fn_7101177490(action, UnkSyncTag(), other, reinterpret_cast<UnkTrackActionB0*>(action)->field_0xb0, time);
        set = self->field_0x40;
    }
}

// ---- Script and reflection bindings ---------------------------------------------------------

struct UnkTrackEntry_bef8 {
    u8 pad_0x0[0x70];
    UnkObj_710117af10* track;
};

struct UnkTrackSet_bef8 {
    u8 pad_0x0[0xf0];
    UnkTrackEntry_bef8** playing;
    u8 pad_0xf8[8];
    u32 playing_count;
};

struct UnkAnimComponent_bef8 {
    u8 pad_0x0[0x40];
    UnkTrackSet_bef8* tracks;

    // fn_710117ae38 inlined.
    bool IsPlaying(u64 id, bool skipBusy) {
        for (UnkTrackEntry_bef8** it = tracks->playing; it != tracks->playing + tracks->playing_count; ++it) {
            UnkObj_710117af10* track = (*it)->track;
            if (TrackName(track) != id)
                continue;
            if (skipBusy && fn_710070d830(track))
                continue;
            return true;
        }
        return false;
    }
};

// Reflected: whether a track now plays the action of class id args[0] (skipping busy ones when
// args[1] is set), as a bool value.
UnkReflValue fn_710117bef8(char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkAnimComponent_bef8* target = *reinterpret_cast<UnkAnimComponent_bef8**>(obj + 8);
    u64 id = *refl_ArgValue<u64>(&a[0]);
    const bool* skipBusy = refl_ArgValue<bool>(&a[1]);
    bool v = target->IsPlaying(id, *skipBusy);
    const CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}

struct UnkVersionRegistry;
struct UnkVersionEntry;

struct UnkAnimComponent_b1c8 {
    u8 pad_0x0[0x40];
    UnkTrackSet* tracks;

    bool IsPlayingClass(const CClass* cls) {
        if (!tracks)
            return false;
        for (UnkTrackEntry** it = tracks->field_0xf0.values; it != tracks->field_0xf0.End(); ++it) {
            UnkTrack* track = (*it)->field_0x70;
            UnkTrackAction* action = fn_710070d500(track);
            if (action && fn_7100c97930(reinterpret_cast<UnkTrackAction_1444*>(action), cls, fn_710070d89c(track)))
                return true;
        }
        return false;
    }
};

// Stores the value in place when `v` already holds a compatible type, else rebuilds it.
static inline void SetValue(UnkReflValue* v, const CType* t, const void* obj) {
    const CType* cur = reinterpret_cast<const CType*>(v->type);
    if (cur && v->field_0x14 == 0 &&
        (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                  : fn_710009d52c(t, cur))) {
        fn_71000a1cc8(v, t, obj);
    } else {
        fn_71000a1c60(v);
        fn_71000a18d4(v, t, obj, false);
    }
}

// Script binding: reads a class name; when it names a reflected class, stores whether a track now
// plays an action of it in `out`.
bool fn_710117b1c8(UnkAnimComponent_b1c8* self, void* stream, UnkReflValue* out) {
    char name[0x80];
    fn_710007cf30(stream, "s", name);
    UnkReflObject* entry;
    {
        UnkVersionRegistry* registry = g_unk_7101d4c8d0;
        u64 id;
        fn_710006663c(&id, name);
        entry = reinterpret_cast<UnkReflObject*>(fn_71000a5118(registry, &id));
    }
    if (!entry || !fn_710009d50c(entry->vf_0x0(), fn_7100096100()))
        return true;
    bool playing = self->IsPlayingClass(reinterpret_cast<const CClass*>(entry));
    SetValue(out, fn_710000d228(), &playing);
    return true;
}

struct UnkAnimComponent_a018 {
    u8 pad_0x0[0x2b4];
    u32 field_0x2b4;
    CStrId field_0x2b8;

    void SetName(const CStrId& name) {
        if (field_0x2b8.data != name.data) {
            field_0x2b8 = name;
            field_0x2b4 = 0;
        }
    }
};

// Script binding: reads a name into the string id at +0x2b8 (resetting +0x2b4 when it changes).
s32 fn_710117a018(UnkAnimComponent_a018* self, void* stream) {
    char buffer[0x80];
    fn_710007cf30(stream, "s", buffer);
    CStrId name(buffer, true);
    self->SetName(name);
    return 0;
}

// ---- Action sets ----------------------------------------------------------------------------

struct UnkMtx44_b46a2 {
    f32 m[16];
};

UnkVec3f fn_710007a320(const UnkVec3f* v, f32 angle);

// fn_7101179878 for the action `id` names, at `seconds` turned into frames on its clock.
void fn_7101179de0(UnkAnimActionSetsOwner* self, void* id, u64 key, f32 seconds, UnkMtx44_b46a2* out, u32 b) {
    void* entry = fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(self), id);
    if (!entry)
        return;
    f32 frames = fn_7100b8aff0(seconds, static_cast<const UnkAnimTiming*>(entry));
    fn_7101179878(self, entry, 0, key, out, b, frames);
}

// ---- Updates --------------------------------------------------------------------------------

struct UnkStruct_71007142e8;
void fn_71007142e8(UnkStruct_71007142e8* self);
struct UnkParent;
void fn_71002d58e4(UnkParent* self, const float* dt);

struct alignas(8) UnkAnimChannel {
    u8 data[0x70];
    UnkAnimChannel() { fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(this)); }
};

// What fn_71002d58e4 is given: a time step, a flag and three channels.
struct UnkAnimUpdate {
    f32 dt;
    bool flag;
    UnkAnimChannel channels[3];
};

struct UnkAnimEffect;
void fn_7100143618(UnkAnimEffect* self, bool flag, f32 value);

struct UnkAnimEffectHolder {
    u8 pad_0x0[0x50];
    UnkAnimEffect* effect;
};

struct UnkAnimOwner_a88 {
    u8 pad_0x0[0x228];
    UnkAnimEffectHolder* effects;
};

struct UnkAnimComponent_f84 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168(f32 time);

    u8 pad_0x8[0x28];
    UnkAnimOwner_a88* owner;  // 0x30
    u8 pad_0x38[8];
    UnkParent* tracks;  // 0x40

    // Rewinds to 0 and runs an empty update over the tracks.
    void Reset() {
        vf_0x168(0.0f);
        UnkAnimUpdate update;
        update.flag = false;
        update.dt = 0.0f;
        fn_71002d58e4(tracks, &update.dt);
    }
};

// Resets unless the game manager says not to (fn_7100deabe4, fn_7100deac04).
void fn_7101179f84(UnkAnimComponent_f84* self) {
    if (fn_7100deabe4(g_unk_7101d47310) || fn_7100deac04(g_unk_7101d47310))
        return;
    self->Reset();
}

// Resets; with `restart`, restarts the owner's effect (+0x228, its +0x50) too.
void fn_7101176a88(UnkAnimComponent_f84* self, bool restart) {
    self->Reset();
    if (!restart)
        return;
    UnkAnimEffectHolder* effects = self->owner->effects;
    if (effects && effects->effect)
        fn_7100143618(effects->effect, true, 0.0f);
}

struct UnkAnimTracks_617c {
    virtual void vf_0x0();
    virtual ~UnkAnimTracks_617c();
};

template <class T>
struct UnkOwnedPtr {
    T* ptr;

    UnkOwnedPtr() : ptr(nullptr) {}
    ~UnkOwnedPtr() { Reset(nullptr); }
    void Reset(T* p) {
        T* old = ptr;
        ptr = p;
        if (old)
            delete old;
    }
    T* Release() {
        T* p = ptr;
        ptr = nullptr;
        return p;
    }
    UnkOwnedPtr& operator=(UnkOwnedPtr&& o) {
        Reset(o.Release());
        return *this;
    }
};

UnkOwnedPtr<UnkAnimTracks_617c> fn_7100db27a8(void* factory);

struct UnkAnimComponent_617c {
    u8 pad_0x0[0x40];
    UnkOwnedPtr<UnkAnimTracks_617c> tracks;
};

// Creates the tracks from the factory g_unk_7172758b18 when there are none yet.
bool fn_710117617c(UnkAnimComponent_617c* self) {
    if (!self->tracks.ptr)
        self->tracks = fn_7100db27a8(g_unk_7172758b18);
    return true;
}

// ---- Lists --------------------------------------------------------------------------------

struct UnkOwner_710070f098;
void fn_710070f098(UnkOwner_710070f098* self, u32 value, u32 extra);

struct UnkActorTarget {
    u8 pad_0x0[0x4c0];
    u8 list_0x4c0[0x28];  // UnkOwner_710070f098
};

// Adds `value` to the list at +0x4c0 and, when `apply` is set, runs fn_7101178c88 (the counterpart
// of fn_7101178e08, CAnimationComponent_b52.cpp).
void fn_710117662c(UnkActorTarget* self, u32 value, u32 extra, bool apply, bool flag, f32 time) {
    fn_710070f098(reinterpret_cast<UnkOwner_710070f098*>(self->list_0x4c0), value, extra);
    if (apply)
        fn_7101178c88(reinterpret_cast<UnkAnimationComponent*>(self), flag, time);
}

struct UnkAnimArray {
    u8 pad_0x0[0xc8];
    void** data;
    u8 pad_0xd0[8];
    u32 count;
};

void fn_71002d596c(UnkAnimArray* self);

struct UnkMorphBallLauncher_b57;
struct UnkHandleOwner_71010c4230;

struct UnkAnimComponent_b1c {
    u8 pad_0x0[0x40];
    UnkAnimArray* array;  // 0x40
    u8 pad_0x48[0x3f8];
    u32 field_0x440;
    u8 pad_0x444[0x24];
    u32 field_0x468;
    u8 pad_0x46c[0x24];
    u32 field_0x490;
    u8 pad_0x494[0x1c];
    s32 field_0x4b0;

    // fn_7101176b98 inlined.
    void ForEach() {
        UnkAnimArray* a = array;
        if (a->count != 0) {
            void** it = a->data;
            do {
                fn_71002d296c(*it);
                ++it;
                a = array;
            } while (it != a->data + a->count);
        }
    }

    void ResetIfIdle() {
        if (field_0x4b0 <= 0) {
            field_0x490 = 0;
            field_0x440 = 0;
            field_0x468 = 0;
            field_0x4b0 = 0;
        }
    }
};
static_assert(__builtin_offsetof(UnkAnimComponent_b1c, field_0x4b0) == 0x4b0, "UnkAnimComponent_b1c layout");

void fn_7101176b1c(UnkAnimComponent_b1c* self) {
    fn_71010c422c(reinterpret_cast<UnkMorphBallLauncher_b57*>(self));
    self->ForEach();
    fn_71002d596c(self->array);
    self->ResetIfIdle();
}

void fn_7101176bec(UnkAnimComponent_b1c* self) {
    self->ForEach();
    self->ResetIfIdle();
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
}

// ---- Slow motion ------------------------------------------------------------------------------

struct UnkPlayer;
f32 fn_71010d6858();

// The slow-motion flags as bytes of the game manager's word at +0xc4.
struct UnkSlomoFlags {
    bool enabled;  // 0xc4
    u8 unk_0xc5;
    bool paused;  // 0xc6
};

struct UnkWaterState {
    u8 pad_0x0[0x70];
    s32 kind;
};

struct UnkAnimOwner_a994 {
    u8 pad_0x0[0x1b8];
    UnkWaterState* water;
};

struct UnkAnimComponent_a994 {
    u8 pad_0x0[0x30];
    UnkAnimOwner_a994* owner;
    u8 pad_0x38[8];
    UnkTrackSet* tracks;
};

static inline bool PlayingTrackOfClass(UnkAnimComponent_a994* self, const CClass* cls, f32 frames) {
    UnkTrackLookup& playing = self->tracks->field_0xf0;
    for (UnkTrackEntry** it = playing.values; it != self->tracks->field_0xf0.End(); ++it) {
        UnkTrackAction* action = fn_710070d500((*it)->field_0x70);
        if (action && fn_7100c97930(reinterpret_cast<UnkTrackAction_1444*>(action), cls, frames))
            return true;
    }
    return false;
}

// The player's animation rate: scaled by fn_71010d6858 while slow motion is on, in low water only
// when a CCharClassIgnoreSlomoInLowWaterTrack plays or the game manager forces it.
f32 fn_710117a994(UnkAnimComponent_a994* self, const UnkAnimTiming* timing, f32 seconds, f32 rate) {
    if (reinterpret_cast<UnkPlayer*>(self->owner) != fn_7100dec788(g_unk_7101d47310))
        return rate;
    UnkWaterState* water = self->owner->water;
    if (!water || water->kind == 2)
        return rate;
    UnkGameManager* manager = g_unk_7101d47310;
    const CClass* cls = fn_7100d129c8();
    f32 frames = fn_7100b8aff0(seconds, timing);
    if (!PlayingTrackOfClass(self, cls, frames) && manager->field_0xc8 == 0 && !manager->field_0x10c)
        return rate;
    UnkSlomoFlags* slomo = reinterpret_cast<UnkSlomoFlags*>(&manager->field_0xc4);
    if (slomo->enabled && !slomo->paused)
        rate *= fn_71010d6858();
    return rate;
}

// ---- Events -----------------------------------------------------------------------------------

// The component's listener lists (+0x438), as UnkAnimListeners_b52 (CAnimationComponent_b52.cpp).
struct UnkAnimListeners {
    base::UnkVector<void*> active;   // 0x0
    base::UnkVector<void*> pending;  // 0x28
    base::UnkVector<void*> queued;   // 0x50
    union {
        s32 lock;  // 0x78
    };
};

// The lock a listener pass holds; releasing the last one applies the pending changes
// (fn_710117ecb0).
struct UnkAnimListenersLock {
    UnkAnimListeners* listeners;

    explicit UnkAnimListenersLock(UnkAnimListeners* l);
    ~UnkAnimListenersLock();
};

void fn_710117ecb0(UnkAnimListenersLock* self);

inline UnkAnimListenersLock::UnkAnimListenersLock(UnkAnimListeners* l) {
    listeners = l;
    ++l->lock;
}

inline UnkAnimListenersLock::~UnkAnimListenersLock() { fn_710117ecb0(this); }

struct UnkAnimListeners_b52;
void fn_710117eb1c(UnkAnimListeners_b52* self, void* value);
struct UnkPairOwner;
struct UnkValueSource;

// A copied string id released in two steps (the original reloads `data` and the count after the
// decrement), as CStrId::Reset does.
struct UnkMsgName : base::global::UnkStrIdBase {
    UnkMsgName(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkMsgName() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// What the listeners are told: kind 2, the event's data and the source's name.
struct UnkAnimEventMsg {
    u32 kind;
    void* info;
    UnkMsgName name;

    UnkAnimEventMsg(void* info_, const CStrId& name_) : kind(2), info(info_), name(name_) {}
};

struct UnkAnimListener {
    u8 pad_0x0[8];
    void* context;
    bool (*callback)(void* context, UnkAnimEventMsg* msg);
};

struct UnkAnimEventInfo {
    u8 pad_0x0[0x34];
    u32 keys;  // bit 0: the first key list, bit 1: the second
    CStrId key;
};

struct UnkAnimEventData {
    u8 pad_0x0[0x10];
    UnkAnimEventInfo* info;
};

struct UnkAnimEvent {
    u8 pad_0x0[8];
    UnkAnimEventData* data;
    bool handled;
};

struct UnkAnimSource {
    u8 pad_0x0[0x7e];
    bool removes_keys;
};

struct UnkAnimComponent_a4a4 {
    u8 pad_0x0[0x2d0];
    UnkGuiKeyList keys_0x2d0;
    u8 pad_0x300[0x80];
    UnkGuiKeyList keys_0x380;
    u8 pad_0x3b0[0x88];
    UnkAnimListeners listeners;  // 0x438
};
static_assert(__builtin_offsetof(UnkAnimComponent_a4a4, listeners) == 0x438, "UnkAnimComponent_a4a4 layout");

// An event: takes its key out of the key lists it names (when the source asks), then, unless it is
// handled, tells every queued listener; listeners that are gone or answer false are removed.
void fn_710117a4a4(UnkAnimComponent_a4a4* self, UnkAnimSource* source, UnkAnimEvent* event) {
    if (source->removes_keys) {
        if (event->data->info->keys & 1)
            fn_710002ffb4(&self->keys_0x2d0, &event->data->info->key);
        if (event->data->info->keys & 2)
            fn_710002ffb4(&self->keys_0x380, &event->data->info->key);
    }
    if (event->handled)
        return;
    UnkAnimEventMsg msg(event->data,
                        *reinterpret_cast<CStrId*>(fn_71002dba48(reinterpret_cast<UnkPairOwner*>(source))));
    UnkAnimListenersLock lock(&self->listeners);
    void** end = self->listeners.queued.data + self->listeners.queued.count;
    for (void** it = self->listeners.queued.data; it != end; ++it) {
        UnkAnimListener* listener = static_cast<UnkAnimListener*>(*it);
        if (!listener || !listener->callback || !listener->callback(listener->context, &msg))
            fn_710117eb1c(reinterpret_cast<UnkAnimListeners_b52*>(&self->listeners), listener);
    }
}
