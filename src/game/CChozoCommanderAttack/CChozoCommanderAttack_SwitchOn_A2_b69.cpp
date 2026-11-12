// CChozoCommanderAttack's unit (the name from reflection): finds (once, cached at +0x40) the component with hash
// 0x8b2dcb0a00297e03 of the owner's (+0x8) object at +0x280 and, unless it is already on, switches it on; then
// sets the flag at +0x38. As CChozoCommanderBeamBurstAttack's start
// (src/game/CChozoCommanderBeamBurstAttack/CChozoCommanderBeamBurstAttack_Start_b61.cpp). Names are placeholders.
#include "game/CFXComponent.h"
#include "game/UnkSwitchObject.h"
#include "types.h"

struct UnkComponent;
struct UnkObject;
// As src/game/CChozoWarriorXAIComponent/fn_710048ecbc.cpp declares them.
struct UnkPart;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);

struct UnkChozoCommanderAttackOwner_A2_b69 {
    u8 pad_0x0[0x280];
    UnkObject* field_0x280;
};

struct UnkChozoCommanderAttack_A2_b69 {
    u8 pad_0x0[0x8];
    UnkChozoCommanderAttackOwner_A2_b69* owner;
    u8 pad_0x10[0x28];
    bool field_0x38;
    u8 pad_0x39[0x7];
    UnkComponent* field_0x40;
};

static inline UnkComponent* Find_A2_b69(UnkChozoCommanderAttack_A2_b69* self, UnkObject* object) {
    if (object && !self->field_0x40) {
        UnkComponent* component = reinterpret_cast<UnkComponent*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(object),
                                                                                0x8b2dcb0a00297e03ull));
        if (component)
            component = fn_710114e680(object, reinterpret_cast<u8*>(component)[0xb]) ? component : nullptr;
        self->field_0x40 = component;
    }
    return self->field_0x40;
}

void fn_7100426fb4(UnkChozoCommanderAttack_A2_b69* self) {
    UnkObject* object = self->owner->field_0x280;
    if (UnkComponent* component = Find_A2_b69(self, object)) {
        asm("" : "+r"(object));
        if (object) {
            auto* sw = reinterpret_cast<UnkSwitchObject*>(component);
            if (!sw->field_0x121)
                fn_7100145e94(sw, true);
        }
    }
    self->field_0x38 = true;
}
