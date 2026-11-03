// Copies of fn_71007cebf0 / fn_71007cec94 (src/game/CXParasiteDropComponent/
// CXParasiteDropComponent_Dtor_b49a3.cpp) in CChozoRobotSoldierCannonShotPattern's unit: the
// vtable, the list of owned objects at +0xf0 destroyed, then the base destructor fn_7100364430;
// fn_710045ef18 also frees the object. Names and layouts are placeholders.
#include "game/UnkComponent.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

extern void* g_unk_7101a38100[];

void fn_7100364430(UnkBaseComponent* self);

struct UnkCannonShotComponent_b52 : UnkBaseComponent {
    u8 pad_0x40[0xb0];
    UnkOwnedList list;  // 0xf0
};

__attribute__((always_inline)) static inline void Destroy_b52(UnkCannonShotComponent_b52* self) {
    self->vtable = g_unk_7101a38100 + 2;
    self->list.~UnkOwnedList();
    fn_7100364430(self);
}

void fn_710045ee74(UnkCannonShotComponent_b52* self) {
    Destroy_b52(self);
}

void fn_710045ef18(UnkCannonShotComponent_b52* self) {
    Destroy_b52(self);
    operator delete(self);
}
