// Unit 251 (as src/game/UnkSphereOwner.cpp): moves the sphere owner's source (+0x78) to a position (its
// centre and a flag byte at +0x1b8 set), then re-reads the source's centre and radius into the sphere at
// +0x20. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkSphere.h"
#include "game/UnkTU226.h"
#include "game/UnkTU227.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSphereSource_b218 {
    u8 pad_0x0[0x24];
    UnkVec3f position;  // 0x24
    u8 pad_0x30[0x188];
    u8 field_0x1b8;
};

struct UnkSphereSource;

struct UnkSphereOwner_b218 {
    u8 pad_0x0[0x20];
    UnkSphere sphere;
    u8 pad_0x70[0x8];
    UnkSphereSource_b218* source;  // 0x78
};

void fn_71001bdd50(UnkSphereOwner_b218* self, const UnkVec3f* position) {
    fn_7100216978(&self->sphere, position);
    UnkSphereSource_b218* source = self->source;
    source->position.x = position->x;
    source->position.y = position->y;
    source->position.z = position->z;
    *(reinterpret_cast<u8*>(source) + 0x1b8) = 1;  // an untyped store: the source pointer is read again after it
    UnkVec3f center(0.0f, 0.0f, 0.0f);
    f32 radius = -1.0f;
    fn_710019fc40(reinterpret_cast<UnkSphereSource*>(self->source), &center, &radius);
    fn_7100216978(&self->sphere, &center);
    fn_710021693c(&self->sphere, radius);
}

// ---- Source creation ----

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

void fn_710019fa64(void* self);

struct UnkSourceNode_b218 {
    u64 key;
    u64 value;
    UnkSourceNode_b218* next;
    UnkSourceNode_b218* prev;
    UnkSourceNode_b218* chain;

    explicit UnkSourceNode_b218(u64 k) : key(k), value(k), next(nullptr), prev(nullptr), chain(nullptr) {}

    UnkSourceNode_b218*& Next() { return next; }
    UnkSourceNode_b218*& Prev() { return prev; }
};

struct UnkSourceRegistry_b218 {
    u64 field_0x0;
    base::UnkAllocator* allocator;  // 0x8
    u32 flags;                      // 0x10
    u32 count;                      // 0x14
    UnkSourceNode_b218** buckets;   // 0x18
    u32 bucket_count;               // 0x20
    u32 bucket_capacity;            // 0x24
    u8 pad_0x28[0x18];
    UnkSourceNode_b218* tail;  // 0x40
    UnkSourceNode_b218* head;  // 0x48

    void Insert(u64 key) {
        UnkSourceNode_b218** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            if (bucket_count <= i)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = &buckets[i];
        }
        UnkSourceNode_b218* old = *bucket;
        auto* node = static_cast<UnkSourceNode_b218*>(allocator->vf_0x40(sizeof(UnkSourceNode_b218), 8, flags));
        if (node)
            new (node) UnkSourceNode_b218(key);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->Prev() = tail;
            tail->Next() = node;
        }
        tail = node;
        ++count;
    }
};

struct UnkSourceObject_b218 {
    u8 pad_0x0[8];
    CStrId id;  // 0x8
};

UnkSphereSource* fn_71001bd7bc(void* registry_, void* key, UnkReflObject* obj, u32 kind) {
    UnkSourceRegistry_b218* registry = static_cast<UnkSourceRegistry_b218*>(registry_);
    UnkSourceObject_b218* object = static_cast<UnkSourceObject_b218*>(fn_71000839a0(0x1c8, kind));
    fn_710019fa64(object);
    if (object) {
        object->id = *static_cast<CStrId*>(key);
        if (obj)
            fn_710019e95c(object, obj);
        registry->Insert(reinterpret_cast<u64>(object));
    }
    return reinterpret_cast<UnkSphereSource*>(object);
}
