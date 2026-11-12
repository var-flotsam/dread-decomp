// CPowerUpLifeComponent's unit (tools/tu_map.py unit 1381): when the counted link at +0x3c8 names an entity,
// calls the owner's "OnOpen" script function, sets the entity's state, moves it to the owner's position
// (vf_0xd0), registers it with the game manager's points, drops the link, plays "onimpact" on the owner's
// timeline (or schedules fn_7100d7b054 when there is none) and switches the owner's collision (+0x2a0) off.
// Returns 0. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CActorComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU341.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkScriptCaller;
struct UnkEntityState;
struct UnkStruct_710024fa0c;

struct UnkPowerUpPos_A5_b63 {
    f32 x, y, z;
};

struct UnkPowerUpEntity_A5_b63 {
    virtual void vf_0x0();
    u8 pad_0x8[0xd0 - 0x8];
};

struct UnkPowerUpEntityVtbl_A5_b63 {
    u8 pad_0x0[0xd0];
    void (*vf_0xd0)(UnkPowerUpEntity_A5_b63*, const UnkPowerUpPos_A5_b63*, bool);
};

struct UnkPowerUpLink_A5_b63 : base::UnkRefCounted {
    UnkPowerUpEntity_A5_b63* entity;  // 0x10
};

struct UnkPowerUpOwner_A5_b63 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1e0 - 0x68];
    UnkScriptCaller* script;  // 0x1e0
    u8 pad_0x1e8[0x2a0 - 0x1e8];
    UnkStruct_710024fa0c* collision;  // 0x2a0
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkPowerUpLife_A5_b63 {
    u8 pad_0x0[0x30];
    UnkPowerUpOwner_A5_b63* owner;
    u8 pad_0x38[0x3c8 - 0x38];
    UnkPowerUpLink_A5_b63* link;  // 0x3c8
};

static inline UnkPowerUpEntity_A5_b63* Entity_A5_b63(UnkPowerUpLife_A5_b63* self) {
    return self->link ? self->link->entity : nullptr;
}

u32 fn_71008e51c4(UnkPowerUpLife_A5_b63* self) {
    if (!self->link || !self->link->entity)
        return 0;
    if (UnkScriptCaller* script = self->owner->script)
        fn_7100305088(script, "OnOpen", "");
    fn_7100232864(reinterpret_cast<UnkEntityState*>(Entity_A5_b63(self)), true);
    {
        UnkPowerUpEntity_A5_b63* entity = Entity_A5_b63(self);
        UnkPowerUpOwner_A5_b63* owner = self->owner;
        f32 x = owner->x;
        f32 y = owner->y;
        f32 z = owner->z;
        UnkPowerUpPos_A5_b63 pos;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        (*reinterpret_cast<UnkPowerUpEntityVtbl_A5_b63**>(entity))->vf_0xd0(entity, &pos, true);
    }
    if (UnkGameManagerSub_0x608* points = g_unk_7101d47310->field_0x608)
        fn_71011a2770(points, Entity_A5_b63(self));
    if (UnkPowerUpLink_A5_b63* link = self->link) {
        if (--link->ref_count == 0)
            delete link;
        self->link = nullptr;
    }
    if (UnkTimelineComponent* timeline = self->owner->timeline) {
        u64 key = fn_7100001570("onimpact", 8);
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &key, &mask.id, false, -1.0f);
    } else {
        fn_7100d7b054(self->owner, 0.06f);
    }
    if (UnkStruct_710024fa0c* collision = self->owner->collision)
        fn_710024fa0c(collision, false);
    return 0;
}
