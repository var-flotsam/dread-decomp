// Unit 2730 of tools/tu_map.py (the cutscene manager): walks the cutscene section of the game blackboard
// from its newest property back, and lists the cutscenes not played yet (a "CutScenePlayed[<name>]" flag
// of 0) by name, up to the first played one after them. Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU112.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflValue;

// A string id built from a string's text, out of line (fn_7100000850), moved out of.
struct UnkTextStrId_l198978 {
    UnkStringData* data;

    explicit UnkTextStrId_l198978(const base::global::CRntString& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextStrId_l198978() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = &pool->empty;
        }
    }
};

struct UnkCutsceneList_l198978 {
    u8 pad_0x0[0xae8];
    base::UnkStrIdVec unplayed;  // 0xae8
    u8 pad_0xb10[0xb18 - 0xb10];
    CStrId section;  // 0xb18
};
static_assert(__builtin_offsetof(UnkCutsceneList_l198978, section) == 0xb18, "layout");

// Moves `id` in at the end, growing by the vector's step.
static inline void MoveBack(base::UnkStrIdVec* vec, UnkTextStrId_l198978& id) {
    u32 n = vec->count;
    u32 c = vec->capacity;
    if (c <= n && vec->step != 0)
        c = (n / vec->step + 1) * vec->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    vec->Reserve(c);
    vec->data[vec->count].data = id.data;
    id.data = &g_unk_7101d47108->empty;
    ++vec->count;
}

// The property under `key`, or the one past the last.
static inline UnkReflValue* FindValue(UnkPropertyArrays* properties, const CStrId& key) {
    u64 i = 0;
    for (; i < properties->count; ++i) {
        if (properties->ids[i].data == key.data)
            break;
    }
    return &static_cast<UnkReflValue*>(properties->values)[i];
}

void fn_7101198978(UnkCutsceneList_l198978* self) {
    CBlackboard::CSection* section = fn_7101047484(&g_unk_7101d47310->field_0x968, self->section);
    if (!section)
        return;
    UnkPropertyArrays* properties = &section->data.properties;
    CStrId* end = properties->ids + properties->count;
    if (properties->ids == end)
        return;
    bool listed = false;
    // Newest first; the oldest property is never visited.
    for (CStrId* key = end - 1; key != properties->ids; --key) {
        UnkReflValue* value = FindValue(properties, *key);
        fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(value->type), fn_7100005178());
        if (*base::reflection::refl_ArgValue<u32>(value) != 0) {
            if (listed)
                return;
            continue;
        }
        ++key->data->reads;
        UnkTunableLine name(key->data->String());
        name = fn_710013c834(name.Str(), "CutScenePlayed[", g_unk_710162e2ed);
        name = fn_710013c834(name.Str(), "]", g_unk_710162e2ed);
        UnkTextStrId_l198978 id(*name.Str());
        MoveBack(&self->unplayed, id);
        listed = true;
    }
}
