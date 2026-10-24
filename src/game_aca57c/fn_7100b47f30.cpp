// Deleting destructor of an object (vtable 0x7101a9cb48) holding a reference at 0xf0, over a
// base (vtable 0x7101a9c8e0) with two strings (at 0x28, and one with inline storage at 0x70).
#include "base/global/CRntString.h"
#include "types.h"

extern void* g_unk_7101a9cb48[];  // the object's vtable
extern void* g_unk_7101a9c8e0[];  // the base's vtable

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkString {
    u8 unk_0x0[0x48];

    ~UnkString();
};
void fn_710007ed40(UnkString* self);
inline UnkString::~UnkString() { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this)); }

struct UnkInlineString {
    UnkString str;
    u8 buffer[0x38];

    ~UnkInlineString() { fn_710007ed40(&str); }
};

struct UnkStringBase {
    void** vtable;
    u8 unk_0x8[0x20];
    UnkString field_0x28;
    UnkInlineString field_0x70;

    ~UnkStringBase() { vtable = g_unk_7101a9c8e0 + 2; }
};

struct UnkStringHolder : UnkStringBase {
    UnkRefObject* field_0xf0;

    ~UnkStringHolder() {
        vtable = g_unk_7101a9cb48 + 2;
        if (UnkRefObject* p = field_0xf0) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};
static_assert(__builtin_offsetof(UnkStringHolder, field_0xf0) == 0xf0, "UnkStringHolder layout");

// NON_MATCHING(minor): ours keeps the address of the string at 0x28 in a register across the
// calls; the original keeps the one at 0x70 and recomputes the other.
void fn_7100b47f30(UnkStringHolder* self) {
    self->~UnkStringHolder();
    operator delete(self);
}
