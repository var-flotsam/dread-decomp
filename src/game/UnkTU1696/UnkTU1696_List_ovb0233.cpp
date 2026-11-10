// Unit 1696: the list screen's start step (fn_7100b4f930) stores the root in the owner, looks the list composition up by path and sets its flags. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CRntString;

using UnkGuiObj = UnkGuiDisplayObject;
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);

struct UnkListComposition_b0233 {
    u8 pad_0x0[0x262];
    bool field_0x262;
    bool field_0x263;
};

struct UnkListScreen_b0233 {
    u8 pad_0x0[0x8];
    UnkGuiObj* root;  // 0x8
};

struct UnkListOwner_b0233 {
    u8 pad_0x0[0x270];
    UnkGuiObj* field_0x270;
    u8 pad_0x278[8];
    s32 mode;  // 0x280
};

struct UnkMenuTarget;
void fn_7100b364dc(UnkMenuTarget* target);

void fn_7100b4f930(UnkListScreen_b0233* self, UnkListOwner_b0233* owner) {
    UnkGuiObj* source = self->root;
    // An untyped store: the original reads the source again afterwards.
    __builtin_memcpy(&owner->field_0x270, &source, sizeof(source));
    UnkGuiObj* root = self->root;
    UnkGuiObj* list;
    {
        CRntString path("Content.ListComposition");
        list = fn_71011d9508(root, reinterpret_cast<const char* const*>(&path));
        if (list && !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(list->GetClass()),
                                   reinterpret_cast<const base::reflection::CType*>(fn_71011fe904())))
            list = nullptr;
    }
    if (list) {
        auto* composition = reinterpret_cast<UnkListComposition_b0233*>(list);
        composition->field_0x262 = owner->mode != 4;
        composition->field_0x263 = true;
    }
    fn_7100b364dc(reinterpret_cast<UnkMenuTarget*>(owner));
}
