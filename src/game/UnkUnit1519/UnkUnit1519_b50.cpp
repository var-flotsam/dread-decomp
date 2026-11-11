// Unit 1519 (tools/tu_map.py; no reflection hint): fn_7100a0f210 sets two vector parameters of the
// owner's (+0x18) render object (+0x150, then +0x88): the colour (0, 1, 1, alpha) at +0x14c and
// (x, y, 0, value) at +0x15c, each mirrored into the buffer of the binding at +0x1f0 / +0x1f8 when
// there is one. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/math/CVector.h"
#include "game/UnkTU132.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector2D;
using base::math::CVector3D;
using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkVec2_b50a5 {
    f32 x;
    f32 y;
};

struct UnkParamBinding_b50a5 {
    u8 unk_0x0[0x30];
    CVector4D* data;
};

struct UnkParamBlock_b50a5 {
    u8 unk_0x0[0x14c];
    CVector4D color;  // 0x14c
    CVector4D extra;  // 0x15c
    u8 unk_0x16c[0x84];
    UnkParamBinding_b50a5* color_binding;  // 0x1f0
    UnkParamBinding_b50a5* extra_binding;  // 0x1f8
};

static inline UnkParamBlock_b50a5* GetBlock_b50a5(char* self) {
    char* owner = *reinterpret_cast<char**>(self + 0x18);
    char* render = *reinterpret_cast<char**>(owner + 0x150);
    return *reinterpret_cast<UnkParamBlock_b50a5**>(render + 0x88);
}

void fn_7100a0f210(char* self, const UnkVec2_b50a5* pos, f32 value, f32 alpha) {
    CColor4F c(0.0f, 1.0f, 1.0f, alpha <= 0.0f ? 0.0f : alpha);
    UnkParamBlock_b50a5* block = GetBlock_b50a5(self);
    UnkVector4D v = fn_7100030878(&c);
    fn_710007b518(&block->color, &v);
    if (UnkParamBinding_b50a5* b = block->color_binding)
        *b->data = v;
    block = GetBlock_b50a5(self);
    f32 x = pos->x, y = pos->y;
    v.x = x;
    v.y = y;
    v.z = 0.0f;
    v.w = value;
    fn_710007b518(&block->extra, &v);
    if (UnkParamBinding_b50a5* b = block->extra_binding)
        *b->data = v;
}

// fn_7100a0f1d0: attach the effect at +0x18 to the scene of the owner's (+0x8, +0x10)
// holder at +0x228 (fn_71001457fc, flag 1).

struct UnkSceneHolder_b50a5 {
    u8 unk_0x0[0x50];
    void* scene;
};

static inline void* GetScene_b50a5(char* self) {
    char* owner = *reinterpret_cast<char**>(*reinterpret_cast<char**>(self + 0x8) + 0x10);
    return (*reinterpret_cast<UnkSceneHolder_b50a5**>(owner + 0x228))->scene;
}

void fn_7100a0f1d0(char* self) {
    fn_71001457fc(*reinterpret_cast<void**>(self + 0x18), GetScene_b50a5(self), 1);
}

// fn_7100a0f2e0: the x/y of a node's position (the translation of its matrix, from the model's
// vf_0x70) plus an offset (from the object at +0x158, vf_0x68), both looked up by the argument.
struct UnkVec3Ret_b50a5 : CVector3D {
    UnkVec3Ret_b50a5(const UnkVec3Ret_b50a5& o) { x = o.x; y = o.y; z = o.z; }
};

struct UnkVec2Ret_b50a5 : CVector2D {
    explicit UnkVec2Ret_b50a5(const CVector3D& v) { fn_710007b430(this, &v); }
    UnkVec2Ret_b50a5(const UnkVec2Ret_b50a5& o) { x = o.x; y = o.y; }
};

struct UnkOffsetSource_b50a5 {
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
    virtual UnkVec3Ret_b50a5 vf_0x68();
};

struct UnkModel_b50a5 {
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
    virtual const f32* vf_0x70(const void* key);
    u8 unk_0x8[0x150];
    UnkOffsetSource_b50a5* offsets;  // 0x158
};

static inline UnkModel_b50a5* GetModel_b50a5(char* self) {
    return *reinterpret_cast<UnkModel_b50a5**>(*reinterpret_cast<char**>(self + 0x10) + 0x10);
}

UnkVec2Ret_b50a5 fn_7100a0f2e0(char* self, const void* key) {
    const f32* m = GetModel_b50a5(self)->vf_0x70(key);
    f32 tx = m[3], ty = m[7], tz = m[11];
    CVector3D p;
    UnkVec3Ret_b50a5 off = GetModel_b50a5(self)->offsets->vf_0x68();
    p.x = tx + off.x;
    p.y = ty + off.y;
    p.z = tz + off.z;
    return UnkVec2Ret_b50a5(p);
}
