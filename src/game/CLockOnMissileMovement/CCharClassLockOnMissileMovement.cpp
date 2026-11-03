// Functions in the CCharClassLockOnMissileMovement unit. fn_7100ab9218 resets the movement state: the
// start position from +0x40 (z cleared) at +0x62c, its rotation angles (fn_710007a7f0) at +0x638, two
// words copied from the object at +0x30, then the two update steps. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

UnkVec3f fn_710007a7f0(UnkVec3f dir);  // direction to rotation angles (src/game_s1/fn_71001bc8a4.cpp)

void fn_7100ab81bc(char* self);
void fn_7100ab8b88(char* self);

struct UnkLockOnMissileSource {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
};

struct UnkLockOnMissileMovement {
    u8 pad_0x0[0x30];
    UnkLockOnMissileSource* source;  // 0x30
    u8 pad_0x38[0x8];
    u32 pos_0x40[2];
    u8 pad_0x48[0x570 - 0x48];
    bool field_0x570;
    u8 pad_0x571[0x62c - 0x571];
    UnkVec3f start;   // 0x62c
    UnkVec3f angle;   // 0x638
    u32 field_0x644;
    u32 field_0x648;
    u32 field_0x64c;
};

void fn_7100ab9218(UnkLockOnMissileMovement* self) {
    ((a_u32*)&self->start)[0] = self->pos_0x40[0];
    ((a_u32*)&self->start)[1] = self->pos_0x40[1];
    ((a_u32*)&self->start)[2] = 0;
    self->angle = ((UnkVec3f(*)(const UnkVec3f&))fn_710007a7f0)(self->start);
    UnkLockOnMissileSource* src = self->source;
    struct Pair { u32 a, b; };
    Pair p = {src->field_0x44, src->field_0x54};
    self->field_0x644 = p.a;
    self->field_0x648 = p.b;
    self->field_0x64c = 0;
    self->field_0x570 = false;
    fn_7100ab81bc(reinterpret_cast<char*>(self));
    fn_7100ab8b88(reinterpret_cast<char*>(self));
}
