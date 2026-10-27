// The play report (unit 2569 of tools/tu_map.py, 0x7100ec32c0-0x7100ec5a2c): records of a key and
// a string or integer value appended to an array (fn_7100ec4240 builds a string record,
// fn_7100ec51e8 grows the array, fn_7100ec50e4 reads a string from a blackboard section). The
// function here fills the report's header: a profile id kept in the profile blackboard (made from
// a new UUID the first time), two times, a count, a mode and the session id. Names and layouts
// are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// nn::util's UUID and formatter (lib/nnsdk's nn/util.h has neither; port/runtime/include/nn/util.h
// declares the same).
namespace nn::util {
struct Uuid {
    u8 data[16];
};
Uuid GenerateUuid();
s32 TSNPrintf(char* buffer, u64 size, const char* format, ...);
}  // namespace nn::util

// A report record (0x20 bytes, as src/game_s7/fn_7101147868.cpp lays it out): the key, a string
// value, an integer value and the value's kind. Built out of line from a key and a string id
// (fn_7100ec4240) or an integer (fn_71003afa88); both take the ids by value.
struct UnkRecord_7101147868 {
    CStrId field_0x0;
    base::UnkStrIdMember field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkRecord_7101147868(CStrId name, CStrId value) asm("fn_7100ec4240");
    UnkRecord_7101147868(CStrId name, s64 value) asm("fn_71003afa88");
};
static_assert(sizeof(UnkRecord_7101147868) == 0x20, "UnkRecord_7101147868 placeholder layout");

// Grows the array to hold `capacity` records (out of line).
void fn_7100ec51e8(base::UnkVector<UnkRecord_7101147868>* self, u32 capacity);

// The report: its records at +0x18.
struct UnkPlayReport {
    u8 pad_0x0[0x18];
    base::UnkVector<UnkRecord_7101147868> records;
};

// Appends `record`, moving its ids (the moved-from ones get the empty id).
static inline void Append(base::UnkVector<UnkRecord_7101147868>* v, UnkRecord_7101147868&& record) {
    u32 n = v->count;
    u32 capacity = v->capacity;
    if (capacity <= n && v->step != 0)
        capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_7100ec51e8(v, capacity);
    UnkRecord_7101147868* dst = &v->data[v->count];
    dst->field_0x0.data = record.field_0x0.data;
    record.field_0x0.data = &g_unk_7101d47108->empty;
    dst->field_0x8.data = record.field_0x8.data;
    record.field_0x8.data = &g_unk_7101d47108->empty;
    // The value and its kind, stored through plain pointers (the original reloads the count after
    // them, not the pool).
    u64* value = &dst->field_0x10;
    u32* kind = &dst->field_0x18;
    *kind = record.field_0x18;
    *value = record.field_0x10;
    ++v->count;
}

// The id table (fn_7100031894 builds it; the same definition as src/game/CSubAreaManager.cpp's).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}
static inline const CStrId& IdTableKey(u32 offset) {
    return *reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x0 + offset);
}

// An item's qualified id (game/UnkGameBlackboard.h), built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The string the profile id is read into (base::global::CRntString's layout, built with a reserved
// size and destroyed out of line).
struct UnkString_7100fc4b34;
void fn_710007eba0(UnkString_7100fc4b34* self, u32 size);
bool fn_7100ec50e4(CBlackboard::CSection* section, const void* name, UnkString_7100fc4b34* out);
struct UnkReportString {
    union {
        CRntString str;
    };

    explicit UnkReportString(u32 size) { fn_710007eba0(Raw(), size); }
    ~UnkReportString() { fn_710007ecd4(&str); }
    UnkString_7100fc4b34* Raw() { return reinterpret_cast<UnkString_7100fc4b34*>(&str); }
    bool IsEmpty() const { return !str.data || !*str.data; }
};

bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// The profile values: a count and the profile id (item property definitions,
// src/game_s7/fn_7100ae39e0.cpp).
extern UnkPropertyEx<> g_unk_7172759d90;
extern UnkPropertyEx<> g_unk_71727594b8;

// The report's keys (this unit's string ids).
extern CStrId g_unk_71729ce0a0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71729ce0a8 __attribute__((visibility("hidden")));
extern CStrId g_unk_71729ce0b0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71729ce0b8 __attribute__((visibility("hidden")));
extern CStrId g_unk_71729ce0c0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71729ce0c8 __attribute__((visibility("hidden")));

