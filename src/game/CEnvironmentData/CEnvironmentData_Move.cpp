// CEnvironmentData's move constructor (fn_7100f44fd8): the copy and move hooks of the
// CRntDictionary<CStrId, CEnvironmentData> entry (src/clones/clones_05.cpp, refl_IterValueMove)
// construct an entry's value with it. The name and the field list come from reflection
// (mercury-engine-data-structures' CEnvironmentData; the offsets from the fields' getter functions,
// src/reflection/registration_02.cpp). Every member is default-constructed first; then the id is
// moved, the settings blocks are copied, and the twelve transition vectors are copy-assigned.
// Layouts inside the blocks follow the copy and the defaults; names past the reflected ones are
// placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"  // g_unk_7101cab1f8, STransition's vtable
#include "base/UnkStrIdValueVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "game/UnkTU2599.h"
#include "types.h"

using base::UnkStrIdValueElem;
using base::UnkVector;
using base::global::CFilePathStrId;
using base::global::CStrId;

namespace env_move {

// A colour: assigned out of line (fn_710007b518, CVector4D's assignment), zero by default.
struct UnkColor : base::math::CVector4D {
    UnkColor() : base::math::CVector4D{} {}
    explicit UnkColor(f32 v) : base::math::CVector4D{v, v, v, v} {}
    UnkColor& operator=(const UnkColor& other) {
        fn_710007b518(this, &other);
        return *this;
    }
};

struct SFog {  // 0x40
    f32 field_0x0;
    UnkColor tColor;  // 0x4
    f32 field_0x14, field_0x18, field_0x1c;
    f32 field_0x20 = 0.0f, field_0x24 = 0.0f;
    UnkColor field_0x28;
    f32 field_0x38;
};
static_assert(sizeof(SFog) == 0x3c, "SFog layout");

struct SAmbient {  // 0x18
    f32 fColorInterp;
    UnkColor tColor;  // 0x4
};

struct SDepthTint {  // 0x28: nine floats, no defaults
    f32 field_0x0[9];
};

struct SMaterialTint {  // 0x20
    f32 fColorInterp;
    UnkColor tColor;  // 0x4
    f32 fBlend, fBlendInterp;
};

struct SPlayerLight {  // 0x40
    f32 fColorInterp;
    UnkColor tDiffuse, tSpecular0, tSpecular1;  // 0x4, 0x14, 0x24
    f32 field_0x34;
    f32 field_0x38 = 0.0f, field_0x3c = 0.0f;
};

struct SHemisphericalLight {  // 0x20
    f32 fColorUpInterp;
    base::math::CVector3D tColorUp = {};  // 0x4
    f32 fColorDownInterp;
    base::math::CVector3D tColorDown = {};  // 0x14
};

struct SBloom {  // 0x10
    f32 fBloomInterp;
    base::math::CVector3D tBloom = {};  // 0x4
};

struct SVerticalFog {  // 0x50
    bool bEnabled;
    UnkColor tColor;  // 0x4
    f32 field_0x14, field_0x18, field_0x1c;
    f32 field_0x20 = 0.0f, field_0x24 = 0.0f;
    f32 field_0x28;
    UnkColor tAttenuation;  // 0x2c
    f32 field_0x3c;
    f32 field_0x40 = 0.0f, field_0x44 = 1.0f, field_0x48 = 0.0f, field_0x4c = 1.0f;
};

struct SCubeMap {  // 0x10
    f32 fInterp;
    bool bEnabled;
    CFilePathStrId sTexturePath;  // 0x8
};

struct SSSAO {  // 0xc (the vector after it is 8-aligned)
    f32 fFallOff = 1.0f, fIntensity = 0.5f, fBias = 0.5f;
};

struct SToneMapping {  // 0x30
    f32 fInterp = 1.0f;
    f32 field_0x4 = 0.0f, field_0x8 = 0.0f, field_0xc = 0.0f, field_0x10 = 0.0f, field_0x14 = 0.0f;
    UnkColor field_0x18 = UnkColor(1.0f);
    f32 field_0x28 = 0.0f;
    bool field_0x2c = false;
};

struct SIBLAttenuation {  // 0x10
    f32 fInterp = 1.0f, fCubeAttFactor = 1.0f, fZDistance = 0.0f, fGradientSize = 0.0f;
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

}  // namespace env_move

extern char g_unk_7101cab220[];  // SFogTransition's
extern char g_unk_7101cab338[];  // SVerticalFogTransition's

// The vectors' out-of-line resizes and grow-at accessors, one per element type.
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

namespace env_move {

template <int Id, class T>
struct UnkTransitionTag {};

// An element type whose grow-at is inline (the fog and vertical fog transitions).
#define DREAD_INLINE_TRANSITION(ID, T, VTABLE, RESIZE)                                                  \
    template <>                                                                                         \
    struct base::UnkStrIdValueTraits<UnkTransitionTag<ID, T>> {                                        \
        using Vec = UnkVector<UnkStrIdValueElem<UnkTransitionTag<ID, T>, T>>;                           \
        static char* base_vtable() { return g_unk_7101cab1f8; }                                         \
        static char* vtable() { return VTABLE; }                                                        \
        static void resize(Vec* v, u32 n) { RESIZE(v, n); }                                             \
        __attribute__((always_inline)) static UnkStrIdValueElem<UnkTransitionTag<ID, T>, T>& grow_at(Vec* v, u32 i) {                  \
            return base::refl_StrIdValueGrowAtInline(v, i);                                                  \
        }                                                                                               \
    };

// One whose grow-at is called.
#define DREAD_CALLED_TRANSITION(ID, T, RESIZE, GROW_AT)                                                 \
    template <>                                                                                         \
    struct base::UnkStrIdValueTraits<UnkTransitionTag<ID, T>> {                                        \
        using Vec = UnkVector<UnkStrIdValueElem<UnkTransitionTag<ID, T>, T>>;                           \
        static void resize(Vec* v, u32 n) { RESIZE(v, n); }                                             \
        static UnkStrIdValueElem<UnkTransitionTag<ID, T>, T>& grow_at(Vec* v, u32 i) {                  \
            return *reinterpret_cast<UnkStrIdValueElem<UnkTransitionTag<ID, T>, T>*>(GROW_AT(v, i));    \
        }                                                                                               \
    };

}  // namespace env_move

using namespace env_move;

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

namespace env_move {

// A transition vector: the engine's array with allocation flags 0x53; copy-assigned element by
// element (it has no move of its own).
template <int Id, class T>
struct UnkTransitions : UnkVector<UnkStrIdValueElem<UnkTransitionTag<Id, T>, T>> {
    UnkTransitions() : UnkVector<UnkStrIdValueElem<UnkTransitionTag<Id, T>, T>>(0x53) {}
    __attribute__((always_inline)) void Assign(UnkTransitions& other) { base::refl_StrIdValueAssign(this, &other); }
};

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
    CStrId sID{base::global::UnkStrIdInline{}, nullptr, false};  // 0x3a0

