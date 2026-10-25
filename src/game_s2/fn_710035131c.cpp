// Per-frame hook: runs fn_7100350da0 when the state word at 0x69c is 6 and, when the owner has an
// attack at +0x220 and a component at +0x2a8 whose flag at 0x128 is clear, and its object at
// +0x270 reacts to the global named id (set to the default id on first use), asks the attack for
// a fixed hashed action and, on success, clears the byte at 0x6a1 and posts a message with the
// owner's direction to the global dispatcher.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkVec {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkComponent {
    u8 pad_0x0[0x128];
    u8 field_0x128;
};

struct UnkOwner {
    u8 pad_0x0[0x38];
    u8 field_0x38[1];
    u8 pad_0x39[0x1e7];
    void* field_0x220;
    u8 pad_0x228[0x48];
    void* field_0x270;
    u8 pad_0x278[0x30];
    UnkComponent* field_0x2a8;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
    u8 pad_0x38[0x664];
    u32 field_0x69c;
    u8 pad_0x6a0;
    u8 field_0x6a1;
};

// A blackboard property definition (game/UnkGameBlackboard.h): its qualified id is built on
// first use.
struct UnkNamedId : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};

extern UnkNamedId g_unk_717275ac18;
extern u8 g_unk_71726b16d0[] __attribute__((visibility("hidden")));

void fn_7100350da0(UnkSelf* self);
void* fn_710083cd30(UnkComponent* component);
bool fn_71011428bc(void* object, const CStrId* id);
bool fn_710117ae38(void* attack, u64 hash, bool flag);
void fn_7100e03a78(UnkGameManager* dispatcher, void* key);
void fn_7100e038b0(UnkGameManager* dispatcher, void* key, UnkVec* vec, u64 arg);

// The bit pattern of a float in memory (a word load, as the original reads it).
static inline u32 AsU32(const f32& value) {
    u32 bits;
    __builtin_memcpy(&bits, &value, 4);
    return bits;
}

void fn_710035131c(UnkSelf* self) {
    if (self->field_0x69c == 6)
        fn_7100350da0(self);
    UnkOwner* owner = self->field_0x30;
    if (!owner->field_0x220)
        return;
    if (!owner->field_0x2a8)
        return;
    if (!fn_710083cd30(owner->field_0x2a8))
        return;
    owner = self->field_0x30;
    if (owner->field_0x2a8->field_0x128)
        return;
    void* object = owner->field_0x270;
    if (!object)
        return;
    if (g_unk_717275ac18.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&g_unk_717275ac18.qid, &g_unk_717275ac18, g_unk_7101d470e0);
    if (!fn_71011428bc(object, &g_unk_717275ac18.qid.id))
        return;
    void* attack = self->field_0x30->field_0x220;
    if (!attack)
        return;
    if (!fn_710117ae38(attack, 0x2376ca3f9e17e420ull, false))
        return;
    self->field_0x6a1 = 0;
    fn_7100e03a78(g_unk_7101d47310, g_unk_71726b16d0);
    UnkGameManager* dispatcher = g_unk_7101d47310;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x30->field_0x38));
    u32 x = AsU32(m.m[2]);  // the third column (the facing), as bit patterns
    u32 y = AsU32(m.m[6]);
    u32 z = AsU32(m.m[10]);
    UnkVec vec = {x, y, z};
    fn_7100e038b0(dispatcher, g_unk_71726b16d0 + 8, &vec, 0);
}
