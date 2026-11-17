// Unit 251 (as src/game/UnkSphereOwner.cpp): moves the sphere owner's source (+0x78) to a position (its
// centre and a flag byte at +0x1b8 set), then re-reads the source's centre and radius into the sphere at
// +0x20. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
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

void fn_710019fa64(void* self);

struct UnkSourceRegistry_b218 {
    u64 field_0x0;
    base::global::CRntDictionary<u64, u64> objects;  // 0x8: each object its own value
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
        registry->objects.InsertChecked(reinterpret_cast<u64>(object), reinterpret_cast<u64>(object));
    }
    return reinterpret_cast<UnkSphereSource*>(object);
}