    __attribute__((always_inline)) CEnvironmentData(CEnvironmentData&& other) {
        sID = static_cast<CStrId&&>(other.sID);
        tFog = other.tFog;
        tAmbient = other.tAmbient;
        tDepthTint = other.tDepthTint;
        tMaterialTint = other.tMaterialTint;
        tPlayerLight = other.tPlayerLight;
        tHemisphericalLight = other.tHemisphericalLight;
        tBloom = other.tBloom;
        tVerticalFog = other.tVerticalFog;
        tCubeMap.fInterp = other.tCubeMap.fInterp;
        tCubeMap.bEnabled = other.tCubeMap.bEnabled;
        tCubeMap.sTexturePath = static_cast<CFilePathStrId&&>(other.tCubeMap.sTexturePath);
        tSSAO = other.tSSAO;
        tToneMapping = other.tToneMapping;
        tIBLAttenuation = other.tIBLAttenuation;
        tFogTransitions.Assign(other.tFogTransitions);
        tAmbientTransitions.Assign(other.tAmbientTransitions);
        tDepthTintTransitions.Assign(other.tDepthTintTransitions);
        tMaterialTintTransitions.Assign(other.tMaterialTintTransitions);
        tPlayerLightTransitions.Assign(other.tPlayerLightTransitions);
        tHemisphericalLightTransitions.Assign(other.tHemisphericalLightTransitions);
        tBloomTransitions.Assign(other.tBloomTransitions);
        tVerticalFogTransitions.Assign(other.tVerticalFogTransitions);
        tCubeMapTransitions.Assign(other.tCubeMapTransitions);
        tSSAOTransitions.Assign(other.tSSAOTransitions);
        tToneMappingTransitions.Assign(other.tToneMappingTransitions);
        tIBLAttenuationTransitions.Assign(other.tIBLAttenuationTransitions);
    }
};
static_assert(__builtin_offsetof(CEnvironmentData, tFogTransitions) == 0x40, "CEnvironmentData layout");
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

}  // namespace env_move

// NON_MATCHING(minor): the same stores, copies and calls in the same order (5772 bytes against 5756); the
// original keeps the members' addresses it reuses (the colours, the vectors) in stack slots from the
// constructor on, so its register allocation and spills differ throughout.
void fn_7100f44fd8(env_move::CEnvironmentData* self, env_move::CEnvironmentData* other) {
    new (self) CEnvironmentData(static_cast<CEnvironmentData&&>(*other));
}
