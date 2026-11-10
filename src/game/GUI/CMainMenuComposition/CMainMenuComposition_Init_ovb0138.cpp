// GUI::CMainMenuComposition's unit (tools/tu_map.py): its set-up. Resets the root's entry for the
// composition's name, runs the base set-up (fn_71011d9b3c) and looks up the "Content" container
// (+0x278) and the "ListComposition" list in it (+0x280). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CStrId;

struct UnkPanel_71011dfeb0;
void fn_71011d9b3c(UnkPanel_71011dfeb0* self);  // the base composition's set-up
struct UnkPtrList_A6;
void fn_7100b774b8(UnkPtrList_A6* self);
UnkPtrList_A6* fn_71011b90a8(UnkGuiRoot* root, const CStrId* name);

struct UnkMainMenuComposition_ovb0138 {
    u8 pad_0x0[0x18];
    CStrId name;  // 0x18
    u8 pad_0x20[0x278 - 0x20];
    UnkGuiDisplayObject* content;  // 0x278
    UnkGuiDisplayObject* list;     // 0x280
};

template <const base::reflection::CClass* (*Class)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(Class()))
               ? obj
               : nullptr;
}

void fn_7100b36d20(UnkMainMenuComposition_ovb0138* self) {
    if (self->name.data != &g_unk_7101d47108->empty) {
        if (UnkPtrList_A6* list = fn_71011b90a8(g_unk_717271e8f8, &self->name))
            fn_7100b774b8(list);
    }
    fn_71011d9b3c(reinterpret_cast<UnkPanel_71011dfeb0*>(self));
    if (self->content)
        return;
    self->content = As<fn_71011d7b84>(
        fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(self), CStrId("Content", true)));
    if (self->list)
        return;
    self->list = As<fn_71011fe904>(fn_71011d94d4(self->content, CStrId("ListComposition", true)));
}
