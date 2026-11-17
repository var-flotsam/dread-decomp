// UnkTU2553 (the game manager): makes `actor` the player. The old player hands its components of the
// shared kinds over to the new one, and the new one is placed at `pos` (and turned to `dir`).
// Names and layouts are placeholders.
#include <initializer_list>

#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

namespace base {
struct UnkAllocator;
}
struct UnkEntityComponents;
struct UnkActor_710023259c;
struct UnkStrIdArray;
struct UnkVec3f;

void fn_7100d7b468(UnkEntityComponents* self, void* a);
void fn_7100d7b394(UnkEntityComponents* self, void* a);
void fn_7100d7822c(UnkActor_710023259c* from, UnkActor_710023259c* to, const UnkStrIdArray& names);
void fn_7100f0ccac(void* listeners, UnkWeakTarget* from, UnkWeakTarget* to);
void fn_7100f0cd54(void* listeners, UnkWeakTarget* to, UnkWeakTarget* from);

// A list of string ids built from an initializer list.
struct UnkKindList_df492c {
    CStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkKindList_df492c(std::initializer_list<CStrId> ids, u32 step, base::UnkAllocator* allocator,
                       u32 flags);  // fn_710097bb7c
    ~UnkKindList_df492c();          // fn_71008600a8
};

// The kind ids components of each class are registered under.
inline const CStrId& Kind_DAMAGE() {
    static const CStrId s_id("DAMAGE", true);
    return s_id;
}

inline const CStrId& Kind_INVENTORY() {
    static const CStrId s_id("INVENTORY", true);
    return s_id;
}

inline const CStrId& Kind_INPUT() {
    static const CStrId s_id("INPUT", true);
    return s_id;
}

inline const CStrId& Kind_FACTION() {
    static const CStrId s_id("FACTION", true);
    return s_id;
}

inline const CStrId& Kind_LIFE() {
    static const CStrId s_id("LIFE", true);
    return s_id;
}

inline const CStrId& Kind_SPECIALENERGY() {
    static const CStrId s_id("SPECIALENERGY", true);
    return s_id;
}

inline const CStrId& Kind_ABILITY() {
    static const CStrId s_id("ABILITY", true);
    return s_id;
}

inline const CStrId& Kind_MELEE() {
    static const CStrId s_id("MELEE", true);
    return s_id;
}

inline const CStrId& Kind_AUDIO() {
    static const CStrId s_id("AUDIO", true);
    return s_id;
}

struct UnkPlayerOwner_df492c {
    u8 pad_0x0[0x38];
    u8 field_0x38[0x29b8 - 0x38];  // what is told when the player changes
    UnkWeakHandle* player;
};

static inline UnkWeakTarget* Player(UnkPlayerOwner_df492c* self) {
    return self->player ? self->player->target : nullptr;
}

// vtable slot 0xd8: turns the actor (the callers pass true).
static inline void SetDirection(UnkActor* actor, const UnkVec3f* dir) {
    reinterpret_cast<void (*)(UnkActor*, const UnkVec3f*, bool)>(actor->vtable[0xd8 / 8])(actor, dir, true);
}

void fn_7100df492c(void* manager, void* newPlayer, const void* pos, void* dir) {
    static const UnkKindList_df492c s_kinds({Kind_DAMAGE(), Kind_INVENTORY(), Kind_INPUT(), Kind_FACTION(),
                                             Kind_LIFE(), Kind_SPECIALENERGY(), Kind_ABILITY(), Kind_MELEE(),
                                             Kind_AUDIO()},
                                            10, nullptr, 15);
    UnkPlayerOwner_df492c* self = static_cast<UnkPlayerOwner_df492c*>(manager);
    UnkWeakTarget* actor = static_cast<UnkWeakTarget*>(newPlayer);
    if (Player(self) == actor)
        return;

    UnkWeakTarget* old = Player(self);
    if (old) {
        fn_7100d7b468(reinterpret_cast<UnkEntityComponents*>(old), actor);
        fn_7100232864(reinterpret_cast<UnkEntityState*>(old), false);
        fn_7100f0ccac(self->field_0x38, old, actor);
    }
    AssignRef(&self->player, actor ? fn_7100d78378(actor) : nullptr);
    if (old)
        fn_7100d7822c(reinterpret_cast<UnkActor_710023259c*>(old), reinterpret_cast<UnkActor_710023259c*>(actor),
                      reinterpret_cast<const UnkStrIdArray&>(s_kinds));

    if (UnkActor* player = reinterpret_cast<UnkActor*>(Player(self))) {
        player->vf_0xd0(static_cast<const UnkVec3f*>(pos), true);
        if (dir)
            SetDirection(reinterpret_cast<UnkActor*>(self->player->target), static_cast<const UnkVec3f*>(dir));
        fn_7100232864(reinterpret_cast<UnkEntityState*>(Player(self)), true);
    }
    if (UnkWeakTarget* player = Player(self)) {
        fn_7100d7b394(reinterpret_cast<UnkEntityComponents*>(player), old);
        fn_7100f0cd54(self->field_0x38, actor, old);
    }
}
