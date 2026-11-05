// CShotComponent unit (tools/tu_map.py unit 1507): two destructors, copies of fn_71007cebf0 and fn_71007cec94
// (CXParasiteDropComponent_Dtor_b49a3.cpp) with their own vtable and base. The vtable, the list of
// owned objects at +0x58 (game_s2/UnkOwnedList.h) destroyed, then the base destructor fn_71011acd74;
// fn_71009f8c70 also frees the object. Names and layouts are placeholders.
#include "game/UnkComponent.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

// Signature as src/game/CCharClassCollisionComponent/CCharClassCollisionComponent.cpp declares it.
struct UnkIdObject;
void fn_71011acd74(UnkIdObject* self);

extern void* g_unk_7101a8ab40[];

struct UnkCShotComponent_b52 : UnkBaseComponent {
    u8 pad_0x40[0x18];
    UnkOwnedList list;  // 0x58
};

__attribute__((always_inline)) static inline void Destroy_b52(UnkCShotComponent_b52* self) {
    self->vtable = g_unk_7101a8ab40 + 2;
    self->list.~UnkOwnedList();
    fn_71011acd74(reinterpret_cast<UnkIdObject*>(self));
}

void fn_71009f8bcc(UnkCShotComponent_b52* self) {
    Destroy_b52(self);
}

void fn_71009f8c70(UnkCShotComponent_b52* self) {
    Destroy_b52(self);
    operator delete(self);
}
