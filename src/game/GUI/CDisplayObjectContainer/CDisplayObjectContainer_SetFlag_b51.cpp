// From unit 2748 of tools/tu_map.py (GUI::CDisplayObjectContainer): sets the flag at +0x18a on a
// container and, recursively, on every child that is itself a container. The class check calls the
// getter, then tests against its static descriptor (g_unk_7172a0f120). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "types.h"

struct UnkContainer_71011da0c0;

struct UnkChildNode_71011da0c0 {
    base::reflection::UnkReflObject* value;
    UnkChildNode_71011da0c0* next;
};

struct UnkContainer_71011da0c0 {
    u8 pad_0x0[0x18a];
    bool flag_0x18a;
    u8 pad_0x18b[0x1d0 - 0x18b];
    UnkChildNode_71011da0c0* head;  // +0x1d0
};

void fn_71011da0c0(UnkContainer_71011da0c0* self, bool flag) {
    self->flag_0x18a = flag;
    for (UnkChildNode_71011da0c0* n = self->head; n; n = n->next) {
        base::reflection::UnkReflObject* c = n->value;
        if (!c)
            continue;
        const base::reflection::CType* t = c->vf_0x0();
        fn_71011d7b84();
        if (fn_710009d50c(t, reinterpret_cast<const base::reflection::CType*>(g_unk_7172a0f120)))
            fn_71011da0c0(reinterpret_cast<UnkContainer_71011da0c0*>(c), flag);
    }
}
