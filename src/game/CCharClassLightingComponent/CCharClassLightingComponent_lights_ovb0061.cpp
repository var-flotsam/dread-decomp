// CCharClassLightingComponent's unit (tools/tu_map.py): a per-light call over the array at +0x60 (as in
// CCharClassLightingComponent_b36.cpp), the component's destructor and the setter of its light set's path
// (fn_71008e756c). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CCharClassLightingComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2527.h"
#include "types.h"

struct UnkLightDesc_ovb0061 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkLightDescHolder_ovb0061 {
    u8 pad_0x0[0x8];
    UnkLightDesc_ovb0061* desc;
};

struct UnkLight_ovb0061 {
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
    virtual void vf_0x70(void* a, void* b);
    UnkLightDescHolder_ovb0061* holder;  // +0x8
    u8 pad_0x10[0x8];
    s32 state;  // +0x18
};

struct UnkLightingComponent_ovb0061 {
    u8 pad_0x0[0x60];
    UnkLight_ovb0061** lights;  // +0x60
    u32 count;                  // +0x68
};

// Passes `a` and `b` to slot 0x70 of each light whose definition is `id` and whose state is 2.
void fn_71008e8188(UnkLightingComponent_ovb0061* self, const u64* id, void* a, void* b) {
    UnkLight_ovb0061** end = self->lights + self->count;
    for (UnkLight_ovb0061** it = self->lights; it != end; ++it) {
        UnkLight_ovb0061* light = *it;
        UnkLightDesc_ovb0061* desc = light->holder->desc;
        u64 key = *id;
        if (desc->id == key && light->state == 2)
            light->vf_0x70(a, b);
    }
}

extern void* g_unk_7101a7f790[];  // the component's vtable

// The id and path pair at +0x48 with its resource: fn_7100027044, then the resource's count, then the ids.
struct UnkLightPath_ovb0061 {
    UnkStrIdPathResource value;
    __attribute__((always_inline)) ~UnkLightPath_ovb0061() {
        fn_7100027044(&value);
        if (UnkCountedResource* r = value.resource) {
            if (--r->ref_count == 0)
                fn_710001737c(r);
        }
    }
};

struct UnkLightingComponentDtor_ovb0061 {
    void** vtable;
    u8 pad_0x8[0x38];
    base::UnkStrIdMemberOf<g_unk_7101d47200> field_0x40;
    UnkLightPath_ovb0061 path;                     // +0x48
    base::UnkVector<UnkLight_ovb0061*> lights;     // +0x60
};
static_assert(__builtin_offsetof(UnkLightingComponentDtor_ovb0061, lights) == 0x60, "layout");

// The destructor: frees the light array, destroys the path pair and the id at +0x40, then runs the base
// component destructor (fn_71010c4194).
void fn_71008e8238(UnkLightingComponentDtor_ovb0061* self) {
    self->vtable = &g_unk_7101a7f790[2];
    self->lights.Free();
    self->path.~UnkLightPath_ovb0061();
    self->field_0x40.~UnkStrIdMemberOf();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

// As src/base/global/CRntString.cpp, src/base/UnkTunableRegistry.cpp and
// src/game/CDroppableComponent/CDroppableComponent_b50.cpp declare them.
struct UnkPooledStrId;
using base::global::CRntString;
void fn_710007eb20(CRntString* self, const UnkPooledStrId* id);
void fn_710007f564(void* self, const char* str);
struct UnkPathIds;
struct UnkInlineString;

// The component (as CCharClassLightingComponent_b36.cpp declares the function).
struct UnkLightingComponent3;

// An id as a string (fn_710007eb20), destroyed as one.
struct UnkLightIdText_ovb0061 {
    union {
        base::global::CRntString str;
    };
    explicit UnkLightIdText_ovb0061(const void* id) {
        fn_710007eb20(&str, static_cast<const UnkPooledStrId*>(id));
    }
    ~UnkLightIdText_ovb0061() { fn_710007ecd4(&str); }
};

// The ids the builder fills in place, destroyed in reverse order (as in CDroppableComponent_b50.cpp).
struct UnkLightBuiltIds_ovb0061 {
    union {
        base::global::CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkLightBuiltIds_ovb0061(const void* source) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(this), static_cast<const UnkInlineString*>(source));
    }
    ~UnkLightBuiltIds_ovb0061() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

// Takes the light set's path (+0x58 of the CCharClassLightingComponent) and resolves the resource
// "lightsdef:<path>" into the pair at +0x48.
void fn_71008e756c(UnkLightingComponent3* component, void* arg) {
    UnkLightingComponentDtor_ovb0061* self = reinterpret_cast<UnkLightingComponentDtor_ovb0061*>(component);
    self->field_0x40 = *static_cast<const base::UnkStrIdMemberOf<g_unk_7101d47200>*>(arg);
    UnkLightIdText_ovb0061 text(g_unk_71727b9f30);
    fn_710007f564(&text.str, ":");
    fn_710007f3b4(&text.str, &base::reflection::refl_RntStringOf(
                                 reinterpret_cast<const base::global::CStrId&>(self->field_0x40)));
    fn_71000267f8(&self->path.value, reinterpret_cast<const UnkStrIdPathPair&>(static_cast<const UnkLightBuiltIds_ovb0061&>(
                                         UnkLightBuiltIds_ovb0061(&text))));
}
