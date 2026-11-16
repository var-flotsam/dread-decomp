// Unit 2702 (tools/tu_map.py, CAIComponent): sends the owner's (+0x30) animation (+0x220) the id
// `id` under `*kind` with `*mode` and the values `a` and `b`; when the owner has a
// CCharClassSetAnimStateTrack record whose sources name a track the animation has, the id's name
// followed by those sources is sent instead. Says whether the send went through. Names and layouts
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// A string with its own 0x100-byte buffer (as src/game/CAttack/CAttack_Entries_ovb0086.cpp has it).
struct UnkInlineString_b0086 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_b0086() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    explicit UnkInlineString_b0086(const char* text) : UnkInlineString_b0086() { fn_710007e77c(Str(), text); }
    ~UnkInlineString_b0086() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// The value descriptor of a send.
struct UnkSendValue_l110964 {
    u32 field_0x0;
    u32 field_0x4;
    f32 value;

    explicit UnkSendValue_l110964(f32 v) : field_0x0(0), field_0x4(0), value(v) {}
};

// The range a send applies over.
struct UnkSendRange_l110964 {
    s64 field_0x0;
    f32 value;
    u32 field_0xc;

    explicit UnkSendRange_l110964(f32 v) : field_0x0(-1), value(v), field_0xc(1) {}
};

struct UnkTrackOwner_l110964 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkTrackAI_l110964 {
    u8 pad_0x0[0x30];
    UnkTrackOwner_l110964* owner;  // 0x30
};

bool fn_7101176e78(UnkStruct_7101176ea8* self, void* key, void** out);

// The id's text, counting the read.
static inline const CRntString* UseText(const CStrId& id) {
    ++id.data->reads;
    return &id.data->String();
}

// Sends `hash` under `kind` with the mode, the range from `a` and the value `b`.
static inline bool Send(UnkAnimationComponent* animation, u64 hash, u64 kind, u32 mode, f32 a, f32 b) {
    base::reflection::UnkReflTypeIdArg sendMode;
    sendMode.id = mode;
    UnkSendRange_l110964 range(a);
    UnkSendValue_l110964 desc(b);
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(animation, hash, kind, &sendMode, &range, &desc, &mask);
}

bool fn_7101110964(UnkTrackAI_l110964* self, const CStrId* id, const u64* kind, const u32* mode, f32 a,
                   f32 b) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    UnkTrackQuery query;
    query.cls = fn_7100c4c1b8();
    query.result = nullptr;
    query.value = -1.0f;
    fn_710117b08c(self->owner->animation, &query);
    if (query.result) {
        const CRntString* name = UseText(*id);
        UnkInlineString_b0086 full;
        fn_710007eaa0(full.Str(), name);
        const CRntString* sources = UseText(query.result->sources);
        UnkInlineString_b0086 suffix;
        fn_710007eaa0(suffix.Str(), sources);
        fn_710007f3b4(full.Str(), suffix.Str());
        u64 hash = full.Str()->key;
        if (fn_7101176e78(reinterpret_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(hash),
                          nullptr)) {
            return Send(animation, hash, *kind, *mode, a, b);
        }
    }
    return Send(animation, id->data->Key(), *kind, *mode, a, b);
}
