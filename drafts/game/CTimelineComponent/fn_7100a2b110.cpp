// CTimelineComponent unit (tools/tu_map.py): the component's update. After the base step and the unit's
// two per-frame steps, every entry's track is stepped with the table locked; then the removals and the
// additions queued meanwhile are applied, and when the random-event timer (+0x58) runs out a random event
// is played (fn_7100a2b9a8): from the definition's mask (+0x1a8, +0x70) when it has one and the owner allows
// it, else the all-flags mask. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntVector.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkBaseComponent;
void fn_710024fb28(UnkBaseComponent*);  // the actor component base step

struct UnkTimelinePlayer_ovb0089;
void fn_7100a2b564(UnkTimelinePlayer_ovb0089* self);

struct UnkTimelineRandom_ovb0089;
void fn_7100a2b9a8(UnkTimelineRandom_ovb0089* self, const u32* value);

struct UnkUnit1134Upd_b68a4;
void fn_710070d970(UnkUnit1134Upd_b68a4* self, f32 dt);

void fn_7100a2b3bc(UnkTimelineComponent* self, f32 dt);

struct UnkTimelineDef_la2b110 {
    u8 pad_0x0[0x70];
    u32 mask;  // 0x70
};

struct UnkTimelineUpdate_la2b110 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x20];
    f32 timer;  // 0x58
    u8 pad_0x5c[0xc];
    UnkKeyedTable table;  // 0x68
    u8 pad_0x80[0x10];
    bool locked;                                          // 0x90
    base::global::CRntVector<UnkPendingAdd> pending_add;  // 0x98
    base::global::CRntVector<u64> pending_remove;         // 0xc0
    u8 pad_0xe8[0xc0];
    UnkTimelineDef_la2b110* def;  // 0x1a8
};
static_assert(__builtin_offsetof(UnkTimelineUpdate_la2b110, pending_remove) == 0xc0, "layout");
static_assert(__builtin_offsetof(UnkTimelineUpdate_la2b110, def) == 0x1a8, "layout");

void fn_7100a2b110(UnkTimelineUpdate_la2b110* self, f32 dt) {
    UnkTimelineComponent* timeline = reinterpret_cast<UnkTimelineComponent*>(self);
    base::reflection::UnkReflTypeIdArg add_mask;
    base::reflection::UnkReflTypeIdArg event_mask;
    fn_710024fb28(reinterpret_cast<UnkBaseComponent*>(self));
    fn_7100a2b3bc(timeline, dt);
    fn_7100a2b564(reinterpret_cast<UnkTimelinePlayer_ovb0089*>(self));
    self->locked = true;
    UnkTableEntry* end = self->table.End();
    for (UnkTableEntry* it = self->table.values; it != end; ++it)
        fn_710070d970(reinterpret_cast<UnkUnit1134Upd_b68a4*>(it->field_0x0), dt);
    self->locked = false;
    for (u64& id : self->pending_remove)
        fn_7100a2b7b8(timeline, &id);
    self->pending_remove.count = 0;
    for (UnkPendingAdd& add : self->pending_add) {
        add_mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &add.key, &add_mask.id, false, -1.0f);
    }
    self->pending_add.count = 0;
    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (self->timer <= 0.0f) {
            event_mask.id = UnkAll_SpitSend().value;
            if (self->def->mask != UnkNone_SpitSend().value) {
                event_mask.id = 1;
                if (fn_7100d7bea0(self->owner, 1))
                    event_mask.id = self->def->mask | 1;
            }
            fn_7100a2b9a8(reinterpret_cast<UnkTimelineRandom_ovb0089*>(self), &event_mask.id);
        }
    }
}
