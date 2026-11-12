// CLogicShapeComponent's unit (tools/tu_map.py). A flat map of bools keyed by string id (values at +0,
// keys at +8, count at +0x10, capacity, step and allocator flags after them, the allocator at +0x20;
// CLogicShapeComponent_MapSet_A5_b66.cpp sets through it): erase by key, the pointer to a key's value
// or null, erase at an iterator (the last entry moves into the hole), shrink to fit (as
// src/base/reflection/UnkUnit83/UnkUnit83_IdKeyedShrink_b62a1.cpp) and copy assignment (reserve
// fn_71007531f8). Also the component's load (after the base read fn_710024fc0c, a collision reader
// builds the shape at +0x30; a shape of kind 0x10 gets its flag at +0x98 set) and a reflected
// invoker answering whether an object lies in the shape (the test of
// CLogicShapeComponent_Test_A5_b68.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CActorComponent.h"
#include "game/UnkTU2505.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "types.h"

#include <new>

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkLogicShapeFlagMap_ovb0037 {
    bool* values;
    CStrId* keys;
    u32 count;
    u32 capacity;  // 0x14
    u32 step;      // 0x18
    u32 flags;     // 0x1c
    base::UnkAllocator* allocator;  // 0x20

    bool* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return values + count;
    }
    bool* End() { return values + count; }
};

bool* fn_7100916c9c(UnkLogicShapeFlagMap_ovb0037* self, bool* it);
void fn_71007531f8(UnkLogicShapeFlagMap_ovb0037* self, u32 capacity);

void fn_7100916418(UnkLogicShapeFlagMap_ovb0037* self, const CStrId* key) {
    bool* it = self->Find(*key);
    if (it != self->End())
        fn_7100916c9c(self, it);
}

bool* fn_7100916488(UnkLogicShapeFlagMap_ovb0037* self, const CStrId* key) {
    bool* it = self->Find(*key);
    return it != self->End() ? it : nullptr;
}

bool* fn_7100916c9c(UnkLogicShapeFlagMap_ovb0037* self, bool* it) {
    u32 index = it - self->values;
    self->keys[index].Reset();
    u32 last = self->count - 1;
    if (last != index) {
        __builtin_memcpy(&self->values[index], &self->values[last], 1);
        CStrId* keys = self->keys;
        u32 back = self->count - 1;
        __builtin_memcpy(&keys[index].data, &keys[back].data, 8);
        UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&keys[back].data, &empty, 8);
        self->keys[self->count - 1].Reset();
    }
    --self->count;
    return self->values + index;
}

void fn_7100916b40(UnkLogicShapeFlagMap_ovb0037* self) {
    if (self->count == self->capacity)
        return;
    bool* values = static_cast<bool*>(self->allocator->vf_0x40(self->count, 8, self->flags));
    auto* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (self->values) {
        __builtin_memcpy(values, self->values, self->count);
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&keys[j]) CStrId(*old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = self->count;
}

UnkLogicShapeFlagMap_ovb0037* fn_71009169dc(UnkLogicShapeFlagMap_ovb0037* self, const UnkLogicShapeFlagMap_ovb0037* other) {
    if (self == other)
        return self;
    if (self->keys) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].Reset();
    }
    self->count = 0;
    u32 count = other->count;
    if (count != self->capacity) {
        fn_7100916b40(self);
        fn_71007531f8(self, other->count);
        count = other->count;
    }
    __builtin_memcpy(self->values, other->values, count);
    self->count = other->count;
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        CStrId* key = &self->keys[i];
        __builtin_memcpy(key, &other->keys[i], sizeof(*key));
        if (&g_unk_7101d47108->empty != key->data)
            ++key->data->ref_count;
    }
    return self;
}

using base::global::CRntFile;

struct UnkPickableItem_A4_b76;

struct UnkCollisionFileReader;

struct UnkLogicShape_ovb0037 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys and frees it
    virtual s32 vf_0x18();   // its kind
};

