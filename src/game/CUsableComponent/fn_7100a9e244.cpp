// Constructor of the object whose state machine fn_7100a9ea64 sets up (destroyed by
// fn_7100a9e6bc): base constructor, empty ids, unbounded ranges (+-FLT_MAX), the state
// machine and its seven states (ids 2, 1, 3..7), the member at +0x4c0 and a listener.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkStaticZero_7172716dcc {
    u32 value;
    UnkStaticZero_7172716dcc() : value(0) {}
};

inline const UnkStaticZero_7172716dcc& UnkGetStaticZero_7172716dcc() {
    static UnkStaticZero_7172716dcc s_value;
    return s_value;
}

extern void* g_unk_7101a94718[];

struct UnkState {
    void** vtable;
    void* machine;
    u32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u64 delegates[8];
};
static_assert(sizeof(UnkState) == 0x70, "UnkState layout");

struct UnkStateObject {
    void** vtable;
    u8 unk_0x8[0x38];
    u32 field_0x40;
    u8 unk_0x44[0x4];
    CStrId field_0x48;
    CStrId field_0x50;
    UnkStringData* field_0x58;  // CStrId
    bool field_0x60;
    u32 field_0x64;
    bool field_0x68;
    bool field_0x69;
    bool field_0x6a;
    bool field_0x6b;
    bool field_0x6c;
    f32 field_0x70;
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    f32 field_0x80;
    f32 field_0x84;
    f32 field_0x88;
    u8 unk_0x8c[0x8];
    u32 field_0x94;
    u32 field_0x98;
    f32 field_0x9c;
    f32 field_0xa0;
    f32 field_0xa4;
    f32 field_0xa8;
    f32 field_0xac;
    f32 field_0xb0;
    u8 unk_0xb4[0x8];
    u32 field_0xbc;
    u32 field_0xc0;
    u8 unk_0xc4[0x4];
    bool field_0xc8;
    f32 field_0xcc;
    u8 unk_0xd0[0x4];
    u32 field_0xd4[7];
    u8 unk_0xf0[0x4];
    u32 field_0xf4;
    bool field_0xf8;
    bool field_0xf9;
    bool field_0xfa;
    bool field_0xfb;
    u16 field_0xfc;
    void** machine_vtable;  // the state machine at +0x100
    u64 field_0x108;
    u64 field_0x110;
    u64 field_0x118;
    u32 field_0x120;
    u8 unk_0x124[0x80];
    u32 field_0x1a4;
    u16 field_0x1a8;
    UnkState states[7];
    u8 field_0x4c0[0x38];
    bool field_0x4f8;
    u8 unk_0x4f9[0xf];
    bool field_0x508;
    u64 field_0x510;
    void** listener_vtable;  // a listener at +0x518
    u64 field_0x520;
    u64 field_0x528;
    u64 field_0x530;
    bool field_0x538;
};
static_assert(__builtin_offsetof(UnkStateObject, machine_vtable) == 0x100, "layout");
static_assert(__builtin_offsetof(UnkStateObject, states) == 0x1b0, "layout");
static_assert(__builtin_offsetof(UnkStateObject, field_0x4f8) == 0x4f8, "layout");
static_assert(__builtin_offsetof(UnkStateObject, listener_vtable) == 0x518, "layout");

extern void* g_unk_7101a94220[];
extern void* g_unk_7101a94668[];
extern void* g_unk_7101a77000[];
void fn_71010c4148(UnkStateObject* self);
void fn_7100aa3ad0(void* self);

static inline void InitState(UnkState* s, u32 id) {
    s->machine = nullptr;
    s->id = id;
    s->field_0x18 = 0;
    s->field_0x20 = 0;
    s->field_0x28 = 2;
    s->field_0x2c = 1;
    s->vtable = &g_unk_7101a94668[2];
    for (u64& d : s->delegates)
        d = 0;
}

static constexpr f32 kMax = 3.40282347e+38f;

// NON_MATCHING(minor): same stores, scheduled differently: the original merges fewer of the
// +-FLT_MAX stores at +0x9c..+0xb0, addresses +0x1a4 from the state machine base and orders
// some of the states' zero stores and the +0xd4..+0xfc stores differently.
void fn_7100a9e244(UnkStateObject* self) {
    fn_71010c4148(self);
    self->vtable = &g_unk_7101a94220[2];
    self->field_0x40 = 1;
    new (&self->field_0x48) CStrId("", true);
    new (&self->field_0x50) CStrId("", true);
    self->field_0x58 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x58 = &g_unk_7101d47108->empty;
    self->field_0x60 = false;
    self->field_0x64 = 0;
    self->field_0x68 = false;
    self->field_0x69 = false;
    self->field_0x6a = true;
    self->field_0x6b = true;
    self->field_0x6c = true;
    self->field_0x70 = 0.0f;
    self->field_0x74 = -kMax;
    self->field_0x78 = -kMax;
    self->field_0x7c = -kMax;
    self->field_0x80 = -kMax;
    self->field_0x84 = kMax;
    self->field_0x88 = kMax;
    self->field_0x94 = UnkGetStaticZero_7172716dcc().value;
    self->field_0x98 = UnkGetStaticZero_7172716dcc().value;
    self->field_0x9c = -kMax;
    self->field_0xa0 = -kMax;
    self->field_0xa4 = -kMax;
    self->field_0xa8 = kMax;
    self->field_0xac = kMax;
    self->field_0xb0 = kMax;
    self->field_0xbc = UnkGetStaticZero_7172716dcc().value;
    self->field_0xc0 = UnkGetStaticZero_7172716dcc().value;
    self->field_0xc8 = false;
    self->field_0xcc = 1.0f;
    for (u32& v : self->field_0xd4)
        v = 0;
    self->field_0xf4 = 0;
    self->field_0xf8 = true;
    self->field_0xf9 = false;
    self->field_0xfa = true;
    self->field_0xfb = true;
    self->field_0xfc = 0;
    self->machine_vtable = &g_unk_7101a94718[2];
    self->field_0x108 = 0;
    self->field_0x110 = 0;
    self->field_0x118 = 0;
    self->field_0x120 = 0;
    self->field_0x1a4 = 8;
    self->field_0x1a8 = 0;
    InitState(&self->states[0], 2);
    InitState(&self->states[1], 1);
    InitState(&self->states[2], 3);
    InitState(&self->states[3], 4);
    InitState(&self->states[4], 5);
    InitState(&self->states[5], 6);
    InitState(&self->states[6], 7);
    fn_7100aa3ad0(self->field_0x4c0);
    self->field_0x4f8 = true;
    self->field_0x508 = false;
    self->field_0x510 = 0;
    self->listener_vtable = &g_unk_7101a77000[2];
    self->field_0x520 = 0;
    self->field_0x528 = 0;
    self->field_0x530 = 0;
    self->field_0x538 = false;
}
