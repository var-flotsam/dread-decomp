// Copies of fn_7100b4c2e8 (src/game/GUI/CDialogComposition/fn_7100b021fc.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7101185c74.
// Weak references to GUI objects: setters that take the focused object of the GUI root (its
// object at 0x230, whose target is at 0x10), and assignments from another object's handle.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiFocusUser238.h"
#include "game/UnkGuiFocusUser318.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkReportRef.h"
#include "game/UnkWeakRefF0.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::CClass;

// A weak reference to a GUI::CBossRushReport, assigned from another weak reference: it refers to
// nothing when the other's object is not one.

__attribute__((always_inline)) static inline const CClass* GetClass(UnkWeakTarget* target) {
    return reinterpret_cast<const CClass* (*)(UnkWeakTarget*)>((*reinterpret_cast<void***>(target))[0])(target);
}

UnkReportRef* fn_7101185c74(UnkReportRef* self, const UnkReportRef* other) {
    UnkWeakTarget* target = other->handle ? other->handle->target : nullptr;
    UnkWeakHandle* handle = nullptr;
    if (target && fn_710009d50c(GetClass(target), fn_71011c3868()))
        handle = GetWeakHandle(target);
    AssignRef(&self->handle, handle);
    return self;
}
