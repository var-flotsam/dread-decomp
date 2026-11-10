// CAttack's unit (unit 444 of tools/tu_map.py, the name from reflection): the attack's entries and ids.
// fn_7100362648, fn_710036270c and fn_71003624f4 look up the entry for a value in the target object
// (+0x20) through fn_71003646b0, with the AI component's value (+0x1788) instead when its string id names
// the same string as the target's (as fn_7100362324, CAttack_SameIdNotify_A2_b68.cpp), and ask it (vf_0x40,
// vf_0x48) or the target (vf_0x28, vf_0x30, offset by the acted-against actor's radius) for distances.
// fn_71003627f8 builds the attack's animation id and fn_71003606c8 sets the preset and its hash.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkAttackTarget_A2_b68;

struct UnkIdData_b0086 {
    u8 pad_0x0[0x30];
    u64 hash;
    u8 pad_0x38[0x44 - 0x38];
    union {
        u32 use_count;
    };
};

struct UnkAttackTarget_b0086 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual f32 vf_0x28(UnkActor* owner);
    virtual f32 vf_0x30(UnkActor* owner);
    u8 pad_0x8[0x30];
    union {
        UnkIdData_b0086* id;
    };
};

struct UnkAttackAI_b0086 {
    u8 pad_0x0[0x1780];
    union {
        UnkIdData_b0086* id;
    };
    u32 value;
};

struct UnkAttack_b0086 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x10];
    UnkAttackTarget_b0086* target;
};

struct UnkAttackEntry_b0086 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual f32 vf_0x40(void* arg);
    virtual f32 vf_0x48(void* arg);
};

void fn_71003646b0(UnkAttackTarget_A2_b68* target, u32 value);

static inline UnkAttackEntry_b0086* GetEntry(UnkAttackTarget_b0086* target, u32 value) {
    return reinterpret_cast<UnkAttackEntry_b0086* (*)(UnkAttackTarget_A2_b68*, u32)>(&fn_71003646b0)(
        reinterpret_cast<UnkAttackTarget_A2_b68*>(target), value);
}

// The entry for `value`, or for the AI component's value when its string id names the same string
// as the target's.
static inline UnkAttackEntry_b0086* FindEntry(UnkAttack_b0086* self, u32 value) {
    if (UnkActor* owner = self->owner) {
        if (UnkAttackAI_b0086* ai = static_cast<UnkAttackAI_b0086*>(owner->field_0x240)) {
            if (reinterpret_cast<void*>(ai->id) != &g_unk_7101d47108->empty) {
                ++ai->id->use_count;
                UnkIdData_b0086* id = ai->id;
                UnkAttackTarget_b0086* target = self->target;
                ++target->id->use_count;
                bool same = id->hash == target->id->hash;
                if (same)
                    return GetEntry(self->target, ai->value);
            }
        }
    }
    return GetEntry(self->target, value);
}

f32 fn_7100362648(UnkAttack_b0086* self, void* arg, u32 value) {
    if (!self->target)
        return 0.0f;
    UnkAttackEntry_b0086* entry = FindEntry(self, value);
    if (!entry)
        return 0.0f;
    return entry->vf_0x40(arg);
}

f32 fn_710036270c(UnkAttack_b0086* self, void* arg, u32 value) {
    if (!self->target)
        return 0.0f;
    UnkAttackEntry_b0086* entry = FindEntry(self, value);
    if (!entry)
        return 0.0f;
    return entry->vf_0x48(arg);
}

void fn_71003624f4(UnkAttack_b0086* self, u32 value, f32* first, f32* second) {
    if (!self->target) {
        *first = -1.0f;
        *second = -1.0f;
        return;
    }
    UnkAttackTarget_b0086* target = self->target;
    if (UnkActor* owner = self->owner) {
        if (UnkAttackAI_b0086* ai = static_cast<UnkAttackAI_b0086*>(owner->field_0x240)) {
            if (reinterpret_cast<void*>(ai->id) != &g_unk_7101d47108->empty) {
                ++ai->id->use_count;
                UnkIdData_b0086* id = ai->id;
                UnkAttackTarget_b0086* t = self->target;
                ++t->id->use_count;
                bool same = id->hash == t->id->hash;
                target = self->target;
                if (same)
                    value = ai->value;
            }
        }
    }
    UnkAttackEntry_b0086* entry = GetEntry(target, value);
    *first = -1.0f;
    *second = -1.0f;
    if (!entry)
        return;
    UnkActor* other = fn_7100d7c990(self->owner);
    f32 radius = other ? other->field_0x37c : 0.0f;
    if (self->target->vf_0x28(self->owner) >= 0.0f)
        *first = radius + self->target->vf_0x28(self->owner);
    if (self->target->vf_0x30(self->owner) >= 0.0f)
        *second = radius + self->target->vf_0x30(self->owner);
}

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkLuaDoString_b50.cpp.
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

