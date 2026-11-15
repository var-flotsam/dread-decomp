// Unit 437 (aim component): looks up the aiming laser's switch object once, switches it on and
// announces "onanalogaiminglasercreated". Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

char* fn_7100208124(char*, bool);

struct UnkLaserDef {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkLaserAim {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
    u8 pad_0x38[0x2a0];
    u32 field_0x2d8;
    u8 pad_0x2dc[0x204];
    UnkLaserDef* field_0x4e0;
    u8 pad_0x4e8[8];
    UnkSwitchObject* field_0x4f0;
    u8 pad_0x4f8[0x30];
    bool field_0x528;
};

void fn_710034dc8c(UnkLaserAim* self) {
    UnkActor* owner = self->field_0x30;
    self->field_0x2d8 = 0;
    UnkSwitchObject* laser = self->field_0x4f0;
    UnkWorld* world = static_cast<UnkWorld*>(owner->field_0x280);
    if (world != nullptr && laser == nullptr) {
        UnkPart* part = fn_7101152b90(world, self->field_0x4e0->field_0x30);
        laser = nullptr;
        if (part && fn_710114e680(reinterpret_cast<UnkObject*>(world), reinterpret_cast<u8*>(part)[0xb]))
            laser = reinterpret_cast<UnkSwitchObject*>(part);
        self->field_0x4f0 = laser;
    }
    if (laser && world && !laser->field_0x121) {
        fn_7100145e94(laser, true);
        fn_7100208124(reinterpret_cast<char*>(self->field_0x4f0->field_0x28), false);
        UnkActor* actor = self->field_0x30;
        u64 key = fn_7100001570("onanalogaiminglasercreated", 0x1a);
        fn_7100d7c828(actor, &key, false);
    }
    self->field_0x528 = true;
}
