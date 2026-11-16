// CAIManager::CTunableAIManager's unit: resets the record fn_7100dac280 builds. It takes weak references
// to the camera holder's autofocus-aim and Metroid camera controllers (ids at +0x868 and +0x17e0 of
// the id table), and drops its other references. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCameraComponent.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAIRecord_dac5cc {
    void* vtable;
    u8 unk_0x8[0x48 - 0x8];
    UnkWeakHandle* autofocus_ctrl;  // 0x48
    UnkWeakHandle* metroid_ctrl;    // 0x50
    u8 unk_0x58[0x118 - 0x58];
    UnkRefObject* field_0x118;
    u32 field_0x120;
    bool field_0x124;
    UnkRefObject* field_0x128;
    UnkRefObject* field_0x130;
    u32 field_0x138;
};
static_assert(__builtin_offsetof(UnkAIRecord_dac5cc, field_0x138) == 0x138, "layout");

static inline char* IdTable() {
    return reinterpret_cast<char*>(&base::reflection::refl_GetSingleton());
}

// The holder's controller for `key` when it is of the class `get_class` returns.
static inline UnkWeakTarget* CameraCtrl(void* holder, u32 key, const CClass* (*get_class)()) {
    UnkCameraCtrl* ctrl = fn_71010e389c(holder, IdTable() + key);
    auto* object = reinterpret_cast<base::reflection::UnkReflObject*>(ctrl);
    return object->vf_0x0() == get_class() ? reinterpret_cast<UnkWeakTarget*>(ctrl) : nullptr;
}

void fn_7100dac5cc(UnkAIRecord_dac5cc* self) {
    void* holder = fn_7100defcec(g_unk_7101d47310);
    AssignRef(&self->autofocus_ctrl, GetWeakHandle(CameraCtrl(holder, 0x868, fn_7100720464)));
    AssignRef(&self->metroid_ctrl, GetWeakHandle(CameraCtrl(holder, 0x17e0, fn_7100932fa4)));
    ResetRef(&self->field_0x118);
    self->field_0x120 = 0;
    self->field_0x124 = false;
    ResetRef(&self->field_0x128);
    ResetRef(&self->field_0x130);
    self->field_0x138 = 0;
}
