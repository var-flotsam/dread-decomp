// CShotManager::CTunableShotManager's unit (tools/tu_map.py, unit 2576): the shot manager's copy
// assignment, member by member. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/CShotManager/UnkShotEffect_b0011.h"
#include "game/UnkObj290_b43a3.h"
#include "game/UnkShotWeakRefData_b0011.h"
#include "game/UnkShotWeakRef_b0011.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;

static_assert(sizeof(UnkObj290_b43a3) == 0x290, "UnkObj290_b43a3 layout");

struct UnkShotWeakTarget_b0011;

struct UnkListedObj;

base::global::CRntVector<UnkObj290_b43a3>* CShotManager_AssignObj290Vector(base::global::CRntVector<UnkObj290_b43a3>* self,
                                               base::global::CRntVector<UnkObj290_b43a3>* other);
void CShotManager_AppendEffectCopy(base::global::CRntVector<UnkShotEffect_b0011>* self, const UnkShotEffect_b0011* effect);
base::global::CRntVector<UnkShotWeakRef_b0011>* CShotManager_CopyAssignWeakRefVector(base::global::CRntVector<UnkShotWeakRef_b0011>* self,
                                                     const base::global::CRntVector<UnkShotWeakRef_b0011>* other);

// The draw state between the vectors and the preset names (models, counts, render instances).
struct UnkShotDrawState_ledd3a4 {
    u8 data[0x288];
};

// The preset names (string ids) and the resolved presets, by kind and preset; each has its own copy
// assignment.
struct UnkShotPresetNames_ledd3a4 {
    u8 data[0x3c0];
};
struct UnkShotPresets_ledd3a4 {
    u8 data[0xb40];
};
UnkShotPresetNames_ledd3a4* fn_7100edcd50(UnkShotPresetNames_ledd3a4* self, const UnkShotPresetNames_ledd3a4* other);
UnkShotPresets_ledd3a4* fn_7100edd9d0(UnkShotPresets_ledd3a4* self, const UnkShotPresets_ledd3a4* other);

struct UnkShotManager_ledd3a4 {
    void** vtable;
    CRntVector<UnkObj290_b43a3> field_0x8;
    CRntVector<UnkShotEffect_b0011> field_0x30;
    CRntVector<UnkObj290_b43a3*> field_0x58;
    UnkShotDrawState_ledd3a4 field_0x80;
    UnkShotPresetNames_ledd3a4 field_0x308;
    UnkShotPresets_ledd3a4 field_0x6c8;
    f32 field_0x1208;
    u8 pad_0x120c[0x4];
    CRntVector<UnkShotWeakRef_b0011> field_0x1210;
    CRntVector<UnkListedObj*> field_0x1238;  // the shot lights
    u32 field_0x1260;
    u8 pad_0x1264[0x4];
    UnkWeakHandle* field_0x1268;  // a counted reference to an object's weak handle
    bool field_0x1270;
};
static_assert(__builtin_offsetof(UnkShotManager_ledd3a4, field_0x6c8) == 0x6c8, "placeholder layout");
static_assert(__builtin_offsetof(UnkShotManager_ledd3a4, field_0x1210) == 0x1210, "placeholder layout");
static_assert(__builtin_offsetof(UnkShotManager_ledd3a4, field_0x1270) == 0x1270, "placeholder layout");

// Copy assignment of the effects: cleared, reserved to the other's capacity, and each appended.
static inline void AssignEffects(CRntVector<UnkShotEffect_b0011>* self, const CRntVector<UnkShotEffect_b0011>* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        for (const UnkShotEffect_b0011 *p = other->data, *e = p + other->count; p != e; ++p)
            CShotManager_AppendEffectCopy(self, p);
    }
}

// A counted reference to an object's weak handle: the copy refers to the handle of the same object.
static inline void AssignWeakRef(UnkWeakHandle** slot, UnkWeakHandle* const* other) {
    if (slot != other)
        CopyWeakRef(slot, *other);
}

UnkShotManager_ledd3a4* fn_7100edd3a4(UnkShotManager_ledd3a4* self, UnkShotManager_ledd3a4* other) {
    CShotManager_AssignObj290Vector(&self->field_0x8, &other->field_0x8);
    AssignEffects(&self->field_0x30, &other->field_0x30);
    self->field_0x58 = other->field_0x58;
    self->field_0x80 = other->field_0x80;
    fn_7100edcd50(&self->field_0x308, &other->field_0x308);
    fn_7100edd9d0(&self->field_0x6c8, &other->field_0x6c8);
    self->field_0x1208 = other->field_0x1208;
    CShotManager_CopyAssignWeakRefVector(&self->field_0x1210, &other->field_0x1210);
    self->field_0x1238 = other->field_0x1238;
    self->field_0x1260 = other->field_0x1260;
    AssignWeakRef(&self->field_0x1268, &other->field_0x1268);
    self->field_0x1270 = other->field_0x1270;
    return self;
}
