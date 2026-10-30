// Destructor body: runs the base destructor (fn_710042bb40), then, when the weak handle at +0x78 still has a
// target, hands the target the object at +0x60 (fn_710117b3f0) and drops the handle.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x60];
    u8 field_0x60[0x18];
    UnkWeakHandle* field_0x78;
};

void fn_710042bb40(UnkSelf* self);

void fn_710043c5c8(UnkSelf* self) {
    fn_710042bb40(self);
    if (UnkWeakHandle* handle = self->field_0x78) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x60);
        ResetRef(&self->field_0x78);
    }
}

// The same destructor body in other units, with their own base destructors and the object and
// handle at other offsets.
#include "game/UnkAttack.h"

struct UnkParent;
void fn_71010c4228(UnkParent* self);
struct UnkHandleOwner_71010c4230;
void fn_71010c4230(UnkHandleOwner_71010c4230* self);

template <u32 Offset>
struct UnkHandoffSelf {
    u8 pad_0x0[Offset];
    u8 object[0x18];
    UnkWeakHandle* handle;
};

template <class Base, void (*BaseDtor)(Base*), u32 Offset>
__attribute__((always_inline)) static inline void HandOff(UnkHandoffSelf<Offset>* self) {
    BaseDtor(reinterpret_cast<Base*>(self));
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->object);
        ResetRef(&self->handle);
    }
}

void fn_71005c5798(UnkHandoffSelf<0x30>* self) { HandOff<UnkAttackBase, fn_71003609b8, 0x30>(self); }
void fn_710060d714(UnkHandoffSelf<0x30>* self) { HandOff<UnkAttackBase, fn_71003609b8, 0x30>(self); }
void fn_71006255b4(UnkHandoffSelf<0x30>* self) { HandOff<UnkAttackBase, fn_71003609b8, 0x30>(self); }
void fn_71007b0408(UnkHandoffSelf<0xc0>* self) { HandOff<UnkHandleOwner_71010c4230, fn_71010c4230, 0xc0>(self); }
void fn_7100899c54(UnkHandoffSelf<0x178>* self) { HandOff<UnkParent, fn_71010c4228, 0x178>(self); }
