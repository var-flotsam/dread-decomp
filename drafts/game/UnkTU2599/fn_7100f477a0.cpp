// Unit 2599 (hint 'CEnvironmentData::SFogTransition'): CEnvironmentData's move assignment: the id and
// the cube map's texture path moved, the settings blocks copied, the twelve transition vectors
// copy-assigned element by element. Names and layouts are placeholders (the field list as
// src/game/CEnvironmentData/CEnvironmentData_Move.cpp).
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkStrIdValueVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "game/UnkTU2599.h"
#include "types.h"

using base::UnkStrIdValueElem;
using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

void fn_7100f79bb0(void*, u32);
void fn_7100f79db4(void*, u32);
void fn_7100f79fb8(void*, u32);
void fn_7100f7a1bc(void*, u32);
void fn_7100f7a3c0(void*, u32);
void fn_7100f7a5c4(void*, u32);
void fn_7100f7a9d8(void*, u32);
void fn_7100f7abdc(void*, u32);
void fn_7100f7ade4(void*, u32);
void fn_7100f7afec(void*, u32);
char* fn_7100f44150(void*, u32);
char* fn_7100f442c4(void*, u32);
char* fn_7100f44438(void*, u32);
char* fn_7100f445ac(void*, u32);
char* fn_7100f44720(void*, u32);
char* fn_7100f44894(void*, u32);
char* fn_7100f44a08(void*, u32);
char* fn_7100f44b7c(void*, u32);
char* fn_7100f44cf0(void*, u32);
char* fn_7100f44e64(void*, u32);

