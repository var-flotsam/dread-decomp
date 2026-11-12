// sound::SGUISoundEventsSelector: picks the sound event for a GUI event id, the candidate whose
// condition scores highest among those that define the id. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU32.h"
#include "types.h"

struct UnkStrIdPathResource;
struct UnkIdPair;


// The id/path resource record; released out of line (fn_7100026f70), so the ids sit in a union.
struct UnkSoundEventRecord {
    union {
        base::global::CStrId id;
    };
    union {
        base::global::CFilePathStrId path;
    };
    UnkCountedResource* resource;

    UnkSoundEventRecord(const base::global::CStrId& i, const base::global::CFilePathStrId& p)
        : id(i), path(p), resource(nullptr) {}
    UnkSoundEventRecord(const UnkSoundEventRecord& other) : id(other.id), path(other.path) {
        resource = nullptr;
        if (UnkCountedResource* r = other.resource) {
            ++r->ref_count;
            resource = r;
        }
    }
    ~UnkSoundEventRecord() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(this)); }
};

struct UnkSoundContext {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkSoundCondition {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual f32 vf_0x18(UnkSoundContext* context);
};

struct UnkSoundCandidate {
    UnkSoundCondition* condition;
    UnkSoundEventRecord* values;
    u64* keys;
    u32 count;
};

struct UnkCandidateList {
    UnkSoundCandidate** data;
    u32 count;
    u8 pad_0xc[0x1c];
};
static_assert(sizeof(UnkCandidateList) == 0x28, "UnkCandidateList placeholder layout");

struct UnkSelector {
    u8 pad_0x0[0x80];
    UnkCandidateList* values;
    u64* keys;
    u32 count;
};

struct UnkBest {
    f32 score;
    UnkSoundEventRecord record;
};

inline UnkCandidateList* FindList(UnkSelector* self, const u64& key) {
    for (u32 i = 0; i < self->count; ++i) {
        if (self->keys[i] == key)
            return &self->values[i];
    }
    return &self->values[self->count];
}

inline UnkSoundEventRecord* FindRecord(UnkSoundCandidate* candidate, const u64& key) {
    for (u32 i = 0; i < candidate->count; ++i) {
        if (candidate->keys[i] == key)
            return &candidate->values[i];
    }
    return &candidate->values[candidate->count];
}


UnkSoundEventRecord fn_7100fc9e58(UnkSelector* self, const u64& key, const UnkSoundContext& context) {
    UnkSoundContext copy;
    UnkBest best = {0.0f, UnkSoundEventRecord(g_unk_7101d470e0, g_unk_7101d470e8)};
    UnkCandidateList* list = FindList(self, key);
    if (list != &self->values[self->count]) {
        for (UnkSoundCandidate **it = list->data, **end = it + list->count; it != end; ++it) {
            UnkSoundCandidate* candidate = *it;
            f32 score = 1.0f;
            if (UnkSoundCondition* condition = candidate->condition) {
                copy = context;
                score = condition->vf_0x18(&copy);
            }
            if (!(score > best.score))
                continue;
            UnkSoundEventRecord* record = FindRecord(candidate, key);
            if (record == &candidate->values[candidate->count])
                continue;
            best.score = score;
            fn_7100027118(reinterpret_cast<UnkStrIdPathResource*>(&best.record),
                          reinterpret_cast<UnkStrIdPathResource*>(record));
        }
    }
    return best.record;
}
