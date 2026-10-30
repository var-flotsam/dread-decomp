// Constructor of a query-system parameter named "IN_MELEE_PATH": runs the base constructor, sets the vtable
// and names the parameter with the string id built from its tag.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkQueryParam {
    void** vtable;
    UnkStrId field_0x8;
    // the base constructor initialises the rest
};

extern void* g_unk_7101a39568[];
void fn_71002bedc8(UnkQueryParam* self);

template <class Base, void (*BaseCtor)(Base*), void** Vtable, class Self>
__attribute__((always_inline)) static inline void Construct(Self* self, const char* name) {
    BaseCtor(reinterpret_cast<Base*>(self));
    self->vtable = &Vtable[2];
    self->field_0x8 = UnkStrId(name, true);
}

void fn_710047785c(UnkQueryParam* self) { Construct<UnkQueryParam, fn_71002bedc8, g_unk_7101a39568>(self, "IN_MELEE_PATH"); }

// Two constructors of an object named "PoisonousGas" in another unit, above their own base
// (fn_7100681a6c), with the name at 0xc8.
struct UnkNamed_71006807f8 {
    void** vtable;
    u8 pad_0x8[0xc0];
    UnkStrId field_0x8;  // at 0xc8
};

extern void* g_unk_7101a600a0[];
void fn_7100681a6c(void* self);

void fn_71006807f8(UnkNamed_71006807f8* self) { Construct<void, fn_7100681a6c, g_unk_7101a600a0>(self, "PoisonousGas"); }
void fn_710068097c(UnkNamed_71006807f8* self) { Construct<void, fn_7100681a6c, g_unk_7101a600a0>(self, "PoisonousGas"); }
