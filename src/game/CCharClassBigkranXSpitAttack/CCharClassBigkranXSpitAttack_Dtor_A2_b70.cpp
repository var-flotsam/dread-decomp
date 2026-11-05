// CCharClassBigkranXSpitAttack's unit (the name from reflection): the destructor (and its deleting form) of
// the class with vtable 0x7101a312d0: the string id at +0x170 (released as base::file::UnkDeviceName, decrement
// and test in two steps), the array at +0x148 freed, then the base destructor fn_7100364430. Names and
// layouts are placeholders.
#include "base/CDeviceQueue.h"
#include "base/UnkVector.h"
#include "game/UnkComponent.h"
#include "types.h"

extern void* g_unk_7101a312d0[];

void fn_7100364430(UnkBaseComponent* self);

struct UnkSpitAttack_A2_b70 : UnkBaseComponent {
    u8 pad_0x40[0x108];
    base::UnkVector<u32> list;  // 0x148
    base::file::UnkDeviceName name;   // 0x170
};
static_assert(__builtin_offsetof(UnkSpitAttack_A2_b70, name) == 0x170, "UnkSpitAttack_A2_b70 layout");

__attribute__((always_inline)) static inline void Destroy_A2_b70(UnkSpitAttack_A2_b70* self) {
    self->vtable = g_unk_7101a312d0 + 2;
    self->name.~UnkDeviceName();
    self->list.Free();
    fn_7100364430(self);
}

void fn_71003f0bac(UnkSpitAttack_A2_b70* self) {
    Destroy_A2_b70(self);
}

void fn_71003f0c48(UnkSpitAttack_A2_b70* self) {
    Destroy_A2_b70(self);
    operator delete(self);
}
