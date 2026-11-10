// Unit 2579 (tools/tu_map.py): adds a sensor to the owner's array of CHearingSensor pointers (fn_7100f0305c): the
// object is cast to CHearingSensor first, so one of another class adds a null. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSensorOwner_b0238 {
    void* vtable;
    base::UnkVector<UnkReflObject*> sensors;  // 0x8
};

static inline UnkReflObject* AsSensor(UnkReflObject* obj) {
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->vf_0x0(), fn_7100f03390()) ? obj : nullptr;
}

void fn_7100f0305c(UnkSensorOwner_b0238* self, UnkReflObject* obj) {
    UnkReflObject* sensor = AsSensor(obj);
    self->sensors.PushBack(sensor);
}