// The pooled data behind a string id as its text accessor sees it: the text at +0x10 and a count of
// the reads at +0x44 (a union member: the original reloads the id after the increment).
struct UnkIdText_b0086 {
    u8 pad_0x0[0x10];
    CRntString text;
    u8 pad_0x40[4];
    union {
        u32 reads;
    };
};

static inline const CRntString* UseText(UnkIdText_b0086* const* id) {
    ++(*id)->reads;
    return &(*id)->text;
}

// A string id as this function returns it (CStrId's layout and mangling-neutral return): built from a
// string (fn_7100000850) or copied from another id.
struct UnkAttackStrId_b0086 {
    UnkStringData* data;

    explicit UnkAttackStrId_b0086(const CRntString& str) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    explicit UnkAttackStrId_b0086(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkAttackStrId_b0086() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

struct UnkAttackOwner3_b0086 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkAttack3_b0086 {
    void** vtable;
    UnkAttackOwner3_b0086* owner;
    u8 pad_0x10[0x10];
    void* target;  // 0x20

    UnkIdText_b0086* const* vf_0xb0() {
        return reinterpret_cast<UnkIdText_b0086* const* (*)(UnkAttack3_b0086*)>(vtable[0xb0 / 8])(this);
    }
};

struct UnkGoliathXSlamAttack_b58a3;

// The attack's animation id: the name vf_0xb0 gives followed by the sources of the owner's
// CCharClassSetAnimStateTrack record, as a string id; the empty id without a target.
UnkAttackStrId_b0086 fn_71003627f8(UnkGoliathXSlamAttack_b58a3* attack, UnkActor* owner) {
    UnkAttack3_b0086* self = reinterpret_cast<UnkAttack3_b0086*>(attack);
    if (!self->target)
        return UnkAttackStrId_b0086(g_unk_7101d470e0);
    const CRntString* name = UseText(self->vf_0xb0());
    UnkInlineString_b0086 str;
    fn_710007eaa0(str.Str(), name);
    UnkTrackQuery query;
    query.cls = fn_7100c4c1b8();
    query.result = nullptr;
    query.value = -1.0f;
    fn_710117b08c(self->owner->animation, &query);
    if (query.result)
        fn_710007f3b4(str.Str(), UseText(reinterpret_cast<UnkIdText_b0086* const*>(&query.result->sources)));
    return UnkAttackStrId_b0086(*str.Str());
}

CRntString fn_710007ef90(const CRntString*, const char*);
u64* fn_7100066630(u64*, const CRntString*);

struct UnkIdHash_b0086 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAttackPreset_b0086 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkIdText_b0086* name;  // 0x38, a string id

    const CStrId* vf_0x58() {
        return reinterpret_cast<const CStrId* (*)(UnkAttackPreset_b0086*)>(vtable[0x58 / 8])(this);
    }
};

struct UnkAttack4_b0086 {
    u8 pad_0x0[0x18];
    u64 hash;                        // 0x18
    UnkAttackPreset_b0086* preset;  // 0x20
};

// The address of a temporary, which lives until the end of the full expression it is made in.
static inline const CRntString* Addr(const CRntString& tmp) { return &tmp; }

// `text` in lower case (ASCII letters only), at most 0xff characters.
static inline UnkInlineString_b0086 LowerCase(const char* text) {
    char lower[0x100];
    lower[0] = 0;
    u32 length = __builtin_strlen(text);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = text[i];
            lower[i] = (u8)(c - 'A') < 26 ? c + 0x20 : c;
        }
        lower[length] = 0;
    }
    return UnkInlineString_b0086(lower);
}

// Sets the preset (+0x20) and the hash at +0x18: the preset's own id (vf_0x58) when it has one,
// else the hash of its name (+0x38) in lower case followed by "_end".
void fn_71003606c8(UnkAttack4_b0086* self, UnkAttackPreset_b0086* preset) {
    self->preset = preset;
    if (!preset)
        return;
    if (preset->vf_0x58()->data != &g_unk_7101d47108->empty) {
        self->hash = reinterpret_cast<UnkIdHash_b0086*>(self->preset->vf_0x58()->data)->hash;
        return;
    }
    u64 hash;
    UnkIdText_b0086* const* id = self->preset ? &self->preset->name
                                              : reinterpret_cast<UnkIdText_b0086* const*>(&g_unk_7101d470e0);
    self->hash = (fn_7100066630(&hash, Addr(fn_710007ef90(LowerCase(UseText(id)->data).Str(), "_end"))), hash);
}
