#pragma once

// The object in the actor slot at +0x220 of game/UnkActor.h as the hand-written AI, attack and
// damage sources (game_s2, game_s7, game_lo, game_hi) see it: the callers call it the owner's
// attack, track, target or character class. tools/tu_map.py puts the functions on it
// (fn_710117b348, fn_7101177ea0, fn_71011771e0, fn_7101176690, fn_710117af10) in the unit it names
// after CAnimationComponent, so the placeholder is named after that component; whether the slot
// is that component is unverified. Its +0x40 holds the actor's tracks: a keyed lookup (the layout
// of base::UnkKeyedValueLookup, keys are name hashes) of every track at +0xc8 and one of the
// tracks now playing at +0xf0. An entry's track object is at its +0x70, and what that track plays
// (an action: fn_7100426c7c tests one against CAIAimAction) is at the track's +0x28. Only the
// members the callers share are spelled out; the names and the layout are placeholders.

#include "types.h"

namespace base::reflection {
class CClass;
}

// What a track plays. Its class comes from vtable slot 3; the float at +0x60 is what
// fn_7101177ea0 and fn_710070d89c read (the callers treat it as how far along the track is).
struct UnkTrackAction {
    void** vtable;
    u8 pad_0x8[0x58];
    f32 field_0x60;

    const base::reflection::CClass* vf_0x18() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkTrackAction*)>(vtable[3])(this);
    }
};
static_assert(__builtin_offsetof(UnkTrackAction, field_0x60) == 0x60, "UnkTrackAction placeholder layout");

// A track: the object at +0x70 of a lookup entry.
struct UnkTrack {
    u8 pad_0x0[0x28];
    UnkTrackAction* field_0x28;  // the action it plays; fn_710070d500 returns it
};
static_assert(__builtin_offsetof(UnkTrack, field_0x28) == 0x28, "UnkTrack placeholder layout");

// A value of the two lookups.
struct UnkTrackEntry {
    u8 pad_0x0[0x70];
    UnkTrack* field_0x70;
};
static_assert(__builtin_offsetof(UnkTrackEntry, field_0x70) == 0x70, "UnkTrackEntry placeholder layout");

// Entries by name hash: the layout of base::UnkKeyedValueLookup (base/UnkCloneHelpers.h) with
// plain hashes as keys. Find returns End when the key is missing.
struct UnkTrackLookup {
    UnkTrackEntry** values;
    u64* keys;
    u32 count;

    UnkTrackEntry** End() { return &values[count]; }
    UnkTrackEntry** Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
};

// The tracks of an actor: the object at +0x40 of the component.
struct UnkTrackSet {
    u8 pad_0x0[0xc8];
    UnkTrackLookup field_0xc8;  // every track
    u8 pad_0xe0[0x10];
    UnkTrackLookup field_0xf0;  // the tracks now playing
};
static_assert(__builtin_offsetof(UnkTrackSet, field_0xc8) == 0xc8, "UnkTrackSet placeholder layout");
static_assert(__builtin_offsetof(UnkTrackSet, field_0xf0) == 0xf0, "UnkTrackSet placeholder layout");

struct UnkAnimationComponent {
    u8 pad_0x0[0x40];
    UnkTrackSet* field_0x40;
    u8 pad_0x48[0x38];
    u8 field_0x80[8];  // what fn_710033187c (game_s2/fn_71005820c4) writes a scaled value into
};
static_assert(__builtin_offsetof(UnkAnimationComponent, field_0x40) == 0x40, "UnkAnimationComponent placeholder layout");
static_assert(__builtin_offsetof(UnkAnimationComponent, field_0x80) == 0x80, "UnkAnimationComponent placeholder layout");

// The action a track plays (its +0x28). The definition is generated (src/lifted/).
UnkTrackAction* fn_710070d500(UnkTrack* track);

// True when one of the tracks now playing has an action that fn_7100c97930 accepts for `cls`:
// the callers read it as "the owner is in a track of this class" (the CCharClass*Track classes,
// e.g. CCharClassCrazyTrack). False without a track set. The definition is generated (src/lifted/).
bool fn_710117b348(UnkAnimationComponent* self, const base::reflection::CClass* cls);

// The float at +0x60 of the action of the track whose name hash is `key`, or 0 when there is no
// such entry, track or action (the callers read it as how far along that track is, with the one
// hash 0xf4061ff25a2163d6). Defined in src/game_hi/CAnimationComponent.cpp.
f32 fn_7101177ea0(UnkAnimationComponent* self, u64 key);
