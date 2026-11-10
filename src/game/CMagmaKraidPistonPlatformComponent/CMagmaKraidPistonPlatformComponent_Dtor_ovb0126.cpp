// CMagmaKraidPistonPlatformComponent's unit (tools/tu_map.py): a component's destructor: the vtable,
// the listener at +0x2b8 (game_5821e4/CAttackDtor.h), four value holders from +0x240 down to +0xf0, the
// member at +0x40, then the component destructor (fn_71010c4194). Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkComponent.h"
#include "types.h"

extern char g_unk_7101a81c18[];  // the component's vtable
extern char g_unk_7101a81de8[];
extern char g_unk_7101a81e40[];

struct UnkKraidPair_ovb0126 {
    u64 a, b;
    ~UnkKraidPair_ovb0126() {
        a = 0;
        b = 0;
    }
};

struct UnkKraidValueHolderBase_ovb0126 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    ~UnkKraidValueHolderBase_ovb0126() {
        vtable = g_unk_7101a81de8 + 0x10;
        fn_71000a1c60(&value);
    }
};

struct UnkKraidValueHolder_ovb0126 : UnkKraidValueHolderBase_ovb0126 {
    UnkKraidPair_ovb0126 pairs[4];
};
static_assert(sizeof(UnkKraidValueHolder_ovb0126) == 0x70, "value holder layout");

struct UnkKraidMember_ovb0126 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    ~UnkKraidMember_ovb0126() {
        vtable = g_unk_7101a81e40 + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkKraidPistonPlatform_ovb0126 {
    char* vtable;
    u8 pad_0x8[0x38];
    UnkKraidMember_ovb0126 field_0x40;
    u8 pad_0x68[0x88];
    UnkKraidValueHolder_ovb0126 field_0xf0;
    UnkKraidValueHolder_ovb0126 field_0x160;
    UnkKraidValueHolder_ovb0126 field_0x1d0;
    UnkKraidValueHolder_ovb0126 field_0x240;
    u8 pad_0x2b0[0x8];
    UnkAttackListenerRaw listener;
};
static_assert(__builtin_offsetof(UnkKraidPistonPlatform_ovb0126, listener) == 0x2b8, "layout");

void fn_71009181d0(UnkKraidPistonPlatform_ovb0126* self) {
    self->vtable = g_unk_7101a81c18 + 0x10;
    self->listener.Destroy();
    self->field_0x240.~UnkKraidValueHolder_ovb0126();
    self->field_0x1d0.~UnkKraidValueHolder_ovb0126();
    self->field_0x160.~UnkKraidValueHolder_ovb0126();
    self->field_0xf0.~UnkKraidValueHolder_ovb0126();
    self->field_0x40.~UnkKraidMember_ovb0126();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
