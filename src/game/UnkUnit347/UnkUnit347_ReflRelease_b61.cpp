// Unit 347 (tools/tu_map.py, no name hint): a reflected function (fn_71002643b8) taking an object and
// a bool. When the bool is set it calls the object's vf_0x48 and, if the game manager's check
// fn_7100deabe4 holds, hands the object to the manager's +0x608 (fn_71011a2770) and to the manager
// (fn_7100dea774). Answers the empty status {0, 0, 2, 1}. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkReleased_b61 {
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
};

void fn_7100dea774(UnkGameManager* self, void* object);

static inline __attribute__((always_inline)) void* ArgPtr_b61(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71002643b8(void*, char*, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkReleased_b61* object = *static_cast<UnkReleased_b61**>(ArgPtr_b61(&a[0]));
    bool flag = *static_cast<const bool*>(ArgPtr_b61(&a[1]));
    if (flag) {
        UnkGameManagerSub_0x608* points = g_unk_7101d47310->field_0x608;
        object->vf_0x48();
        UnkGameManager* manager = g_unk_7101d47310;
        if (fn_7100deabe4(manager)) {
            fn_71011a2770(points, object);
            fn_7100dea774(manager, object);
        }
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
