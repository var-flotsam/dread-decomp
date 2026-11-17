// CSPBGun's unit (tools/tu_map.py unit 1317): the gun's update. Not yet started, it tries to start
// (fn_710085e504) and says whether it did; started, it stops (fn_710085e918) once it may no longer fire
// (fn_710085e6d4), and otherwise clears +0x18e8 once fn_710085e7e8 holds. Names and layouts are placeholders.
#include "types.h"

struct UnkSPBGun_A5_b63;
struct UnkSPBGun_b57;
// As src/game/CSPBGun/CSPBGun_CanFire_ovb0058.cpp, CSPBGun_Start_A5_b63.cpp and CSPBGun_Send_b57.cpp
// declare them.
bool fn_710085e6d4(UnkSPBGun_A5_b63* gun);
bool fn_710085e504(UnkSPBGun_A5_b63* self);
bool fn_710085e7e8(UnkSPBGun_A5_b63* self);
void fn_710085e918(UnkSPBGun_b57* self);

struct UnkSPBGun_l85eb68 {
    u8 pad_0x0[0x1880];
    bool started;  // 0x1880
    u8 pad_0x1881[0x18e8 - 0x1881];
    bool field_0x18e8;
};

bool fn_710085eb68(UnkSPBGun_l85eb68* self) {
    if (self->started) {
        if (!fn_710085e6d4(reinterpret_cast<UnkSPBGun_A5_b63*>(self)))
            fn_710085e918(reinterpret_cast<UnkSPBGun_b57*>(self));
        else if (self->field_0x18e8)
            self->field_0x18e8 = !fn_710085e7e8(reinterpret_cast<UnkSPBGun_A5_b63*>(self));
    } else if (fn_710085e504(reinterpret_cast<UnkSPBGun_A5_b63*>(self))) {
        return true;
    }
    return false;
}
