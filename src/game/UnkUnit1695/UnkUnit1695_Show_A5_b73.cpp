// Unit 1695 (tools/tu_map.py, no name hint): a GUI screen's show: builds its root display object when it has
// none (vf_0x10), hands the root to the owner (+0x268, 7 at +0x27c) and posts the state g_unk_71729f01c8 for
// it to the GUI sound sink (fn_7100fc9d78). Names and layouts are placeholders.
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

struct UnkStateSink;
struct UnkStateKey;
struct UnkEnumType;
extern UnkStateSink* g_unk_717275c380;
extern UnkEnumType g_unk_71729f01c8;

struct UnkUnit1695Root_A5_b73;

struct UnkUnit1695Screen_A5_b73 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // builds the root
    UnkUnit1695Root_A5_b73* root;
};

struct UnkUnit1695Owner_A5_b73 {
    u8 pad_0x0[0x268];
    UnkUnit1695Root_A5_b73* root;
    u8 pad_0x270[0x27c - 0x270];
    s32 field_0x27c;
};

void fn_7100b4f1a4(UnkUnit1695Screen_A5_b73* self, UnkUnit1695Owner_A5_b73* owner) {
    if (!self->root)
        self->vf_0x10();
    UnkUnit1695Root_A5_b73* root = self->root;
    owner->root = root;
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f01c8), root);
    owner->field_0x27c = 7;
}
