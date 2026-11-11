// CMorphBallLauncherComponent's unit (tools/tu_map.py): fn_7100886a00 runs the component base step
// (fn_71010c422c), unregisters the listener at +0x368 from the linked actor's (+0x310, +0x10) object
// at +0x2a8 (fn_710083b0bc), calls fn_71008659c4(0x10000, 0x10) on the owner's (+0x30) collision
// container (+0x2a0) with +0x40; then, when the linked actor's sub-object at +0x220 is of
// fn_7100c4041c's class (fn_710117b348), sets the flag at +0x104 of the actor's object at +0x210,
// removes the link at +0x348 from that sub-object (fn_710117b3f0) and sends it the id at +0x30 of
// the id table's entry at +0x1d78 (fn_7100031894; as CDoorCentralUnitLifeComponent_Send_b56.cpp)
// with mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CComponent.h"
#include "game/CGunComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkListener_b40;
void fn_71008659c4(UnkCollisionContainer*, u32, u32, void*);

struct UnkMorphBallLauncherActor_b57 {
    u8 pad_0x0[0x210];
    u8* field_0x210;  // a flag at +0x104
    u8 pad_0x218[8];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x2a8 - 0x228];
    void* field_0x2a8;
};

struct UnkMorphBallLauncherLink_b57 {
    u8 pad_0x0[0x10];
    UnkMorphBallLauncherActor_b57* actor;
};

struct UnkMorphBallLauncherOwner_b57 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkMorphBallLauncher_b57 {
    u8 pad_0x0[0x30];
    UnkMorphBallLauncherOwner_b57* owner;
    u8 pad_0x38[8];
    u8 field_0x40[0x310 - 0x40];
    UnkMorphBallLauncherLink_b57* link;
    u8 pad_0x318[0x348 - 0x318];
    u8 field_0x348[0x20];
    u8 field_0x368[8];
};

struct UnkIdHolder_b57mb {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkIdTable_b57mb {
    UnkIdTable_b57mb();  // fn_7100031894
    ~UnkIdTable_b57mb();
    u8 pad_0x0[0x1d78];
    UnkIdHolder_b57mb* field_0x1d78;
};

inline UnkIdTable_b57mb& IdTable_b57mb() {
    static UnkIdTable_b57mb s_table;
    return s_table;
}

void fn_7100886a00(UnkMorphBallLauncher_b57* self) {
    fn_71010c422c(self);
    if (self->link && self->link->actor && self->link->actor->field_0x2a8)
        fn_710083b0bc(self->link->actor->field_0x2a8, reinterpret_cast<UnkListener_b40*>(self->field_0x368));
    fn_71008659c4(self->owner->collision, 0x10000, 0x10, self->field_0x40);
    if (!self->link || !self->link->actor)
        return;
    if (!fn_710117b348(self->link->actor->field_0x220, fn_7100c4041c()))
        return;
    self->link->actor->field_0x210[0x104] = 1;
    fn_710117b3f0(reinterpret_cast<UnkWeakTarget*>(self->link->actor->field_0x220), self->field_0x348);
    UnkSpitSend(self->link->actor->field_0x220, IdTable_b57mb().field_0x1d78->id, 0xf4061ff25a2163d6);
}
