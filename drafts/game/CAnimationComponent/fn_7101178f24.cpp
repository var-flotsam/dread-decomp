// CAnimationComponent unit: the component's update. Counts down the pending-name timer and takes the
// pending (or the character class's) action-set name when it runs out, steps the two animated parts of
// the owner (slowed with the player's slow motion), updates the tracks and the blend, and when the
// mode asks for it places the owner on the root track's motion; then notes whether that track's timer
// runs. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1134.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkMtx44_b46a2 {
    f32 m[16];
};

struct UnkStruct_71007142e8;
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

struct UnkPlayer;
f32 fn_71010d6858();

// The slow-motion flags as bytes of the game manager's word at +0xc4.
struct UnkSlomoFlags {
    bool enabled;  // 0xc4
    u8 unk_0xc5;
    bool paused;  // 0xc6
};
bool fn_710007fbc0(const CRntString* a, const CRntString* b);
void fn_710117954c(UnkWeighted_7100714808* self, const UnkAnimChannel* channel);
void fn_7100714808(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other, bool flag);

// The flags the placement clears from the owner's word at +0x90.
extern u32 g_unk_7172a0ae60 __attribute__((visibility("hidden")));

// The name hash of the root track.
static const u64 kRootTrack_l178f24 = 0xf4061ff25a2163d6ull;

// An animated part of the owner, stepped twice per update.
struct UnkAnimPart_l178f24 {
    void** vtable;
    u8 pad_0x8[0x18];
    bool enabled;  // 0x20

    void vf_0x168(f32 dt) { reinterpret_cast<void (*)(UnkAnimPart_l178f24*, f32)>(vtable[0x168 / 8])(this, dt); }
    void vf_0x1d8(f32 dt) { reinterpret_cast<void (*)(UnkAnimPart_l178f24*, f32)>(vtable[0x1d8 / 8])(this, dt); }
    void vf_0x1e0(f32 dt) { reinterpret_cast<void (*)(UnkAnimPart_l178f24*, f32)>(vtable[0x1e0 / 8])(this, dt); }
};

// What says whether the owner places itself on the root track.
struct UnkAnimPlacer_l178f24 {
    void** vtable;

    bool vf_0x358(bool* place) {
        return reinterpret_cast<bool (*)(UnkAnimPlacer_l178f24*, bool*)>(vtable[0x358 / 8])(this, place);
    }
};

struct UnkAnimOwner_l178f24 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x18];
    u32 flags;  // 0x90
    u8 pad_0x94[0x1f0 - 0x94];
    UnkAnimPart_l178f24* part;  // 0x1f0
    u8 pad_0x1f8[0x8];
    UnkAnimPlacer_l178f24* placer;  // 0x200
    u8 pad_0x208[0x38];
    UnkAnimPart_l178f24* other_part;  // 0x240
    u8 pad_0x248[0x3b0 - 0x248];
    bool field_0x3b0;
    u8 pad_0x3b1[0xf];
    bool field_0x3c0;
};
static_assert(__builtin_offsetof(UnkAnimOwner_l178f24, field_0x3c0) == 0x3c0, "layout");

// The character class component, as its action-set name is read.
struct UnkCharClassAnim_l178f24 {
    u8 pad_0x0[0x60];
    CStrId action_sets;  // 0x60
};

// A track as its timer is read.
struct UnkTrackTimer_l178f24 {
    u8 pad_0x0[0xb0];
    u8 timer[0x8];  // 0xb0
};

// The id table's slot whose key names the placement's motion channel.
struct UnkIdTableSlots_l178f24 {
    u8 pad_0x0[0xd50];
    CStrId field_0xd50;
};

struct UnkAnimationComponent_l178f24 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x18];
    UnkAnimOwner_l178f24* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkTrackSet* tracks;  // 0x40
    u8 pad_0x48[0xc0];
    u8 blend[0x70];         // 0x108
    u8 field_0x178[0x70];   // 0x178
    u8 field_0x1e8[0x70];   // 0x1e8
    UnkMatrix44 xform;      // 0x258
    u8 pad_0x298[0x1c];
    f32 name_timer;         // 0x2b4
    CStrId action_sets;     // 0x2b8
    CStrId pending;         // 0x2c0
    s32 mode;               // 0x2c8
    u8 pad_0x2cc[0x4];
    u8 field_0x2d0[0xb0];   // 0x2d0
    u8 field_0x380[0x138];  // 0x380
    bool field_0x4b8;
    bool field_0x4b9;

    void vf_0x168(f32 dt) {
        reinterpret_cast<void (*)(UnkAnimationComponent_l178f24*, f32)>(vtable[0x168 / 8])(this, dt);
    }
};
static_assert(__builtin_offsetof(UnkAnimationComponent_l178f24, xform) == 0x258, "layout");
static_assert(__builtin_offsetof(UnkAnimationComponent_l178f24, field_0x4b9) == 0x4b9, "layout");

