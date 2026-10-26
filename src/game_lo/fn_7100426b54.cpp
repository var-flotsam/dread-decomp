// When the owner has the sub-object at +0x220, forwards a request to it (the one
// CBigkranXSpitAttack sends, UnkSpitSend.h): the caller's id, a fixed 64-bit one, a mode (2 when
// `flag` is set, else 0), the two global descriptors and the all-bits mask.
#include "../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkOwner_7100426b54 {
    u8 unk_0x0[0x220];
    void* field_0x220;
};

struct UnkStruct_7100426b54 {
    u8 unk_0x0[0x8];
    UnkOwner_7100426b54* owner;
};

void fn_7100426b54(UnkStruct_7100426b54* self, u64 id, bool flag) {
    void* target = self->owner->field_0x220;
    if (!target)
        return;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = flag ? 2 : 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, id, 0x71b15e29dc542a11, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
