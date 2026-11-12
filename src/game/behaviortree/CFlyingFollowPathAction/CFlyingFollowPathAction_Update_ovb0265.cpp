// behaviortree::CFlyingFollowPathAction's unit (unit 484 of tools/tu_map.py): the update step. The time at +0x78
// grows by `dt`; a finished action (+0x70) reports 5. A component of the owner (+0x18, its AI at +0x240) whose
// limit (+0x13d8 while +0x73 is set, else +0x113c) has run out stops the action (vf_0x78). While the owner's
// character class (+0x220) is on a track and +0x71 is set, the track change is requested once (+0x50 tells
// whether the track is the action's); then the state (2 or 6) follows the navigation's (+0x298) path record.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "types.h"

#include "../../../game_s2/UnkSpitSend.h"

struct UnkAnimationComponent;

struct UnkFlyingAI {
    u8 pad_0x0[0x113c];
    f32 field_0x113c;
    u8 pad_0x1140[0x13d8 - 0x1140];
    f32 field_0x13d8;
};

struct UnkFlyingPathKind {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkFlyingPathEntry {
    u8 pad_0x0[0x10];
    UnkFlyingPathKind* field_0x10;
};

struct UnkFlyingPathRecord {
    UnkFlyingPathEntry* entry;  // 0x0
    u8 pad_0x8[8];
    void* field_0x10;
    u8 pad_0x18[8];
    void* field_0x20;
};

struct UnkFlyingNavigation {
    u8 pad_0x0[0xb0];
    UnkFlyingPathRecord* record;  // 0xb0
};

struct UnkFlyingOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkFlyingAI* ai;  // 0x240
    u8 pad_0x248[0x50];
    UnkFlyingNavigation* navigation;  // 0x298
};

struct UnkFlyingTrackName {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};

struct UnkFlyingTrack {
    u8 pad_0x0[0x38];
    void* field_0x38;
};

struct UnkFlyingFollow {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkFlyingOwner* owner;  // 0x18
    u8 pad_0x20[0x30];
    bool field_0x50;
    u8 pad_0x51[7];
    UnkFlyingTrackName* name;  // 0x58
    u8 pad_0x60[0x10];
    bool field_0x70;
    bool field_0x71;
    u8 pad_0x72;
    bool field_0x73;
    u8 pad_0x74[4];
    f32 elapsed;  // 0x78

    void vf_0x78() { reinterpret_cast<void (*)(UnkFlyingFollow*)>(vtable[0x78 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkFlyingFollow, elapsed) == 0x78, "UnkFlyingFollow layout");

static inline bool HasBigLimit(UnkFlyingAI* ai, UnkFlyingFollow& self) {
    return ai && self.field_0x73;
}

u32 fn_7100387460(UnkFlyingFollow* self, f32 dt) {
    f32 elapsed = self->elapsed + dt;
    self->elapsed = elapsed;
    if (self->field_0x70)
        return 5;
    UnkFlyingOwner* owner = self->owner;
    UnkFlyingAI* ai = owner->ai;
    UnkFlyingNavigation* navigation = owner->navigation;
    UnkAnimationComponent* animation = owner->animation;
    f32 limit;
    if (HasBigLimit(ai, *self)) {
        limit = ai->field_0x13d8;
        if (limit > 0.001f && elapsed > limit)
            goto stop;
    }
    limit = ai->field_0x113c;
    if (limit > 0.001f && elapsed > limit) {
    stop:
        self->vf_0x78();
    }
    if (animation && self->field_0x71 && !self->field_0x50) {
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        u64 key = self->name->key;
        mode.id = 0;
        mask.id = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
        fn_7101176690(animation, key, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        UnkFlyingTrack* track = static_cast<UnkFlyingTrack*>(fn_7101177d2c(animation, 0xf4061ff25a2163d6));
        self->field_0x50 = track->field_0x38 == self->name;
        if (!navigation)
            return 6;
    } else if (!navigation) {
        return 6;
    }
    UnkFlyingPathRecord* record = navigation->record;
    if (!record->entry || !record->entry->field_0x10)
        return 6;
    UnkFlyingPathKind* kind = record->entry->field_0x10;
    if (kind->kind == 2 && record->field_0x10) {
    } else {
        // the original reads the kind again
        asm("" : "+r"(kind));
        if (kind->kind == 4 && record->field_0x20) {
        } else {
            return 6;
        }
    }
    return self->field_0x50 ? 2 : 6;
}
