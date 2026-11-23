// Weak references to GUI objects: setters that take the focused object of the GUI root (its
// object at 0x230, whose target is at 0x10), and assignments from another object's handle.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFocusUser238.h"
#include "game/UnkGuiFocusUser318.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkReportRef.h"
#include "game/UnkWeakRefF0.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::CClass;

void fn_7100b021fc(UnkGuiFocusUser318* self) {
    if (self->field_0x181)
        return;
    UnkWeakHandle* focus = g_unk_717271e8f8->field_0x230;
    UnkWeakTarget* target = focus ? focus->target : nullptr;
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    AssignRef(&self->field_0x318, handle);
    if (handle && handle->target)
        fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(handle->target), false);
    fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(self), true);
    fn_71011b6bbc(g_unk_717271e8f8, reinterpret_cast<UnkGuiDisplayObject*>(self), false);
}

void fn_7100b619b4(UnkGuiFocusUser238* self) {
    if (UnkWeakHandle* focus = g_unk_717271e8f8->field_0x230) {
        if (UnkWeakTarget* target = focus->target) {
            AssignRef(&self->field_0x238, GetWeakHandle(target));
            UnkWeakHandle* handle = self->field_0x238;
            fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(handle ? handle->target : nullptr), false);
        }
    }
    fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(self), true);
    fn_71011b6bbc(g_unk_717271e8f8, reinterpret_cast<UnkGuiDisplayObject*>(self), false);
}

void fn_7100b42be0(UnkWeakRefF0* self, UnkWeakTarget* other) { AssignRef(&self->field_0xf0, GetWeakHandle(other)); }

// A weak reference to a GUI::CBossRushReport, assigned from another weak reference: it refers to
// nothing when the other's object is not one.

__attribute__((always_inline)) static inline const CClass* GetClass(UnkWeakTarget* target) {
    return reinterpret_cast<const CClass* (*)(UnkWeakTarget*)>((*reinterpret_cast<void***>(target))[0])(target);
}

UnkReportRef* fn_7100b4c2e8(UnkReportRef* self, const UnkReportRef* other) {
    UnkWeakTarget* target = other->handle ? other->handle->target : nullptr;
    UnkWeakHandle* handle = nullptr;
    if (target && fn_710009d50c(GetClass(target), fn_7100af4354()))
        handle = GetWeakHandle(target);
    AssignRef(&self->handle, handle);
    return self;
}
