// CEnemyMovement unit (unit 2686 of tools/tu_map.py): sets two flags at 0x19f8, then either notifies the
// object at 0x848 (slot 0xa0) when the flag at 0x1c11 is set, or marks 0x1c12 pending. Names and layouts are
// placeholders.
#include "types.h"

struct UnkEnemyMovementTarget {
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
};

struct UnkEnemyMovementFlags {
    u8 pad_0x0[0x848];
    UnkEnemyMovementTarget* target;
    u8 pad_0x850[0x19f8 - 0x850];
    bool flag_0x19f8;
    bool flag_0x19f9;
    u8 pad_0x19fa[0x1c11 - 0x19fa];
    bool flag_0x1c11;
    bool flag_0x1c12;
};

void fn_71010c9b08(UnkEnemyMovementFlags* self) {
    self->flag_0x19f8 = true;
    self->flag_0x19f9 = true;
    if (self->flag_0x1c11) {
        if (UnkEnemyMovementTarget* target = self->target)
            target->vf_0xa0();
    } else {
        self->flag_0x1c12 = true;
    }
}
