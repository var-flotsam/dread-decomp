// CGooplotUndoJumpAttack unit (tools/tu_map.py): whether the undo jump can start. The jump attack's check
// (fn_71005944e8) must pass; when the owner's AI component (+0x240) is a CGooplotAIComponent, it also needs
// the word at +0x14808 set and jumps left (+0x147f8). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkGooplotJump_ovb0123;
bool CGooplotJumpAttack_CanStart(UnkGooplotJump_ovb0123* self, void* arg, u32 mode);

struct UnkGooplotAI_l596b30 {
    void** vtable;
    u8 pad_0x8[0x147f8 - 0x8];
    s32 jumps;  // 0x147f8
    u8 pad_0x147fc[0x14808 - 0x147fc];
    s32 field_0x14808;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkGooplotAI_l596b30*)>(vtable[0])(this);
    }
};

struct UnkGooplotOwner_l596b30 {
    u8 pad_0x0[0x240];
    UnkGooplotAI_l596b30* ai;
};

struct UnkGooplotUndoJump_l596b30 {
    u8 pad_0x0[0x8];
    UnkGooplotOwner_l596b30* owner;
};

bool fn_7100596b30(UnkGooplotUndoJump_l596b30* self, void* arg, u32 mode) {
    bool ok = CGooplotJumpAttack_CanStart(reinterpret_cast<UnkGooplotJump_ovb0123*>(self), arg, mode);
    UnkGooplotAI_l596b30* ai;
    if (ok && (ai = self->owner->ai) != nullptr && fn_710009d50c(ai->vf_0x0(), fn_71005996a0()))
        ok = (ai->field_0x14808 != 0) & (ai->jumps > 0);
    return ok;
}
