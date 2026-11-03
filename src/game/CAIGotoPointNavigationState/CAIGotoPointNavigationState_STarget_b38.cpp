// Callbacks that take a reflected target value: when the value at +0x120 holds a
// CAIGotoPointNavigationState::STarget, call the bound delegate with it.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

extern __attribute__((visibility("hidden"))) base::reflection::CClass g_unk_717270cde8;  // STarget's class

struct UnkTarget;
typedef void (*UnkTargetDelegate)(void* context, const UnkTarget* target, void* owner, u32 arg, void* data);

struct UnkTargetHolder {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0xc0];
    void* context;
    UnkTargetDelegate delegate;
    bool enabled;
    u8 pad_0x109[0x17];
    UnkReflValue target;
};

__attribute__((always_inline)) static inline bool IsTarget(UnkReflValue* v) {
    const CType* type = reinterpret_cast<const CType*>(v->type);
    if (!type)
        return false;
    fn_71007019f0();
    return fn_710009d50c(type, reinterpret_cast<const CType*>(&g_unk_717270cde8));
}

__attribute__((always_inline)) static inline const UnkTarget* GetTarget(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    return base::reflection::refl_ArgValue<UnkTarget>(v);
}

u32 fn_7100702564(UnkTargetHolder* self, u32 arg, void* data) {
    const UnkTarget* target = IsTarget(&self->target) ? GetTarget(&self->target) : nullptr;
    if (!target || !self->enabled || !self->delegate)
        return 0;
    self->delegate(self->context, target, self->owner, arg, data);
    return 1;
}
