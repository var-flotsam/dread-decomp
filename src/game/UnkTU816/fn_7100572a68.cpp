#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/math/UnkOneVector3.h"
#include "game/UnkTU284.h"
#include "game/UnkXformNode.h"
#include "types.h"
#include "game/CFXComponent.h"

struct UnkWorld;
struct UnkPart;
struct UnkObject;

struct UnkEmmyPart_b0258p {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
};

struct UnkEmmyComp_b0258p {
    virtual const base::reflection::CClass* vf_0x0() const;
    void CallD8() { reinterpret_cast<void (*)(UnkEmmyComp_b0258p*)>((*reinterpret_cast<void***>(this))[0xd8 / 8])(this); }
};

struct UnkEmmyHolderSub_b0258p {
    u8 pad_0x0[0x280];
    UnkWorld* world;  // 0x280
};

struct UnkEmmyHolder_b0258p {
    u8 pad_0x0[0x10];
    UnkEmmyComp_b0258p* comp;  // 0x10
    u8 pad_0x18[0x18];
    UnkEmmyHolderSub_b0258p* sub;  // 0x30
};

struct UnkEmmyBehavior_b0258p {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    u8 pad_0x8[0x18];
    UnkEmmyHolder_b0258p* owner;  // 0x20
    bool active;                  // 0x28
    u8 pad_0x29[0x2f];
    f32 field_0x58;
    u8 pad_0x5c[0x8];
    bool wide;  // 0x64
    u8 pad_0x65[3];
    f32 field_0x68;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100572a68(UnkEmmyBehavior_b0258p* self) {
    if (!self->active)
        return;
    UnkWorld* world = self->owner->sub->world;
    UnkPart* part = fn_7101152b90(world, 0xddd2c5ec4e0b8138ull);
    UnkEmmyPart_b0258p* found = nullptr;
    if (part) {
        if (fn_710114e680(reinterpret_cast<UnkObject*>(world), reinterpret_cast<UnkEmmyPart_b0258p*>(part)->kind))
            found = reinterpret_cast<UnkEmmyPart_b0258p*>(part);
    }
    UnkEmmyComp_b0258p* comp = self->owner->comp;
    if (comp) {
        if (fn_710009d50c(comp->vf_0x0(), fn_710051e010()))
            comp->CallD8();
    }
    f32 scale = self->wide ? self->field_0x68 : self->field_0x58;
    f32 tx = scale * g_unk_71016477a8.x;
    f32 ty = scale * g_unk_71016477a8.y;
    f32 tz = scale * g_unk_71016477a8.z;
    UnkVec3f v;
    v.x = tx * 0.02f;
    v.y = ty * 0.02f;
    v.z = tz * 0.02f;
    fn_7100207be8(reinterpret_cast<UnkXformNode*>(reinterpret_cast<char*>(found) + 0x28), &v);
    self->vf_0x38();
}
