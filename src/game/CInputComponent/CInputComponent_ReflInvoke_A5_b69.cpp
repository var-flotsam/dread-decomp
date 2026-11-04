// CInputComponent's unit (tools/tu_map.py unit 1326): a reflected function's invoker (fn_710087412c) for the input
// request of src/game/CInputComponent/CInputComponent.cpp (fn_7100872460, inlined): arguments `enable`, `reset`
// and a pointer, with the object at +8; answers the empty status {0, 0, 2, 1}. Names are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkInputComponent {
    u8 pad_0x0[0xad0];
    u8 field_0xad0[1];
};

void fn_7100872268(UnkInputComponent* self);
void fn_710086f870(void* block, void* arg);
void fn_7100870150(void* block, void* arg);

static inline __attribute__((always_inline)) void* ArgPtr_A5_b69(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

static inline __attribute__((always_inline)) void* ArgPtrOrNull_A5_b69(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    return ArgPtr_A5_b69(arg);
}

static inline __attribute__((always_inline)) void Request_A5_b69(UnkInputComponent* self, bool enable, bool reset,
                                                                 void* arg) {
    if (reset)
        fn_7100872268(self);
    if (enable)
        fn_710086f870(self->field_0xad0, arg);
    else
        fn_7100870150(self->field_0xad0, arg);
}

UnkReflValue fn_710087412c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkInputComponent* self = *reinterpret_cast<UnkInputComponent**>(obj + 8);
    bool enable = *static_cast<const bool*>(ArgPtr_A5_b69(&a[0]));
    bool reset = *static_cast<const bool*>(ArgPtr_A5_b69(&a[1]));
    Request_A5_b69(self, enable, reset, ArgPtrOrNull_A5_b69(&a[2]));
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
