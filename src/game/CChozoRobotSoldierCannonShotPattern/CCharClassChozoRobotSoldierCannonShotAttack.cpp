// In the CChozoRobotSoldierCannonShotPattern unit: the constructor of the 0x118-byte reflected
// CCharClassChozoRobotSoldierCannonShotAttack (fn_7100461524 news it): the attack base constructor
// fn_71003641fc, the vtable 0x7101a38100, an empty array of patterns at +0xf0 ("tPatterns"; step 10,
// flags 0xf, the default allocator), and the base's flag at +0xc0 cleared. Layout placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CCharClassAttack.h"
#include "types.h"

// Declared as src/game/CCharClassAttack/fn_71003641fc.cpp defines it.
struct UnkStruct_71003641fc;
extern void* g_unk_7101a38100[];

struct UnkCannonShotAttackDef {
    void** vtable;
    u8 pad_0x8[0xb8];
    bool field_0xc0;
    u8 pad_0xc1[0x2f];
    base::UnkVector<void*> patterns;  // 0xf0
};

void fn_710045ee14(UnkCannonShotAttackDef* self) {
    fn_71003641fc(reinterpret_cast<UnkStruct_71003641fc*>(self));
    self->vtable = g_unk_7101a38100 + 2;
    new (&self->patterns) base::UnkVector<void*>(0xf);
    self->field_0xc0 = false;
}
