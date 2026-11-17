// UnkTU1376 (CDoorLifeComponent's unit): refreshes the door from its definition. When the definition says
// so, takes its open flag, copies the bounds of the named collider and opens the door; then restarts the
// owner's animation track when the id table's track plays, and resends the next track of the current one.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1376.h"
#include "types.h"

using base::global::CStrId;

struct UnkTimelineUserA5_b61;
void fn_71008d2c28(UnkTimelineUserA5_b61* self, const u64* key);
f32 fn_7101177e24(UnkAnimationComponent* self, u64 key);
void fn_710117ab48(UnkAnimationComponent* self, bool flag);

extern const u64 g_unk_710164b090;
extern CStrId g_unk_7172731770 __attribute__((visibility("hidden")));

struct UnkDoorCollider_l8d27f4 {
    virtual void vf_0x0();
    u8 pad_0x8[0x40 - 0x8];
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u8 pad_0x50[0x60 - 0x50];
    bool dirty;  // 0x60

    void vf_0x98() {
        reinterpret_cast<void (*)(UnkDoorCollider_l8d27f4*)>((*reinterpret_cast<void***>(this))[0x98 / 8])(this);
    }
};

struct UnkDoorTrack_l8d27f4 {
    u8 pad_0x0[0x48];
    u64 next;  // 0x48
};

struct UnkDoorDef_l8d27f4 {
    u8 pad_0x0[0x128];
    bool use_open;  // 0x128
    bool open;      // 0x129
};

struct UnkDoorOwner_l8d27f4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* collisions;  // 0x2a0
};

struct UnkDoorLife_l8d27f4 {
    u8 pad_0x0[0x30];
    UnkDoorOwner_l8d27f4* owner;  // 0x30
    u8 pad_0x38[0x41b - 0x38];
    bool open;  // 0x41b
    u8 pad_0x41c[0x41f - 0x41c];
    bool field_0x41f;
    u8 pad_0x420[0x7b4 - 0x420];
    u32 bounds[4];  // 0x7b4
    u8 pad_0x7c4[0x9f8 - 0x7c4];
    UnkDoorDef_l8d27f4* def;  // 0x9f8
};

// The id table names its tracks 0x1b28 and 0x1b30 inside a gap of its placeholder layout.
static inline const CStrId& TrackId(u32 index) {
    return reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x1b20)[index];
}

void fn_71008d27f4(UnkDoorLife_l8d27f4* self) {
    base::reflection::UnkReflTypeIdArg restart_mode;
    base::reflection::UnkReflTypeIdArg restart_mask;
    base::reflection::UnkReflTypeIdArg next_mode;
    base::reflection::UnkReflTypeIdArg next_mask;
    UnkDoorDef_l8d27f4* def = self->def;
    if (def->use_open) {
        self->open = def->open;
        if (UnkCollisionContainer* collisions = self->owner->collisions) {
            auto* collider =
                reinterpret_cast<UnkDoorCollider_l8d27f4*>(fn_71008640a4(collisions, &g_unk_7172731770));
            if (collider) {
                if (collider->dirty)
                    collider->vf_0x98();
                self->bounds[0] = collider->field_0x40;
                self->bounds[1] = collider->field_0x44;
                self->bounds[2] = collider->field_0x48;
                self->bounds[3] = collider->field_0x4c;
            }
        }
        if (self->open) {
            if (!self->field_0x41f)
                fn_71008d2c28(reinterpret_cast<UnkTimelineUserA5_b61*>(self), &g_unk_710164b090);
            fn_71008d0ef4(self);
        }
    }

    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return;
    const u64 kind = 0xf4061ff25a2163d6;
    if (fn_710117ae38(animation, GameManagerIdTable().field_0xb08.data->Key(), false)) {
        u64 restart = GameManagerIdTable().field_0xb10.data->Key();
        restart_mode.id = 0;
        restart_mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, restart, kind, &restart_mode, &g_unk_710164b958, &g_unk_7172769740, &restart_mask);
        fn_7101177e24(animation, kind);
        fn_710117ab48(animation, false);
    }
    if (fn_710117ae38(animation, TrackId(1).data->Key(), false) ||
        fn_710117ae38(animation, TrackId(2).data->Key(), false)) {
        auto* track = static_cast<UnkDoorTrack_l8d27f4*>(fn_7101177d2c(animation, kind));
        u64 next = track->next;
        next_mode.id = 0;
        next_mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, next, kind, &next_mode, &g_unk_710164b958, &g_unk_7172769740, &next_mask);
    }
}
