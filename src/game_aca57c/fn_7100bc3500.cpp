// Copy constructors of two classes derived from one middle class: the base is copied out of
// line (fn_7100067ebc), then the middle class's string ids at 0x10 and 0x18, file path at 0x20,
// refcounted pointer at 0x28 (count at 0x54, freed by fn_710001737c) and 9 plain bytes at 0x30
// (copied as one block); the class's own vtable last. Two identical copies each.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

void fn_7100067ebc(char* self, const char* other);

struct UnkShared54Ptr {
    UnkCountedResource* ptr;

    UnkShared54Ptr(const UnkShared54Ptr& other) {
        ptr = nullptr;
        *this = other.ptr;
    }
    UnkShared54Ptr& operator=(UnkCountedResource* p) {
        if (p != ptr) {
            if (p)
                ++p->ref_count;
            if (UnkCountedResource* old = ptr) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            ptr = p;
        }
        return *this;
    }
};

extern char unk_7101aa15b8[];  // the middle class's vtable
extern char unk_7101aa1588[];
extern char unk_7101aa15e8[];

struct UnkPathRefRecord {
    char* vtable;
    u8 unk_0x8[0x8];
    CStrId a;
    CStrId b;
    CFilePathStrId path;
    UnkShared54Ptr shared;
    u64 field_0x30;
    u8 field_0x38;
};

template <char* Vtable>
__attribute__((always_inline)) static inline void CopyRecord(UnkPathRefRecord* self, const UnkPathRefRecord* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = unk_7101aa15b8 + 0x10;
    new (&self->a) CStrId(other->a);
    new (&self->b) CStrId(other->b);
    new (&self->path) CFilePathStrId(other->path);
    new (&self->shared) UnkShared54Ptr(other->shared);
    __builtin_memcpy(&self->field_0x30, &other->field_0x30, 9);
    self->vtable = Vtable + 0x10;
}

void fn_7100bc3500(UnkPathRefRecord* self, const UnkPathRefRecord* other) { CopyRecord<unk_7101aa1588>(self, other); }
void fn_7100bc3608(UnkPathRefRecord* self, const UnkPathRefRecord* other) { CopyRecord<unk_7101aa1588>(self, other); }
void fn_7100bc4048(UnkPathRefRecord* self, const UnkPathRefRecord* other) { CopyRecord<unk_7101aa15e8>(self, other); }
void fn_7100bc4150(UnkPathRefRecord* self, const UnkPathRefRecord* other) { CopyRecord<unk_7101aa15e8>(self, other); }
