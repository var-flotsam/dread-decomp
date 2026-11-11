// Unit 108 (tools/tu_map.py, no name hint): a reflected function's invoker (fn_710011df5c) taking an
// index and a 3D point. The point's x and y (z = 0) are stored at that index (fn_7100122b2c), the
// object is flagged at +0x2d, and the stored entry (fn_7100118950) is answered by reference, described
// by the type at g_unk_7108b6da30. Names are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU108.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::CType;
using base::reflection::UnkReflValue;

extern base::reflection::CType g_unk_7108b6da30;

struct UnkPointSet_b61 {
    u8 pad_0x0[0x2d];
    bool field_0x2d;
};

// Typed as src/game_s1/fn_710011e1e8.cpp declares it (one signature per symbol).
struct UnkInsertTarget;
struct UnkArgObj;
void* fn_7100118950(UnkPointSet_b61* self, u32 index);

struct UnkProjected_b61 : CVector2D {
    explicit UnkProjected_b61(const CVector3D* v) { fn_710007b430(this, v); }
};

static inline __attribute__((always_inline)) void* ArgPtr_b61(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

static inline __attribute__((always_inline)) void* OptArgPtr_b61(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    return ArgPtr_b61(arg);
}

UnkReflValue fn_710011df5c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* self = *reinterpret_cast<UnkPointSet_b61**>(obj + 8);
    u32 index = *static_cast<const u32*>(ArgPtr_b61(&a[0]));
    const auto* v = static_cast<const CVector3D*>(OptArgPtr_b61(&a[1]));
    UnkProjected_b61 p(v);
    CVector3D point = {p.x, p.y, 0.0f};
    fn_7100122b2c(reinterpret_cast<UnkInsertTarget*>(self), reinterpret_cast<const UnkArgObj*>(&point), index);
    self->field_0x2d = true;
    void* entry = fn_7100118950(self, index);
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, &g_unk_7108b6da30, entry, true);
    return r;
}
