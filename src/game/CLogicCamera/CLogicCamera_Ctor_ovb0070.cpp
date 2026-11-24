// CLogicCamera's unit (tools/tu_map.py unit 2655). Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/CLogicCamera.h"
#include "game/CLogicCameraComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CStrId;

struct UnkLogicCamera_ovb0070 {
    void** vtable;
    u8 pad_0x8[0x8];
    CStrId field_0x10;
    CStrId sControllerID;
    bool bStatic;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 fFovX;
    f32 fMinExtraZDist;
    f32 fMaxExtraZDist;
    f32 fDefaultInterp;
};

// The constructor: the parent constructor fn_7101043904, the class vtable, an empty id and the id of
// "" at +0x10 and +0x18, cleared fields, 45 at +0x3c and -1 at +0x48, then the post-construction
// virtual call (slot 3).
void fn_710105263c(UnkLogicCamera_ovb0070* self) {
    fn_7101043904(self);
    self->vtable = &g_unk_7101a81678[2];
    new (&self->field_0x10) CStrId(base::global::CStrId::Empty());
    new (&self->sControllerID) CStrId("", true);
    self->bStatic = false;
    self->field_0x24 = 0.0f;
    self->field_0x28 = 0.0f;
    self->field_0x2c = 0.0f;
    self->field_0x30 = 0.0f;
    self->field_0x34 = 0.0f;
    self->field_0x38 = 0.0f;
    self->fFovX = 45.0f;
    self->fMinExtraZDist = 0.0f;
    self->fMaxExtraZDist = 0.0f;
    self->fDefaultInterp = -1.0f;
    reinterpret_cast<void (*)(UnkLogicCamera_ovb0070*)>(self->vtable[3])(self);
}

// An id constructed empty: the empty string of the pool, created first if there is none.
static inline void InitEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

// The copy constructor: the parent constructor fn_7101043904, the class vtable, two empty ids (not
// copied) and the other's fields from +0x20 on.
void fn_71010515c8(UnkLogicCamera_ovb0070* self, const UnkLogicCamera_ovb0070* other) {
    fn_7101043904(self);
    self->vtable = &g_unk_7101a81678[2];
    InitEmpty(&self->field_0x10);
    InitEmpty(&self->sControllerID);
    self->field_0x24 = 0.0f;
    self->field_0x28 = 0.0f;
    self->field_0x2c = 0.0f;
    self->field_0x30 = 0.0f;
    self->field_0x34 = 0.0f;
    self->field_0x38 = 0.0f;
    self->bStatic = other->bStatic;
    self->field_0x24 = other->field_0x24;
    self->field_0x28 = other->field_0x28;
    self->field_0x2c = other->field_0x2c;
    self->field_0x30 = other->field_0x30;
    self->field_0x34 = other->field_0x34;
    self->field_0x38 = other->field_0x38;
    self->fFovX = other->fFovX;
    self->fMinExtraZDist = other->fMinExtraZDist;
    self->fMaxExtraZDist = other->fMaxExtraZDist;
    self->fDefaultInterp = other->fDefaultInterp;
}

struct UnkCameraVtable_ovb0070 {
    void** vtable;
    UnkCameraVtable_ovb0070() { vtable = &g_unk_7101a20998[2]; }
};

struct UnkCameraCallbackBase_ovb0070_0105263c : UnkCameraVtable_ovb0070 {
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;
    UnkCameraCallbackBase_ovb0070_0105263c() : field_0x30(0) {}
};

// The callback set at the camera's +0x38 handle.
struct UnkCameraCallbackSet_ovb0070 : UnkCameraCallbackBase_ovb0070_0105263c {
    base::UnkRefHandle owner;

    // The move constructor (fn_7101052fd4): an empty set takes the other's owner, state and
    // callbacks; the other is left empty.
    __attribute__((always_inline)) UnkCameraCallbackSet_ovb0070(UnkCameraCallbackSet_ovb0070&& other);
};

inline UnkCameraCallbackSet_ovb0070::UnkCameraCallbackSet_ovb0070(UnkCameraCallbackSet_ovb0070&& other) {
    vtable = &g_unk_7101cac850[2];
    owner.ptr = nullptr;
    owner.Set(other.owner.ptr);
    field_0x30 = other.field_0x30;
    UnkErasedVector* from = &other.callbacks;
    fn_710023b29c(&callbacks, from);
    other.field_0x30 = 0;
    fn_7100ad3588(from);
    if (base::UnkRefCounted* p = other.owner.ptr) {
        if (--p->ref_count == 0)
            delete p;
        other.owner.ptr = nullptr;
    }
}

void fn_7101052fd4(UnkCameraCallbackSet_ovb0070* self, UnkCameraCallbackSet_ovb0070* other) {
    new (self) UnkCameraCallbackSet_ovb0070(static_cast<UnkCameraCallbackSet_ovb0070&&>(*other));
}
