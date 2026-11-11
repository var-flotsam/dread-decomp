// CLogicCameraComponent's unit (tools/tu_map.py unit 1399): the component's destructor, with its
// CLogicCamera member at +0x30 (vtable g_unk_7101a81678, src/game/CLogicCamera/CLogicCamera_Ctor_ovb0070.cpp)
// destroyed inline. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/CGameObject.h"
#include "game/CLogicCameraComponent.h"
#include "types.h"

struct UnkCameraCtrl_ovb0049;
struct UnkBaseComponent;
void fn_710024f6d4(UnkBaseComponent*);
extern void* g_unk_7101a81540[];

struct UnkLogicCamera_ovb0106 {
    void** vtable;
    u8 pad_0x8[0x8];
    base::UnkStrIdMember field_0x10;
    base::UnkStrIdMember field_0x18;
    u8 pad_0x20[0x2c];
};

struct UnkLogicCameraComponent_ovb0106 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLogicCamera_ovb0106 camera;  // 0x30
};

static inline void Destroy(UnkLogicCameraComponent_ovb0106* self) {
    self->vtable = &g_unk_7101a81540[2];
    UnkLogicCamera_ovb0106* camera = &self->camera;
    camera->vtable = &g_unk_7101a81678[2];
    camera->field_0x18.~UnkStrIdMemberOf();
    camera->field_0x10.~UnkStrIdMemberOf();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(camera));
    fn_710024f6d4(reinterpret_cast<UnkBaseComponent*>(self));
}

// The destructor and the deleting destructor.
void fn_7100910984(UnkLogicCameraComponent_ovb0106* self) { Destroy(self); }

void fn_7100910a6c(UnkLogicCameraComponent_ovb0106* self) {
    Destroy(self);
    operator delete(self);
}

// The camera's fields as its assignment sees them.
// A refcounted object deleted through its vtable; the count is a union member (the original reloads
// the holder after raising it).
struct UnkLogicCameraRef_ovb0106 {
    virtual void vf_0x0();
    virtual ~UnkLogicCameraRef_ovb0106();
    union {
        s32 ref_count;
    };
};

struct UnkLogicCameraTail_ovb0106 {
    f32 v[4];
};

struct UnkLogicCameraData_ovb0106 {
    void** vtable;
    UnkLogicCameraRef_ovb0106* field_0x8;
    base::UnkStrIdMember field_0x10;
    base::UnkStrIdMember field_0x18;
    bool field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    UnkLogicCameraTail_ovb0106 field_0x3c;
};
static_assert(sizeof(UnkLogicCameraData_ovb0106) == 0x50, "layout");

// The camera's copy assignment (its vtable is kept).
UnkLogicCameraData_ovb0106* fn_7100911098(UnkLogicCameraData_ovb0106* self, const UnkLogicCameraData_ovb0106* other) {
    UnkLogicCameraRef_ovb0106* ref = other->field_0x8;
    if (self->field_0x8 != ref) {
        if (ref)
            ++ref->ref_count;
        if (UnkLogicCameraRef_ovb0106* old = self->field_0x8) {
            if (--old->ref_count == 0)
                delete old;
        }
        self->field_0x8 = ref;
    }
    self->field_0x10 = other->field_0x10;
    self->field_0x18 = other->field_0x18;
    self->field_0x20 = other->field_0x20;
    self->field_0x24 = other->field_0x24;
    self->field_0x28 = other->field_0x28;
    self->field_0x2c = other->field_0x2c;
    self->field_0x30 = other->field_0x30;
    self->field_0x34 = other->field_0x34;
    self->field_0x38 = other->field_0x38;
    self->field_0x3c = other->field_0x3c;
    return self;
}