struct UnkLogicShapeKind10_ovb0037 : UnkLogicShape_ovb0037 {
    u8 pad_0x8[0x90];
    bool field_0x98;
};
static_assert(__builtin_offsetof(UnkLogicShapeKind10_ovb0037, field_0x98) == 0x98, "layout");

// An owning pointer to a shape (freed through its vf_0x10).
struct UnkLogicShapePtr_ovb0037 {
    UnkLogicShape_ovb0037* ptr;

    ~UnkLogicShapePtr_ovb0037() {
        UnkLogicShape_ovb0037* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x10();
    }
};

UnkLogicShapePtr_ovb0037 fn_7100d46440(UnkCollisionFileReader* self, CRntFile* file);

struct UnkLogicShapeComponent_ovb0037 {
    u8 pad_0x0[0x30];
    UnkLogicShape_ovb0037* shape;  // 0x30
};

void fn_7100914284(UnkLogicShapeComponent_ovb0037* self, CRntFile* file) {
    fn_710024fc0c(reinterpret_cast<UnkPickableItem_A4_b76*>(self), file);
    if (file->position < file->size) {
        alignas(8) u8 reader_storage[0x18];
        auto* reader = reinterpret_cast<UnkCollisionFileReader*>(reader_storage);
        fn_7100d45fc4(reader, self);
        {
            UnkLogicShapePtr_ovb0037 shape = fn_7100d46440(reader, file);
            UnkLogicShape_ovb0037* p = shape.ptr;
            shape.ptr = nullptr;
            UnkLogicShape_ovb0037* old = self->shape;
            self->shape = p;
            if (old)
                old->vf_0x10();
        }
        if (self->shape->vf_0x18() == 0x10) {
            UnkLogicShape_ovb0037* s = self->shape;
            auto* k = s->vf_0x18() == 0x10 ? static_cast<UnkLogicShapeKind10_ovb0037*>(s) : nullptr;
            k->field_0x98 = true;
        }
        fn_7100d45fd4(reader);
    }
}

using base::reflection::UnkReflValue;

struct UnkShapeTester_A5_b68;

// Returned in memory (not trivially copyable); only bit 0 of the first word is read.
struct UnkShapeHit_A5_b68 {
    u32 flags;  // bit 0: hit
    u8 pad_0x4[0xc];
    UnkShapeHit_A5_b68(const UnkShapeHit_A5_b68& other);
};

struct UnkShapeObject_ovb0037 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

static inline base::math::CVector3D Position_ovb0037(const UnkShapeObject_ovb0037* obj) {
    base::math::CVector3D r;
    r.x = obj->x;
    r.y = obj->y;
    r.z = obj->z;
    return r;
}

struct UnkLogicShapeTest_ovb0037 {
    u8 pad_0x0[0x30];
    void* shape;  // 0x30

    bool Contains(const UnkShapeObject_ovb0037* obj) {
        if (!obj || !shape)
            return false;
        UnkShapeTester_A5_b68* tester = g_unk_71726a47a0;
        if (!tester) {
            tester = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
            fn_7100d5134c(tester);
            g_unk_71726a47a0 = tester;
        }
        base::math::CVector2D pos;
        {
            base::math::CVector3D v = Position_ovb0037(obj);
            fn_710007b430(&pos, &v);
        }
        return fn_7100d572f8(tester, shape, &pos, &g_unk_710164779c, 0).flags & 1;
    }
};

UnkReflValue fn_7100915970(void*, char* obj, UnkReflValue* const* args) {
    UnkLogicShapeTest_ovb0037* self = *reinterpret_cast<UnkLogicShapeTest_ovb0037**>(obj + 8);
    const UnkShapeObject_ovb0037* target = *base::reflection::refl_ArgValue<const UnkShapeObject_ovb0037*>(args[0]);
    bool value = self->Contains(target);
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
