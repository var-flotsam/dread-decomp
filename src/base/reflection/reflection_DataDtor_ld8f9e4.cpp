// Unit 2525 (base::reflection): the destruction of a 0x3a8-byte record of the environment tables:
// its name id, a file path id and thirteen arrays of polymorphic elements, destroyed in reverse
// order. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// An element with a virtual destructor (its vtable's second entry).
template <u32 Size>
struct UnkPolyElem_d8f9e4 {
    virtual void vf_0x0();
    virtual ~UnkPolyElem_d8f9e4();
    u8 unk_0x8[Size - 8];
};

// An array (base::global::CRntVector's layout) that owns its elements: destroyed with them.
template <class T>
struct UnkOwnedVector_d8f9e4 : base::global::CRntVector<T> {
    ~UnkOwnedVector_d8f9e4() {
        if (this->data) {
            for (u64 i = 0; i < this->count; ++i)
                this->data[i].~T();
            this->count = 0;
            this->allocator->vf_0x48(this->data);
            this->capacity = 0;
        }
    }
};

using UnkElems18_d8f9e4 = UnkOwnedVector_d8f9e4<UnkPolyElem_d8f9e4<0x18>>;
using UnkElems20_d8f9e4 = UnkOwnedVector_d8f9e4<UnkPolyElem_d8f9e4<0x20>>;
using UnkElems28_d8f9e4 = UnkOwnedVector_d8f9e4<UnkPolyElem_d8f9e4<0x28>>;

struct UnkEnvRecord_d8f9e4 {
    u8 unk_0x0[0x40];
    UnkElems20_d8f9e4 field_0x40;
    u8 unk_0x68[0x18];
    UnkElems18_d8f9e4 field_0x80;
    u8 unk_0xa8[0x28];
    UnkElems18_d8f9e4 field_0xd0;
    u8 unk_0xf8[0x20];
    UnkElems18_d8f9e4 field_0x118;
    u8 unk_0x140[0x40];
    UnkElems18_d8f9e4 field_0x180;
    u8 unk_0x1a8[0x20];
    UnkElems18_d8f9e4 field_0x1c8;
    u8 unk_0x1f0[0x10];
    UnkElems18_d8f9e4 field_0x200;
    u8 unk_0x228[0x50];
    UnkElems28_d8f9e4 field_0x278;
    u8 unk_0x2a0[0x8];
    CFilePathStrId path;  // 0x2a8
    UnkElems18_d8f9e4 field_0x2b0;
    u8 unk_0x2d8[0x10];
    UnkElems18_d8f9e4 field_0x2e8;
    u8 unk_0x310[0x30];
    UnkElems18_d8f9e4 field_0x340;
    u8 unk_0x368[0x10];
    UnkElems18_d8f9e4 field_0x378;
    CStrId name;  // 0x3a0
};
static_assert(__builtin_offsetof(UnkEnvRecord_d8f9e4, path) == 0x2a8, "layout");
static_assert(__builtin_offsetof(UnkEnvRecord_d8f9e4, field_0x378) == 0x378, "layout");
static_assert(__builtin_offsetof(UnkEnvRecord_d8f9e4, name) == 0x3a0, "layout");

// The record's members are destroyed one by one, last first.
void fn_7100d8f9e4(void* p) {
    UnkEnvRecord_d8f9e4* self = static_cast<UnkEnvRecord_d8f9e4*>(p);
    self->name.~CStrId();
    self->field_0x378.~UnkElems18_d8f9e4();
    self->field_0x340.~UnkElems18_d8f9e4();
    self->field_0x2e8.~UnkElems18_d8f9e4();
    self->field_0x2b0.~UnkElems18_d8f9e4();
    self->path.~CFilePathStrId();
    self->field_0x278.~UnkElems28_d8f9e4();
    self->field_0x200.~UnkElems18_d8f9e4();
    self->field_0x1c8.~UnkElems18_d8f9e4();
    self->field_0x180.~UnkElems18_d8f9e4();
    self->field_0x118.~UnkElems18_d8f9e4();
    self->field_0xd0.~UnkElems18_d8f9e4();
    self->field_0x80.~UnkElems18_d8f9e4();
    self->field_0x40.~UnkElems20_d8f9e4();
}
