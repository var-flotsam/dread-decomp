// Unit 211 (tools/tu_map.py): the destructor of the object fn_7100192994 constructs. It deletes the
// objects of its four pointer arrays (giving the decal resources back to their manager) and frees the
// arrays, releases the counted resources, then destroys the holder at +0x210 and the resource vector
// and runs the base destructor. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU211.h"
#include "game/UnkTU216.h"
#include "game/UnkTU226.h"
#include "game/UnkTU228.h"
#include "types.h"

void fn_7100192df4(void* member);

// An object owned through a pointer and deleted through its virtual destructor.
struct UnkOwned_l192b50 {
    virtual ~UnkOwned_l192b50();
};

// An object owned through a pointer whose destructor is out of line.
struct UnkPart_l192b50 {
    ~UnkPart_l192b50() { fn_7100196e7c(this); }
};

// A counted reference to a resource, released when the last one goes.
struct UnkResourceRef_l192b50 {
    UnkCountedResource* ptr;

    ~UnkResourceRef_l192b50() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

struct UnkObject_l192b50 {
    void* vtable;
    u8 base_0x8[0x18];
    u32 object_count;  // 0x20
    u32 decal_count;   // 0x24
    u32 part_count;    // 0x28
    u32 other_count;   // 0x2c
    u32 field_0x30;
    UnkOwned_l192b50** objects;  // 0x38
    UnkDecalResource** decals;   // 0x40
    base::global::CRntVector<UnkResourceRef_l192b50> resources;  // 0x48
    UnkPart_l192b50** parts;     // 0x70
    UnkOwned_l192b50** others;   // 0x78
    u8 pad_0x80[0x190];
    u8 holder[0x48];  // 0x210
};
static_assert(__builtin_offsetof(UnkObject_l192b50, parts) == 0x70, "layout");
static_assert(__builtin_offsetof(UnkObject_l192b50, holder) == 0x210, "layout");

void fn_7100192b50(UnkObject_l192b50* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1dc70) + 0x10;
    base::UnkAllocator* allocator = GetDefaultAllocator();

    for (u32 i = 0; i < self->object_count; ++i) {
        if (self->objects[i]) {
            delete self->objects[i];
            self->objects[i] = nullptr;
        }
    }
    allocator->vf_0x48(self->objects);
    self->objects = nullptr;

    for (u32 i = 0; i < self->decal_count; ++i)
        fn_71001a080c(g_unk_716e352450, self->decals[i]);
    allocator->vf_0x48(self->decals);
    self->decals = nullptr;

    self->resources.Clear();

    for (u32 i = 0; i < self->part_count; ++i) {
        if (UnkPart_l192b50* part = self->parts[i]) {
            delete part;
            self->parts[i] = nullptr;
        }
    }
    allocator->vf_0x48(self->parts);
    self->parts = nullptr;

    for (u32 i = 0; i < self->other_count; ++i) {
        if (self->others[i]) {
            delete self->others[i];
            self->others[i] = nullptr;
        }
    }
    allocator->vf_0x48(self->others);
    self->others = nullptr;

    fn_7100192df4(self->holder);
    self->resources.Free();
    fn_710019e82c(self);
}