f32 fn_7100dfd15c(UnkGameManager* self);
f32 fn_7100dffc44(UnkGameManager* self);
void* fn_7100df55e4(UnkGameManager* self);
s64 fn_7101144a5c(void* object);
nn::util::Uuid fn_7100132a00();  // the session id
// Passed by address: two words, both zero here.
struct UnkProfileSaveArg {
    void* field_0x0;
    void* field_0x8;
};
void fn_7100df99f8(UnkGameManager* self, u32 a, const UnkProfileSaveArg* arg, u32 b);

static inline void FormatUuid(char* buffer, const nn::util::Uuid& u) {
    nn::util::TSNPrintf(buffer, 0x40, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                        u.data[0], u.data[1], u.data[2], u.data[3], u.data[4], u.data[5], u.data[6], u.data[7],
                        u.data[8], u.data[9], u.data[10], u.data[11], u.data[12], u.data[13], u.data[14],
                        u.data[15]);
}

// Appends the report's header records; false (nothing appended) when there is no profile id.
// `fixed_mode` replaces the mode the game manager gives with 0x1b.
// NON_MATCHING(minor): after each append the original reloads the moved-from record's two ids from
// the stack before destroying it (it keeps them without type-based alias information); here the
// stored values are reused, so the string id's release test (its value is the pool's empty id) is
// folded away and the key's test compares the stored value: 504 bytes shorter, 7 fewer (never
// taken) release calls. The stack slots of the profile id, the record and the typed value differ,
// and the blackboard and array pointers swap registers (x20/x21).
bool fn_7100ec32c0(UnkPlayReport* self, bool fixed_mode) {
    UnkGameManager* gm = g_unk_7101d47310;
    CBlackboard* profile = reinterpret_cast<CBlackboard*>(reinterpret_cast<char*>(gm) + 0x820);
    f32 time_a = fn_7100dfd15c(gm);
    f32 time_b = fn_7100dffc44(g_unk_7101d47310);
    s32 count = 0;
    {
        const CStrId& section = IdTableKey(0x2050);
        const UnkQualifiedId& name = Qid(g_unk_7172759d90);
        if (CBlackboard::CSection* s = fn_7101047270(profile, section))
            fn_7100395a30(s, &name, &count);
    }
    UnkReportString id(0x40);
    {
        const CStrId& section = IdTableKey(0x1148);
        const UnkQualifiedId& name = Qid(g_unk_71727594b8);
        if (CBlackboard::CSection* s = fn_7101047270(profile, section))
            fn_7100ec50e4(s, &name, id.Raw());
    }
    char text[0x40];
    if (id.IsEmpty()) {
        FormatUuid(text, nn::util::GenerateUuid());
        {
            const CStrId& section = IdTableKey(0x1148);
            const UnkQualifiedId& name = Qid(g_unk_71727594b8);
            CRntString value(text);
            if (CBlackboard::CSection* s = fn_71010472e0(profile, section)) {
                if (!profile->field_0xe8) {
                    UnkTypedValue v;
                    v.type = fn_7100007f84();
                    v.value = &value;
                    fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
                }
            }
        }
        UnkProfileSaveArg arg = {nullptr, nullptr};
        fn_7100df99f8(g_unk_7101d47310, 0, &arg, 0);
    }
    {
        const CStrId& section = IdTableKey(0x1148);
        const UnkQualifiedId& name = Qid(g_unk_71727594b8);
        if (CBlackboard::CSection* s = fn_7101047270(profile, section))
            fn_7100ec50e4(s, &name, id.Raw());
    }
    FormatUuid(text, fn_7100132a00());
    if (id.IsEmpty())
        return false;

    base::UnkVector<UnkRecord_7101147868>* records = &self->records;
    Append(records, UnkRecord_7101147868(g_unk_71729ce0a0, CStrId(id.str.data, true)));
    Append(records, UnkRecord_7101147868(g_unk_71729ce0a8, (s64)time_a));
    Append(records, UnkRecord_7101147868(g_unk_71729ce0b0, (s64)time_b));
    if (!fixed_mode)
        Append(records, UnkRecord_7101147868(g_unk_71729ce0b8, fn_7101144a5c(fn_7100df55e4(g_unk_7101d47310))));
    else
        Append(records, UnkRecord_7101147868(g_unk_71729ce0b8, (s64)0x1b));
    Append(records, UnkRecord_7101147868(g_unk_71729ce0c0, (s64)count));
    Append(records, UnkRecord_7101147868(g_unk_71729ce0c8, CStrId(text, true)));
    return true;
}
