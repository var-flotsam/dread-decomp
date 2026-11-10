// CMenuAnimationChangeComponent's unit (tools/tu_map.py unit 1423): fn_7100932c04, unless the owner's
// animation (+0x220) is playing "exit", requests one of four animations (g_unk_7101647ce8, picked with
// rand) with the value 2, as refl_EmitU32 (base/reflection/UnkReflStatics.h) written out. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

using base::global::CStrId;

extern "C" int rand();

struct UnkStruct_710117af10;
struct UnkCls_710117af10;
bool fn_710117af10(UnkStruct_710117af10* self, UnkCls_710117af10** name, bool skipBusy);

// Four animation hashes, one picked at random.
extern const u64 g_unk_7101647ce8[4] __attribute__((visibility("hidden")));

struct UnkMenuAnimOwner_ovb0136 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkMenuAnimationChange_ovb0136 {
    u8 pad_0x0[0x30];
    UnkMenuAnimOwner_ovb0136* owner;
};

void fn_7100932c04(UnkMenuAnimationChange_ovb0136* self) {
    void* anim = self->owner->field_0x220;
    if (!anim)
        return;
    bool exiting;
    {
        CStrId exit("exit", true);
        exiting = fn_710117af10(static_cast<UnkStruct_710117af10*>(anim), reinterpret_cast<UnkCls_710117af10**>(&exit),
                                false);
    }
    if (exiting)
        return;
    // Declared past a goto (no lifetime markers), so they keep their own slots.
    goto send;
    base::reflection::UnkReflTypeIdArg value;
    base::reflection::UnkReflTypeIdArg arg;
send:
    u64 hash = g_unk_7101647ce8[rand() % 4];
    value.id = 2;
    arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
    fn_7101176690(anim, hash, 0xf4061ff25a2163d6, &value, &g_unk_710164b958, &g_unk_7172769740, &arg);
}