namespace env_move_assign {

// A colour: assigned out of line (fn_710007b518, CVector4D's assignment).
struct UnkColor : base::math::CVector4D {
    UnkColor& operator=(const UnkColor& other) {
        fn_710007b518(this, &other);
        return *this;
    }
};

// A 3D vector assigned component by component.
struct UnkVec3 : base::math::CVector3D {
    UnkVec3& operator=(const UnkVec3& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

// A 2D vector assigned component by component.
struct UnkVec2 : base::math::CVector2D {
    UnkVec2& operator=(const UnkVec2& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

struct SFog {
    f32 field_0x0;
    UnkColor tColor;  // 0x4
    f32 field_0x14, field_0x18, field_0x1c;
    UnkVec2 field_0x20;
    UnkColor field_0x28;
    f32 field_0x38;
};

struct SAmbient {
    f32 fColorInterp;
    UnkColor tColor;  // 0x4
};

struct SDepthTint {
    f32 field_0x0[9];
};

struct SMaterialTint {
    f32 fColorInterp;
    UnkColor tColor;  // 0x4
    f32 fBlend, fBlendInterp;
};

struct SPlayerLight {
    f32 fColorInterp;
    UnkColor tDiffuse, tSpecular0, tSpecular1;  // 0x4, 0x14, 0x24
    UnkVec3 field_0x34;
};

struct SHemisphericalLight {
    f32 fColorUpInterp;
    UnkVec3 tColorUp;  // 0x4
    f32 fColorDownInterp;
    UnkVec3 tColorDown;  // 0x14
};

struct SBloom {
    f32 fBloomInterp;
    UnkVec3 tBloom;  // 0x4
};

struct SVerticalFog {
    bool bEnabled;
    UnkColor tColor;  // 0x4
    f32 field_0x14, field_0x18, field_0x1c;
    UnkVec2 field_0x20;
    f32 field_0x28;
    UnkColor tAttenuation;  // 0x2c
    f32 field_0x3c;
    f32 field_0x40, field_0x44, field_0x48, field_0x4c;
};

struct SCubeMap {
    f32 fInterp;
    bool bEnabled;
    CFilePathStrId sTexturePath;  // 0x8
};

struct SSSAO {
    f32 fFallOff, fIntensity, fBias;
};

struct SToneMapping {
    f32 fInterp;
    f32 field_0x4, field_0x8, field_0xc, field_0x10, field_0x14;
    UnkColor field_0x18;
    f32 field_0x28;
    bool field_0x2c;
};

struct SIBLAttenuation {
    f32 fInterp, fCubeAttFactor, fZDistance, fGradientSize;
};

// The transitions' values after their base (vtable, preset id).
struct UnkInterp1 {
    f32 a;
};
struct UnkInterp2 {
    f32 a, b;
};
struct UnkInterp4 {
    f32 a, b, c, d;
};
struct UnkInterp6 {
    f32 a, b, c, d, e, f;
};

template <int Id, class T>
struct UnkTransitionTag {};

template <int Id, class T>
using UnkTransitions = CRntVector<UnkStrIdValueElem<UnkTransitionTag<Id, T>, T>>;

// Moves the elements to a new block of `n` (copy, then destroy the old one).
template <class Tag, class T>
static void Reallocate(CRntVector<UnkStrIdValueElem<Tag, T>>* v, u32 n) {
    using E = UnkStrIdValueElem<Tag, T>;
    E* p = static_cast<E*>(v->allocator->vf_0x40((u64)n * sizeof(E), 8, v->flags));
    if (E* s = v->data) {
        E* o = p;
        for (u32 i = 0; i < v->count; ++i, ++s, ++o) {
            new (o) E(*s);
            s->~E();
        }
        v->allocator->vf_0x48(v->data);
    }
    v->data = p;
    v->capacity = n;
}

// The element at `index`, growing the array to hold it when it is growable.
template <class Tag, class T>
static UnkStrIdValueElem<Tag, T>& GrowAt(CRntVector<UnkStrIdValueElem<Tag, T>>* v, u32 index) {
    if (v->field_0x14 && v->count <= index) {
        u32 cap = v->capacity;
        if (cap <= index && v->step != 0) {
            u32 grown = (index / v->step + 1) * v->step;
            if (cap < grown)
                Reallocate(v, grown);
        }
        base::UnkStrIdValueTraits<Tag>::resize(v, index + 1);
    }
    return v->data[index];
}

// The element at `index` for the element types whose grow-at is inline; the others' out-of-line
// grow-at overloads follow.
template <int Id, class T>
static UnkStrIdValueElem<UnkTransitionTag<Id, T>, T>& ElementAt(UnkTransitions<Id, T>* v, u32 index) {
    return GrowAt(v, index);
}

// The source's element: within its count, so it never grows (the called grow-at is still called).
template <int Id, class T>
static UnkStrIdValueElem<UnkTransitionTag<Id, T>, T>& SourceAt(UnkTransitions<Id, T>* v, u32 index) {
    return v->data[index];
}

}  // namespace env_move_assign

// An element type whose grow-at is inline (the fog and vertical fog transitions).
#define DREAD_INLINE_TRANSITION(ID, T, VTABLE, RESIZE)                                                       \
    template <>                                                                                              \
    struct base::UnkStrIdValueTraits<env_move_assign::UnkTransitionTag<ID, env_move_assign::T>> {            \
        using Vec = env_move_assign::UnkTransitions<ID, env_move_assign::T>;                                 \
        static char* base_vtable() { return g_unk_7101cab1f8; }                                              \
        static char* vtable() { return VTABLE; }                                                             \
        static void resize(Vec* v, u32 n) { RESIZE(v, n); }                                                  \
    };

// One whose grow-at is called.
#define DREAD_CALLED_TRANSITION(ID, T, RESIZE, GROW_AT)                                                      \
    template <>                                                                                              \
    struct base::UnkStrIdValueTraits<env_move_assign::UnkTransitionTag<ID, env_move_assign::T>> {            \
        using Vec = env_move_assign::UnkTransitions<ID, env_move_assign::T>;                                 \
        static void resize(Vec* v, u32 n) { RESIZE(v, n); }                                                  \
    };                                                                                                       \
    namespace env_move_assign {                                                                              \
    static UnkStrIdValueElem<UnkTransitionTag<ID, T>, T>& ElementAt(UnkTransitions<ID, T>* v, u32 i) {       \
        return *reinterpret_cast<UnkStrIdValueElem<UnkTransitionTag<ID, T>, T>*>(GROW_AT(v, i));             \
    }                                                                                                        \
    static UnkStrIdValueElem<UnkTransitionTag<ID, T>, T>& SourceAt(UnkTransitions<ID, T>* v, u32 i) {        \
        return *reinterpret_cast<UnkStrIdValueElem<UnkTransitionTag<ID, T>, T>*>(GROW_AT(v, i));             \
    }                                                                                                        \
    }

DREAD_INLINE_TRANSITION(0, UnkInterp4, g_unk_7101cab220, fn_7100f799b0)
DREAD_CALLED_TRANSITION(1, UnkInterp1, fn_7100f79bb0, fn_7100f44150)
DREAD_CALLED_TRANSITION(2, UnkInterp1, fn_7100f79db4, fn_7100f442c4)
DREAD_CALLED_TRANSITION(3, UnkInterp2, fn_7100f79fb8, fn_7100f44438)
DREAD_CALLED_TRANSITION(4, UnkInterp2, fn_7100f7a1bc, fn_7100f445ac)
DREAD_CALLED_TRANSITION(5, UnkInterp2, fn_7100f7a3c0, fn_7100f44720)
DREAD_CALLED_TRANSITION(6, UnkInterp1, fn_7100f7a5c4, fn_7100f44894)
DREAD_INLINE_TRANSITION(7, UnkInterp6, g_unk_7101cab338, fn_7100f7a7c8)
DREAD_CALLED_TRANSITION(8, UnkInterp1, fn_7100f7a9d8, fn_7100f44a08)
DREAD_CALLED_TRANSITION(9, UnkInterp1, fn_7100f7abdc, fn_7100f44b7c)
DREAD_CALLED_TRANSITION(10, UnkInterp1, fn_7100f7ade4, fn_7100f44cf0)
DREAD_CALLED_TRANSITION(11, UnkInterp1, fn_7100f7afec, fn_7100f44e64)

namespace env_move_assign {

// A transition vector's copy assignment, element by element: the count first (growing out of line or
// destroying the extra elements), then each element through the grow-at accessor on both sides.
template <int Id, class T>
static void AssignTransitions(UnkTransitions<Id, T>* self, UnkTransitions<Id, T>* other) {
    using Traits = base::UnkStrIdValueTraits<UnkTransitionTag<Id, T>>;
    using Elem = UnkStrIdValueElem<UnkTransitionTag<Id, T>, T>;
    u32 n = other->count;
    if (self->count < n) {
        Traits::resize(self, n);
    } else if (self->count > n) {
        for (u32 i = n; i < self->count; ++i)
            self->data[i].~Elem();
        self->count = n;
    }
    for (u32 i = 0; i < other->count; ++i)
        ElementAt(self, i) = SourceAt(other, i);
}

struct CEnvironmentData {  // 0x3a8
    SFog tFog;
    UnkTransitions<0, UnkInterp4> tFogTransitions;  // 0x40
    SAmbient tAmbient;                               // 0x68
    UnkTransitions<1, UnkInterp1> tAmbientTransitions;
    SDepthTint tDepthTint;  // 0xa8
    UnkTransitions<2, UnkInterp1> tDepthTintTransitions;
    SMaterialTint tMaterialTint;  // 0xf8
    UnkTransitions<3, UnkInterp2> tMaterialTintTransitions;
    SPlayerLight tPlayerLight;  // 0x140
    UnkTransitions<4, UnkInterp2> tPlayerLightTransitions;
    SHemisphericalLight tHemisphericalLight;  // 0x1a8
    UnkTransitions<5, UnkInterp2> tHemisphericalLightTransitions;
    SBloom tBloom;  // 0x1f0
    UnkTransitions<6, UnkInterp1> tBloomTransitions;
    SVerticalFog tVerticalFog;  // 0x228
    UnkTransitions<7, UnkInterp6> tVerticalFogTransitions;
    SCubeMap tCubeMap;  // 0x2a0
    UnkTransitions<8, UnkInterp1> tCubeMapTransitions;
    SSSAO tSSAO;  // 0x2d8
    UnkTransitions<9, UnkInterp1> tSSAOTransitions;
    SToneMapping tToneMapping;  // 0x310
    UnkTransitions<10, UnkInterp1> tToneMappingTransitions;
    SIBLAttenuation tIBLAttenuation;  // 0x368
    UnkTransitions<11, UnkInterp1> tIBLAttenuationTransitions;
    CStrId sID;  // 0x3a0
};
static_assert(__builtin_offsetof(CEnvironmentData, tAmbient) == 0x68, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tDepthTint) == 0xa8, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tMaterialTint) == 0xf8, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tPlayerLight) == 0x140, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tHemisphericalLight) == 0x1a8, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tBloom) == 0x1f0, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tVerticalFog) == 0x228, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tCubeMap) == 0x2a0, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tSSAO) == 0x2d8, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tToneMapping) == 0x310, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, tIBLAttenuation) == 0x368, "CEnvironmentData layout");
static_assert(__builtin_offsetof(CEnvironmentData, sID) == 0x3a0, "CEnvironmentData layout");
static_assert(sizeof(CEnvironmentData) == 0x3a8, "CEnvironmentData layout");

}  // namespace env_move_assign

