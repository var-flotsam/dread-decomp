// The Redenki swarm controller's constructor (tools/tu_map.py: the CRedenkiSwarmControllerComponent unit):
// base constructor, vtable, three allocator-backed lists, then the timers at -1, then a virtual call.
// The repeated zero stores are the original's (value-initialised members then their constructors).
// Names are placeholders.
#include "base/UnkVector.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "types.h"

struct UnkRedenkiHeader_b46a2 {
    u32 count;
    u8 flag;
};

// Fields of the swarm controller set by its constructor; the rest belongs to the base.
struct UnkRedenkiSwarm_b46a2 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    u8 pad_0x8[0x460];
    u64 field_0x468[6];
    union {
        u64 raw_0x498;
        UnkRedenkiHeader_b46a2 header_0x498;
    };
    u64 field_0x4a0;
    base::UnkAllocator* alloc_0x4a8;
    u64 field_0x4b0[5];
    u64 field_0x4d8;
    u64 field_0x4e0;
    UnkRedenkiHeader_b46a2 header_0x4e8;
    u32 field_0x4f0;
    base::UnkAllocator* alloc_0x4f8;
    u64 field_0x500;
    u64 field_0x508;
    UnkRedenkiHeader_b46a2 header_0x510;
    u32 field_0x518;
    base::UnkAllocator* alloc_0x520;
    u8 field_0x528;
    u8 pad_0x529[7];
    f32 field_0x530;
    f32 field_0x534;
    u8 field_0x538[0x1d];
    f32 field_0x558;
    u64 field_0x560;
    f32 field_0x568;
    u32 field_0x56c;
};

void fn_7100737d58(UnkRedenkiSwarm_b46a2* self);

void fn_710065b090(UnkRedenkiSwarm_b46a2* self) {
    fn_7100737d58(self);
    *reinterpret_cast<void**>(self) = g_unk_7101a5af00 + 0x10;
    for (int i = 0; i < 6; i++) self->field_0x468[i] = 0;
    self->raw_0x498 = 0;
    self->field_0x4a0 = 0xf;
    self->alloc_0x4a8 = nullptr;
    for (int i = 0; i < 5; i++) self->field_0x4b0[i] = 0;
    self->header_0x498.count = 0xa;
    self->header_0x498.flag = 1;
    self->alloc_0x4a8 = fn_7100083990();
    self->field_0x4b0[0] = 0;
    self->field_0x4b0[1] = 0;
    self->field_0x4b0[2] = 0;
    self->field_0x4b0[3] = 0;
    *reinterpret_cast<u32*>(&self->field_0x4b0[4]) = 0;
    self->field_0x4d8 = 0;
    self->field_0x4e0 = 0;
    self->header_0x4e8.count = 0xa;
    self->header_0x4e8.flag = 1;
    self->field_0x4f0 = 0xf;
    self->alloc_0x4f8 = fn_7100083990();
    self->field_0x500 = 0;
    self->field_0x508 = 0;
    self->header_0x510.count = 0xa;
    self->header_0x510.flag = 1;
    self->field_0x518 = 0xf;
    self->alloc_0x520 = fn_7100083990();
    self->field_0x528 = 0;
    self->field_0x530 = 0.0f;
    self->field_0x534 = -1.0f;
    __builtin_memset(self->field_0x538, 0, sizeof(self->field_0x538));
    self->field_0x558 = -1.0f;
    self->field_0x560 = 0;
    self->field_0x568 = -1.0f;
    self->field_0x56c = 0;
    self->vf_0x18();
}
