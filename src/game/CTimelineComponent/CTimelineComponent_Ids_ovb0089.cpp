// CTimelineComponent unit (tools/tu_map.py): calls on the timeline named by two ids: the hash of their
// strings joined when that names a known resource entry (fn_7100a2c13c), else the first id's own hash.
// The pooled id data keeps a read counter at +0x44. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CRntString;

struct UnkTimelineIdData_ovb0089 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 field_0x44;
    };
};

struct UnkTimelineId_ovb0089 {
    UnkTimelineIdData_ovb0089* data;

    bool IsEmpty() const {
        return reinterpret_cast<base::global::UnkStringData*>(data) == &g_unk_7101d47108->empty;
    }
    u64 Hash() const { return static_cast<u64>(data->str.key); }
    const CRntString& Str() const {
        ++data->field_0x44;
        return data->str;
    }
};

// Removes the timeline (fn_7100a2b7b8).
void fn_7100a2d400(UnkTimelineComponent* self, const UnkTimelineId_ovb0089* a, const UnkTimelineId_ovb0089* b) {
    if (!b->IsEmpty()) {
        u64 hash;
        {
            CRntString joined = fn_710007eee0(&a->Str(), &b->Str());
            fn_7100066630(&hash, &joined);
        }
        if (fn_7100a2c13c(self, &hash)) {
            fn_7100a2b7b8(self, &hash);
            return;
        }
    }
    u64 key = a->Hash();
    fn_7100a2b7b8(self, &key);
}

// Whether the timeline is a known resource entry (looked up again for the answer, as in the original).
bool fn_7100a2caac(UnkTimelineComponent* self, const UnkTimelineId_ovb0089* a, const UnkTimelineId_ovb0089* b) {
    if (!b->IsEmpty()) {
        u64 hash;
        {
            CRntString joined = fn_710007eee0(&a->Str(), &b->Str());
            fn_7100066630(&hash, &joined);
        }
        if (fn_7100a2c13c(self, &hash))
            return fn_7100a2c13c(self, &hash) != nullptr;
    }
    u64 key = a->Hash();
    return fn_7100a2c13c(self, &key) != nullptr;
}

// Sets the timeline with the default mask (fn_7100a2cb7c).
void fn_7100a2cddc(UnkTimelineComponent* self, const UnkTimelineId_ovb0089* a, const UnkTimelineId_ovb0089* b,
                   f32 delay, bool unique) {
    if (!b->IsEmpty()) {
        u64 hash;
        {
            CRntString joined = fn_710007eee0(&a->Str(), &b->Str());
            fn_7100066630(&hash, &joined);
        }
        if (fn_7100a2c13c(self, &hash)) {
            u32 mask = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
            fn_7100a2cb7c(self, &hash, &mask, unique, delay);
            return;
        }
    }
    u64 key = a->Hash();
    u32 mask = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
    fn_7100a2cb7c(self, &key, &mask, unique, delay);
}
