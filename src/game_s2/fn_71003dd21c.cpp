// Constructor of an object derived from fn_7100393e68's: sets its vtable, a pair of -1 floats and
// a few zeroed members, constructs the sub-object at +0x50 (fn_71002acfe4), clears 17 bytes at
// +0x12ad8, stores a second pair of -1 floats at +0x12aec and finishes in vtable slot 0x18.
#include "types.h"

typedef u64 u64_a4 __attribute__((aligned(4)));
typedef void** __attribute__((may_alias)) UnkVtable;

struct UnkNode {
    UnkVtable vtable;
    u8 pad_0x8[0x24];
    u64_a4 field_0x2c;
    u64_a4 field_0x34;
    u16 field_0x3c;
    u8 pad_0x3e[2];
    u64 field_0x40;
    u8 pad_0x48[8];
    u8 field_0x50[0x12ad8 - 0x50];
    u64 field_0x12ad8;
    u64 field_0x12ae0;
    u8 field_0x12ae8;
    u8 pad_0x12ae9[3];
    u64_a4 field_0x12aec;

    void vf_0x18() { reinterpret_cast<void (*)(UnkNode*)>(vtable[3])(this); }
};

void fn_7100393e68(UnkNode*);
void fn_71002acfe4(void* sub);
extern void* g_unk_7101a2f0d8[];

// NON_MATCHING(minor): the stores at +0x3c and +0x40 are made in the other order
void fn_71003dd21c(UnkNode* self) {
    fn_7100393e68(self);
    self->field_0x2c = 0xbf800000bf800000ull;
    self->vtable = g_unk_7101a2f0d8 + 2;
    self->field_0x34 = 0xbf800000ull;
    self->field_0x3c = 0;
    self->field_0x40 = 0;
    fn_71002acfe4(self->field_0x50);
    self->field_0x12ae8 = 0;
    self->field_0x12ad8 = 0;
    self->field_0x12ae0 = 0;
    self->field_0x12aec = 0xbf800000bf800000ull;
    self->vf_0x18();
}
