// Unit 1693 (tools/tu_map.py, no name hint): a GUI screen's set-up: clears its word at +0x10, adds its
// key to the GUI root's key list (+0xca8), builds its root display object when it has none (vf_0x10),
// hands the root to the owner (+0x268, with -1 at +0x27c), and gives the "CompanyName" child, when it is
// a GUI::CDisplayObject, four values of 1 (fn_71011ceb54, fn_71011cebb0, fn_71011cec0c, fn_71011cec68)
// and 0.8 (fn_71011ce208). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU1693.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CStrId;

void fn_71011ceb54(UnkGuiDisplayObject* self, f32 value);
void fn_71011cebb0(UnkGuiDisplayObject* self, f32 value);
void fn_71011cec0c(UnkGuiDisplayObject* self, f32 value);

struct UnkUnit1693Screen {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // builds the root
    UnkGuiDisplayObject* root;  // 0x8
    u32 field_0x10;
};

// The owner: the root at +0x268 and a word at +0x27c, written through plain pointers (the original
// reloads the screen's root after them).
struct UnkUnit1693Owner {
    u8 data[0x280];

    UnkGuiDisplayObject*& Root() { return *reinterpret_cast<UnkGuiDisplayObject**>(data + 0x268); }
    s32& Field27c() { return *reinterpret_cast<s32*>(data + 0x27c); }
};

static inline UnkGuiDisplayObject* AsDisplayObject(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(fn_71011c3868()))
               ? obj
               : nullptr;
}

void fn_7100b4d210(UnkUnit1693Screen* self, UnkUnit1693Owner* owner) {
    self->field_0x10 = 0;
    fn_710002fd10(&g_unk_717271e8f8->field_0xca8, &g_unk_71727645a0);
    if (!self->root)
        self->vf_0x10();
    owner->Root() = self->root;
    owner->Field27c() = -1;
    UnkGuiDisplayObject* name = AsDisplayObject(fn_71011d94d4(self->root, CStrId("CompanyName", true)));
    if (name) {
        fn_71011ceb54(name, 1.0f);
        fn_71011cebb0(name, 1.0f);
        fn_71011cec0c(name, 1.0f);
        fn_71011cec68(name, 1.0f);
        fn_71011ce208(name, 0.8f);
    }
}
