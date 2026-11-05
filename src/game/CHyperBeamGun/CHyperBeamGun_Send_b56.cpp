// CHyperBeamGun's unit (tools/tu_map.py): fn_7100846b20 clears bit 0 of the flags at +0x18a0; when
// that was the only bit set, it calls fn_71008449ec(0, 0), looks up the owner's (+0x18) sub-object at
// +0x220 for fn_7100cf6ef8's class (fn_710117b08c) and sends it the hash of the found record's string
// id with mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h), unless that id is the pool's empty
// one. fn_7100846ca0 (same unit) clears bit 4 of those flags and goes on while any bit is left:
// without input 0x800 (mask 0xfd) from the sound holder's input it clears the flag at +0x18fa;
// otherwise, unless bit 8 of the flags is set, it does the same call, lookup and send (UnkSpitSend
// written out, its mode declared above the query). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkHyperBeamOwner_b56 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkHyperBeamGun_b56 {
    u8 pad_0x0[0x18];
    UnkHyperBeamOwner_b56* owner;
    u8 pad_0x20[0x18a0 - 0x20];
    u32 field_0x18a0;
};

struct UnkSoundHolder;
extern UnkSoundHolder* g_unk_7101d47340;
void fn_710006cd9c(UnkSoundHolder* holder);

struct UnkInput_b56hb {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool IsPressed(u32 button, u32 mask);
};

static inline UnkSoundHolder* SoundHolder_b56() {
    if (!g_unk_7101d47340) {
        auto* h = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(h);
        g_unk_7101d47340 = h;
    }
    return g_unk_7101d47340;
}

void fn_71008449ec(UnkHyperBeamGun_b56* self, bool a, bool b);

void fn_7100846b20(UnkHyperBeamGun_b56* self) {
    u32 flags = self->field_0x18a0;
    u32 rest = flags;
    if (flags & 1) {
        rest = flags & ~1u;
        self->field_0x18a0 = rest;
    }
    if (flags == 0 || rest != 0)
        return;
    fn_71008449ec(self, false, false);
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (!target)
        return;
    UnkTrackQuery query = {fn_7100cf6ef8(), nullptr, -1.0f};
    fn_710117b08c(target, &query);
    if (!query.result)
        return;
    base::global::UnkStringData* id = query.result->sources.data;
    if (id == &g_unk_7101d47108->empty)
        return;
    // UnkSpitSend written out, so that its locals sit below the query as in the original.
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id) + 0x30);
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_7100846ca0(UnkHyperBeamGun_b56* self) {
    u32 flags = self->field_0x18a0;
    if (flags & 0x10) {
        flags &= ~0x10u;
        self->field_0x18a0 = flags;
    }
    if (flags == 0)
        return;
    auto* input = *reinterpret_cast<UnkInput_b56hb**>(SoundHolder_b56());
    if (!input->IsPressed(0x800, 0xfd)) {
        *(reinterpret_cast<u8*>(self) + 0x18fa) = 0;
        return;
    }
    if (self->field_0x18a0 & 0x100)
        return;
    fn_71008449ec(self, false, false);
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (!target)
        return;
    base::reflection::UnkReflTypeIdArg mode;  // declared first: it sits above the query
    UnkTrackQuery query = {fn_7100cf6ef8(), nullptr, -1.0f};
    fn_710117b08c(target, &query);
    if (!query.result)
        return;
    base::global::UnkStringData* id = query.result->sources.data;
    if (id == &g_unk_7101d47108->empty)
        return;
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id) + 0x30);
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