// The id's move as this unit has it inline: the old string released and the id set to the pool's
// empty one, then the other's string taken and the other left empty (CFilePathStrId's move, on
// CStrId's pool).
static void MoveId(CStrId& self, CStrId& other) {
    base::global::UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        if (self.data && self.data != &pool->empty) {
            --self.data->ref_count;
            if (self.data->ref_count == 0) {
                fn_71000803a4(pool, self.data);
                pool = g_unk_7101d47108;
            }
        }
        self.data = &pool->empty;
    }
    self.data = other.data;
    other.data = &g_unk_7101d47108->empty;
}

struct UnkAssignedRecord_f78e68;

// The original returns `self`; src/game_d63c74/fn_7100f78e68.cpp declares it void, with `other` const
// (it is moved from).
UnkAssignedRecord_f78e68* fn_7100f477a0(UnkAssignedRecord_f78e68* dst, const UnkAssignedRecord_f78e68* src) {
    using namespace env_move_assign;
    CEnvironmentData* self = reinterpret_cast<CEnvironmentData*>(dst);
    CEnvironmentData* other = reinterpret_cast<CEnvironmentData*>(const_cast<UnkAssignedRecord_f78e68*>(src));
    MoveId(self->sID, other->sID);
    self->tFog = other->tFog;
    self->tAmbient = other->tAmbient;
    self->tDepthTint = other->tDepthTint;
    self->tMaterialTint = other->tMaterialTint;
    self->tPlayerLight = other->tPlayerLight;
    self->tHemisphericalLight = other->tHemisphericalLight;
    self->tBloom = other->tBloom;
    self->tVerticalFog = other->tVerticalFog;
    self->tCubeMap = static_cast<SCubeMap&&>(other->tCubeMap);
    self->tSSAO = other->tSSAO;
    self->tToneMapping = other->tToneMapping;
    self->tIBLAttenuation = other->tIBLAttenuation;
    AssignTransitions(&self->tFogTransitions, &other->tFogTransitions);
    AssignTransitions(&self->tAmbientTransitions, &other->tAmbientTransitions);
    AssignTransitions(&self->tDepthTintTransitions, &other->tDepthTintTransitions);
    AssignTransitions(&self->tMaterialTintTransitions, &other->tMaterialTintTransitions);
    AssignTransitions(&self->tPlayerLightTransitions, &other->tPlayerLightTransitions);
    AssignTransitions(&self->tHemisphericalLightTransitions, &other->tHemisphericalLightTransitions);
    AssignTransitions(&self->tBloomTransitions, &other->tBloomTransitions);
    AssignTransitions(&self->tVerticalFogTransitions, &other->tVerticalFogTransitions);
    AssignTransitions(&self->tCubeMapTransitions, &other->tCubeMapTransitions);
    AssignTransitions(&self->tSSAOTransitions, &other->tSSAOTransitions);
    AssignTransitions(&self->tToneMappingTransitions, &other->tToneMappingTransitions);
    AssignTransitions(&self->tIBLAttenuationTransitions, &other->tIBLAttenuationTransitions);
    return dst;
}
