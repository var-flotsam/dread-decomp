// In the animtree::CPoseNode unit: creates a 0x48-byte child object (its vtable, a cleared word and
// a member copied, by fn_7100014dfc, from a temporary built by fn_7100014d94 with 0), registers it
// with fn_71002dba54 and keeps what that returns at +0x28. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

void fn_7100014d94(void* self, u32 arg);
void fn_7100014dfc(void* self, const void* other);
void fn_7100014e88(void* member);

extern char g_unk_7101a220c8[];

struct UnkPoseChild {
    char* vtable;
    u64 field_0x8;
    alignas(8) u8 member[0x38];
};

struct UnkPoseNodeOwner {
    u8 unk_0x0[0x28];
    void* child;
};

void* fn_71002dba54(UnkPoseNodeOwner* self, UnkPoseChild* child);

void fn_71002e8310(UnkPoseNodeOwner* self) {
    UnkPoseChild* child = static_cast<UnkPoseChild*>(operator new(sizeof(UnkPoseChild)));
    alignas(8) u8 temp[0x38];
    fn_7100014d94(temp, 0);
    child->vtable = g_unk_7101a220c8 + 0x10;
    child->field_0x8 = 0;
    fn_7100014dfc(child->member, temp);
    self->child = fn_71002dba54(self, child);
    fn_7100014e88(temp);
}
