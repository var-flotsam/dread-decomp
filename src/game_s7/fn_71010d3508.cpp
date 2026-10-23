// Reads or writes one of an object's named settings, some of them on the actor it is attached
// to (position, angles). Unknown keys go to the base handler (fn_71010c4250). Layouts are
// placeholders.
#include "UnkPropS7.h"
#include "game/UnkXformNode.h"

using namespace unk_prop;
using base::global::CRntString;

struct UnkActor_71010d3508 {
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
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8(const UnkVec3f* pos, bool flag);
    u8 unk_0x8[0x18];
    UnkXformNode node;
    u8 unk_0x130[0x24c];
    union {
        f32 field_0x37c;
    };
    u8 unk_0x380[0x8];
    UnkVec3f field_0x388;

    void SetField37c(f32 value) { field_0x37c = value; }
    void SetField388(f32 x, f32 y, f32 z) {
        field_0x388.x = x;
        field_0x388.y = y;
        field_0x388.z = z;
    }

    // The translation column of the node's matrix.
    const f32* Matrix() const { return reinterpret_cast<const f32*>(node.xform_0x18); }
};

struct UnkObj_71010d3508 {
    u8 unk_0x0[0x30];
    UnkActor_71010d3508* actor;
    u8 unk_0x38[0x8];
    f32 field_0x40;
    u8 unk_0x44[0x14];
    f32 field_0x58;
    u8 unk_0x5c[0x20];
    f32 field_0x7c;
    u8 unk_0x80[0x4];
    UnkVec3f origin_pos;
    UnkVec3f origin_ang;
    u8 unk_0x9c[0x5d];
    bool field_0xf9;
    u8 unk_0xfa[0x6];
    f32 field_0x100;
    bool field_0x104;
};

bool fn_71010d32ec(UnkObj_71010d3508* self, const CRntString& name);
void fn_710007c00c(void* stream, void* unk, s32 flags);
u32 fn_71010c4250(UnkObj_71010d3508* self, UnkProp* p);

u32 fn_71010d3508(UnkObj_71010d3508* self, UnkProp* p) {
    if (p->set && Is(p, Key(0x10b8)))
        return fn_710007cf30(p->stream, "f", &self->field_0x7c) != 0;
    if (!p->set && Is(p, Key(0x1528)))
        return fn_710007cb4c(p->stream, "b", self->field_0xf9) != 0;
    if (!p->set && Is(p, Key(0x12c0)))
        return fn_710007cb4c(p->stream, "b", self->field_0x104) != 0;
    if (p->set && Is(p, Key(0x12c0)))
        return fn_710007cf30(p->stream, "b", &self->field_0x104) != 0;
    const f32* m = self->actor->Matrix();
    f32 x = m[3], y = m[7], z = m[11];
    UnkVec3f pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    if (!p->set && Is(p, Key(0xcf0)))
        return fn_710007cb4c(p->stream, "v", &pos) != 0;
    if (IsName(p, CRntString("OriginPos")) && !p->set)
        return fn_710007cb4c(p->stream, "o", &self->origin_pos) == 8;
    if (IsName(p, CRntString("OriginPos")) && p->set) {
        const UnkVec3f* v;
        if (fn_710007cf30(p->stream, "o", &v) != 8)
            return false;
        self->origin_pos = *v;
        return true;
    }
    if (IsName(p, CRntString("OriginAng")) && !p->set)
        return fn_710007cb4c(p->stream, "o", &self->origin_ang) == 8;
    if (IsName(p, CRntString("OriginAng")) && p->set) {
        const UnkVec3f* v;
        if (fn_710007cf30(p->stream, "o", &v) != 8)
            return false;
        self->origin_ang = *v;
        return true;
    }
    if (!p->set && Is(p, Key(0x15a0)))
        return fn_710007cb4c(p->stream, "f", (f64)self->field_0x100) != 0;
    if (p->set && Is(p, Key(0x15a0)))
        return fn_710007cf30(p->stream, "f", &self->field_0x100) != 0;
    if (Is(p, Key(0x1480)) && p->set) {
        UnkVec3f v;
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 0.0f;
        fn_710007cf30(p->stream, "fff", &v.x, &v.y, &v.z);
        self->actor->vf_0xd8(&v, true);
        return true;
    }
    if (Is(p, Key(0x2038))) {
        f32 a, b, c, d;
        fn_710007cf30(p->stream, "ffff", &a, &b, &c, &d);
        self->actor->SetField37c(a);
        self->actor->SetField388(b, c, d);
        return true;
    }
    if (!p->set && Is(p, Key(0xde0)))
        return fn_710007cb4c(p->stream, "f", (f64)self->field_0x40) != 0;
    if (!p->set && Is(p, Key(0x24b0)))
        return fn_710007cb4c(p->stream, "f", (f64)self->field_0x58) != 0;
    if (!p->set && Is(p, Key(0x1c50)))
        return fn_710007cb4c(p->stream, "f", (f64)(self->actor ? self->actor->Matrix()[3] : 0.0f)) != 0;
    if (!p->set && Is(p, Key(0x7a8)))
        return fn_710007cb4c(p->stream, "f",
                             (f64)(self->actor ? self->actor->node.GetAngle().y : 0.0f)) != 0;
    if (p->set && Is(p, Key(0xde0)))
        return fn_710007cf30(p->stream, "f", &self->field_0x40) != 0;
    if (p->set && Is(p, Key(0x24b0)))
        return fn_710007cf30(p->stream, "f", &self->field_0x58) != 0;
    if (Is(p, Key(0xc30)) && !p->set) {
        char buf[0x80];
        fn_710007cf30(p->stream, "s", buf);
        bool result = fn_71010d32ec(self, CRntString(buf));
        fn_710007c00c(p->stream, nullptr, 0);
        fn_710007cb4c(p->stream, "b", result);
        return true;
    }
    return fn_71010c4250(self, p);
}