// The id's string, counting the read.
inline const CRntString* StringOf_l178f24(const CStrId& id) {
    ++id.data->reads;
    return &id.data->String();
}

// The time step of the owner's parts: the player's is slowed while slow motion is on.
inline f32 PartStep_l178f24(UnkAnimationComponent_l178f24* self, f32 dt) {
    UnkGameManager* manager = g_unk_7101d47310;
    UnkSlomoFlags* slomo = reinterpret_cast<UnkSlomoFlags*>(&manager->field_0xc4);
    if ((manager->field_0xc8 != 0 || manager->field_0x10c) && slomo->enabled && !slomo->paused &&
        reinterpret_cast<UnkPlayer*>(self->owner) == fn_7100dec788(manager))
        return fn_71010d6858() * dt;
    return dt;
}

// Whether the owner is placed on the root track: what its placer says (else the first timer), or the
// second timer.
inline bool WantsPlacement_l178f24(UnkAnimationComponent_l178f24* self) {
    bool place = false;
    UnkAnimPlacer_l178f24* placer = self->owner->placer;
    bool active = placer && placer->vf_0x358(&place) ? place : fn_710002fb70(self->field_0x2d0);
    return active || fn_710002fb70(self->field_0x380);
}

void fn_7101178f24(UnkAnimationComponent_l178f24* self, f32 dt) {
    if (self->name_timer > 0.0f) {
        self->name_timer -= dt;
        if (self->name_timer < 0.0f) {
            self->name_timer = 0.0f;
            const CRntString* pending = StringOf_l178f24(self->pending);
            if (fn_710007fbc0(pending, StringOf_l178f24(GameManagerIdTable().field_0x1d78))) {
                self->action_sets = self->pending;
            } else {
                UnkReflObject* object = self->field_0x10;
                // Read even when the object is of another class, as in the original.
                auto* char_class = reinterpret_cast<UnkCharClassAnim_l178f24*>(
                    fn_710009d50c(object->vf_0x0(), fn_71007105e4()) ? object : nullptr);
                self->action_sets = char_class->action_sets;
            }
        }
    }

    UnkAnimPart_l178f24* part = self->owner->part;
    if (part && part->enabled)
        part->vf_0x1d8(PartStep_l178f24(self, dt));
    self->vf_0x168(dt);

    UnkAnimUpdate update;
    update.flag = self->field_0x4b8;
    update.dt = dt;
    fn_71002d58e4(reinterpret_cast<UnkParent*>(self->tracks), &update.dt);
    auto* blend = reinterpret_cast<UnkWeighted_7100714808*>(self->blend);
    fn_710117954c(blend, &update.channels[0]);
    fn_7100714604(blend, reinterpret_cast<const UnkWeighted_7100714808*>(self->field_0x1e8));
    fn_7100714808(blend, reinterpret_cast<const UnkWeighted_7100714808*>(self->field_0x178), true);

    if (part && part->enabled)
        part->vf_0x1e0(PartStep_l178f24(self, dt));
    UnkAnimPart_l178f24* other = self->owner->other_part;
    if (other && other->enabled)
        other->vf_0x168(dt);

    if (self->mode == 1) {
        if (WantsPlacement_l178f24(self)) {
            UnkTrackEntry** it = self->tracks->field_0xc8.Find(kRootTrack_l178f24);
            if (it != self->tracks->field_0xc8.End() && *it) {
                UnkTrack* track = (*it)->field_0x70;
                if (track && fn_710070d4e8(track)) {
                    UnkMatrix44 motion = {};
                    motion.m[15] = 1.0f;
                    motion.m[10] = 1.0f;
                    motion.m[5] = 1.0f;
                    motion.m[0] = 1.0f;
                    fn_7101179878(reinterpret_cast<UnkAnimActionSetsOwner*>(self), fn_710070d4e8(track), 0,
                                  reinterpret_cast<UnkIdTableSlots_l178f24*>(&GameManagerIdTable())->field_0xd50.data->Key(),
                                  reinterpret_cast<UnkMtx44_b46a2*>(&motion), 2, fn_710070d89c(track));
                    UnkAnimOwner_l178f24* owner = self->owner;
                    UnkCopyMatrix(&owner->transform, fn_71000127c8(fn_710002614c(&self->xform), motion));
                    owner->flags = (owner->flags | 8) & ~g_unk_7172a0ae60;
                    owner->field_0x3c0 = true;
                    owner->field_0x3b0 = true;
                }
            }
        }
    }

    bool running = false;
    UnkTrackEntry** it = self->tracks->field_0xc8.Find(kRootTrack_l178f24);
    if (it != self->tracks->field_0xc8.End() && *it) {
        if (UnkTrack* track = (*it)->field_0x70)
            running = fn_710002fb70(reinterpret_cast<UnkTrackTimer_l178f24*>(track)->timer);
    }
    self->field_0x4b9 = running;
}
