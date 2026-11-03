// CYamplotXBiteAttack unit (tools/tu_map.py): whether the bite can start. After the base attack's check,
// when the owner's AI component is a CYamplotXAIComponent, fn_71006f29c4 must not hold for it on a
// second look either. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkYamplotXAI_ybt {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkYamplotXAI_ybt*)>(vtable[0])(this);
    }
};

struct UnkBiteAttack_ybt {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

// The base attack's check (declared elsewhere without its arguments; called with them).
struct UnkSelf;
u32 fn_7100360b84(UnkSelf*);
// Declared elsewhere with a float argument; this caller leaves s0 unset, so the value passed is left
// undefined here.
struct UnkYamplotXAI;
bool fn_71006f29c4(UnkYamplotXAI* self, f32 value);

bool fn_71006eca84(UnkBiteAttack_ybt* self, void* arg1, void* arg2) {
    bool ok = reinterpret_cast<bool (*)(UnkBiteAttack_ybt*, void*, void*)>(fn_7100360b84)(self, arg1, arg2);
    UnkYamplotXAI_ybt* ai = static_cast<UnkYamplotXAI_ybt*>(self->owner->field_0x240);
    if (ai) {
        bool is_yamplot = fn_710009d50c(ai->vf_0x0(), fn_71006ef064());
        UnkYamplotXAI* yamplot = reinterpret_cast<UnkYamplotXAI*>(is_yamplot ? ai : nullptr);
        f32 unset;
        if (ok && is_yamplot)
            ok = !(fn_71006f29c4(yamplot, unset) && fn_71006f29c4(yamplot, unset));
    }
    return ok;
}
