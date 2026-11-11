// CEntity unit (tools/tu_map.py, the name from reflection): the reflected getter of the entity's
// bounding box ("oBBox": fn_7100d7bcc8, src/game/CEntity_2.cpp), returned as a reflected value of the
// box type g_unk_7108b6d3d0. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkEntityBox {
    base::math::CVector3D min;
    base::math::CVector3D max;
};
struct UnkEntityBounds;
UnkEntityBox fn_7100d7bcc8(const UnkEntityBounds* self);

extern base::reflection::CType g_unk_7108b6d3d0;

UnkReflValue fn_7100d823f4(void*, char* obj) {
    UnkEntityBox box = fn_7100d7bcc8(*reinterpret_cast<const UnkEntityBounds**>(obj + 8));
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, &g_unk_7108b6d3d0, &box, false);
    return r;
}

// A reflected getter (a bool): false when the entity's component at +0x228 says so (fn_7101173964);
// else what fn_7100729978 says of the object fn_7100d7c214 finds for it; else whether the render
// manager's test (fn_7100219588) passes for the entity's position (+0x44, +0x54, +0x64).

void* fn_7100d7c214(char* self);
bool fn_7100729978(void* self);
bool fn_7100219588(UnkRenderManager* self, const void* pos, u32 a, u32 b);

struct alignas(16) UnkEntityPos_b41a3 {
    u32 x, y, z;
};

UnkReflValue fn_7100d82448(void*, char* obj) {
    char* self = *reinterpret_cast<char**>(obj + 8);
    void* component = *reinterpret_cast<void**>(self + 0x228);
    UnkEntityPos_b41a3 pos;
    bool result;
    if (component && fn_7101173964(component)) {
        result = false;
    } else if (void* found = fn_7100d7c214(self)) {
        result = fn_7100729978(found);
    } else {
        UnkRenderManager* manager = g_unk_7101d47300;
        u32 x = *reinterpret_cast<u32*>(self + 0x44);
        u32 y = *reinterpret_cast<u32*>(self + 0x54);
        u32 z = *reinterpret_cast<u32*>(self + 0x64);
        pos.x = x;
        pos.y = y;
        pos.z = z;
        result = fn_7100219588(manager, &pos, 0, 0);
    }
    bool value = result;
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
