// CCameraComponent's unit (tools/tu_map.py unit 2691): the property handler. The front camera
// gets the first look at the property; then the component sets itself up from three floats and
// two objects, switches a named state, or reads or writes two floats and a flag. Other keys go
// to the base handler (fn_71010c4250). Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CStrId.h"
#include "game/CCameraComponent.h"
#include "game/UnkComponent.h"
#include "types.h"

using namespace unk_prop;

struct UnkOwner_71010e2278;
struct UnkArg1_71010e2278;
struct UnkArg2_71010e2278;
void CCameraComponent_CreateSceneObjectAndControllers(UnkOwner_71010e2278* self, UnkArg1_71010e2278* a1, UnkArg2_71010e2278* a2,
                   f32 x, f32 y, f32 z);

struct UnkCamComp7_ovb5;
void fn_71010e51e4(UnkCamComp7_ovb5* self);

struct UnkCamera_l0e4ab8 {
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
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual s32 OnProperty(UnkProp* p);  // vf_0x98; -1 when not handled
};

struct UnkCamNode_l0e4ab8 {
    UnkCamera_l0e4ab8* value;
    UnkCamNode_l0e4ab8* next;
};

struct UnkCamState_l0e4ab8 {
    u8 pad_0x0[0x10];
    bool active;
};

struct UnkCamStates_l0e4ab8 {
    UnkCamState_l0e4ab8** values;
    CStrId* keys;
    u32 count;

    UnkCamState_l0e4ab8** Find(const CStrId& key) {
        u32 i = 0;
        for (; i < count; ++i) {
            if (keys[i].data == key.data)
                return &values[i];
        }
        return &values[count];
    }
};

struct UnkCameraComponent_l0e4ab8 {
    u8 pad_0x0[0x48];
    f32 field_0x48;
    u8 pad_0x4c[0x14];
    UnkCamNode_l0e4ab8* cameras;  // +0x60, the queue's first node
    u8 pad_0x68[0x18];
    UnkCamStates_l0e4ab8 states;  // +0x80
    u8 pad_0x98[0x1c0];
    f32 default_fov;  // +0x258
    u8 pad_0x25c[0x9c4];
    bool scaled;  // +0xc20
};
static_assert(__builtin_offsetof(UnkCameraComponent_l0e4ab8, default_fov) == 0x258, "layout");
static_assert(__builtin_offsetof(UnkCameraComponent_l0e4ab8, scaled) == 0xc20, "layout");

u32 CCameraComponent_HandleProperty(UnkCameraComponent_l0e4ab8* self, UnkProp* p) {
    if (UnkCamNode_l0e4ab8* front = self->cameras) {
        if (UnkCamera_l0e4ab8* camera = front->value) {
            s32 result = camera->OnProperty(p);
            if (result != -1)
                return result;
        }
    }
    if (Is(p, Key(0x1458)) && p->set) {
        f32 x, y, z;
        UnkArg1_71010e2278* a1;
        UnkArg2_71010e2278* a2;
        CRntFile_ReadFormat(p->stream, "fffoo", &x, &y, &z, &a1, &a2);
        CCameraComponent_CreateSceneObjectAndControllers(reinterpret_cast<UnkOwner_71010e2278*>(self), a1, a2, x, y, z);
        return true;
    }
    if (Is(p, Key(0xa80)) && p->set) {
        char buf[0x80];
        u8 flag;
        CRntFile_ReadFormat(p->stream, "sb", buf, &flag);
        CStrId id(buf, true);
        bool on = flag;
        UnkCamState_l0e4ab8** it = self->states.Find(id);
        if (it != &self->states.values[self->states.count] && (*it)->active)
            fn_71010e41e0(reinterpret_cast<UnkCamComp2_ovb5*>(self),
                          reinterpret_cast<UnkCamEntry_ovb5*>(*it), on);
        return true;
    }
    if (Is(p, Key(0x2228)) && p->set) {
        fn_71010e51e4(reinterpret_cast<UnkCamComp7_ovb5*>(self));
        return true;
    }
    if (!p->set && Is(p, Key(0xdc0)))
        return CRntFile_WriteFormat(p->stream, "f", (f64)self->field_0x48) != 0;
    if (p->set && Is(p, Key(0xdc0)))
        return CRntFile_ReadFormat(p->stream, "f", &self->field_0x48) != 0;
    if (!p->set && Is(p, Key(0xda0)))
        return CRntFile_WriteFormat(p->stream, "f", (f64)self->default_fov) != 0;
    if (p->set && Is(p, Key(0xda0)))
        return CRntFile_ReadFormat(p->stream, "f", &self->default_fov) != 0;
    if (p->set && Is(p, Key(0x13d0)))
        return CRntFile_ReadFormat(p->stream, "b", &self->scaled) != 0;
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
