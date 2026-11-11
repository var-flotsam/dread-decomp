// CCharClassLavaPoolComponent unit (unit 1393 of tools/tu_map.py): the update, fn_71008fc438. After
// the base update (fn_71010c4234), unless paused (+0xcb1) and while changing (+0xcb0): once the time
// (+0xcb8) is past the duration (+0xcbc) and the level (+0xcb4) is within 0.001 of 1 or of 0, the
// change ends (vtable slot 0x238, flag cleared); otherwise vtable slot 0x230 is told whether the
// target (+0xcc0) is not above zero and the time runs on. Names and layouts are placeholders.
#include "game/CComponent.h"
#include "types.h"

struct UnkLavaPool_b52 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual void vf_0x230(bool below);
    virtual void vf_0x238();
    u8 unk_0x8[0xca8];
    bool changing;  // 0xcb0
    bool paused;    // 0xcb1
    u8 unk_0xcb2[2];
    f32 level;     // 0xcb4
    f32 time;      // 0xcb8
    f32 duration;  // 0xcbc
    f32 target;    // 0xcc0
};

static inline bool Near_b52(f32 v, f32 to) {
    return v - 0.001f <= to && v + 0.001f >= to;
}

void fn_71008fc438(UnkLavaPool_b52* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    if (self->paused || !self->changing)
        return;
    if (self->time > self->duration && (Near_b52(self->level, 1.0f) || Near_b52(self->level, 0.0f))) {
        self->vf_0x238();
        self->changing = false;
        return;
    }
    self->vf_0x230(self->target <= 0.0f);
    self->time += dt;
}
