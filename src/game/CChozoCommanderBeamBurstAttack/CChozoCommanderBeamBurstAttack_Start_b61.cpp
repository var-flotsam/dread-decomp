// CChozoCommanderBeamBurstAttack's unit (tools/tu_map.py): sets "onbeamburststart" on the owner's (+0x8) timeline
// (+0x2e8) to the all-flags mask, then finds (once, cached at +0x60) the component with hash 0x801aa095101c1236 of
// the owner's object at +0x280 and, unless it is already on, switches it on. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkComponent {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};
struct UnkObject;
// As src/game/CChozoWarriorXAIComponent/fn_710048ecbc.cpp declares them.
struct UnkPart;

struct UnkBeamBurstOwner_b61a2 {
    u8 pad_0x0[0x280];
    UnkObject* field_0x280;
    u8 pad_0x288[0x60];
    void* field_0x2e8;
};

struct UnkBeamBurstAttack_b61a2 {
    u8 pad_0x0[0x8];
    UnkBeamBurstOwner_b61a2* owner;
    u8 pad_0x10[0x50];
    UnkComponent* field_0x60;
};

// The component with the hash, looked up once and cached at +0x60 (null when the kind check rejects it).
static inline UnkComponent* Find_b61a2(UnkBeamBurstAttack_b61a2* self, UnkObject* object) {
    if (object && !self->field_0x60) {
        UnkComponent* component = reinterpret_cast<UnkComponent*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(object),
                                                                                0x801aa095101c1236ull));
        if (component)
            component = fn_710114e680(object, component->field_0xb) ? component : nullptr;
        self->field_0x60 = component;
    }
    return self->field_0x60;
}

void fn_7100429384(UnkBeamBurstAttack_b61a2* self) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (timeline) {
        base::reflection::UnkReflTypeIdArg mask;
        base::global::CName id("onbeamburststart");
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id.hash, &mask.id, false, -1.0f);
    }
    UnkObject* object = self->owner->field_0x280;
    if (UnkComponent* component = Find_b61a2(self, object)) {
        asm("" : "+r"(object));
        if (object) {
            auto* sw = reinterpret_cast<UnkSwitchObject*>(component);
            if (!sw->field_0x121)
                fn_7100145e94(sw, true);
        }
    }
}
