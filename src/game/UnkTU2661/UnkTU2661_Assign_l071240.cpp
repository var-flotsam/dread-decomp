// Unit 2661 (tools/tu_map.py, hint CCutscene::CTunableCutscene): the copy assignment of the object that
// fn_71010673fc constructs: every member in turn, the counted media base keeping its own vtable and count.
// Names and layouts are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkBufferStringPool.h"
#include "game/sound/UnkSoundHandle.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;

struct UnkTunableHolder_l06f5cc;
struct UnkRenderEntry_l071694;
struct UnkBigState_b0181;
using Holder = UnkTunableHolder_l06f5cc;
using UnkRenderEntryVector_l071694 = base::global::CRntVector<UnkRenderEntry_l071694>;
base::global::CRntVector<Holder>* fn_710106f5cc(base::global::CRntVector<Holder>* self,
                                                const base::global::CRntVector<Holder>* other);
UnkRenderEntryVector_l071694* fn_7101071694(UnkRenderEntryVector_l071694* self, const UnkRenderEntryVector_l071694* other);
UnkBigState_b0181* fn_71010718fc(UnkBigState_b0181* self, const UnkBigState_b0181* other);

// The counted base: assigning leaves its vtable and count alone.
struct UnkCountedMedia_l071240 {
    void** vtable;
    u32 ref_count;  // 0x8

    UnkCountedMedia_l071240& operator=(const UnkCountedMedia_l071240&) { return *this; }
};

struct alignas(8) UnkCutsceneRecord_l071240 {
    u8 data[0x110];

    UnkCutsceneRecord_l071240& operator=(const UnkCutsceneRecord_l071240& other) asm("fn_7100c8c7b4");
};

// A counted reference to a resource.
struct UnkResourceRef_l071240 {
    UnkCountedResource* resource;

    UnkResourceRef_l071240& operator=(const UnkResourceRef_l071240& other) {
        UnkCountedResource* r = other.resource;
        if (resource != r) {
            if (r)
                ++r->ref_count;
            if (UnkCountedResource* old = resource) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            resource = r;
        }
        return *this;
    }
};

struct alignas(8) UnkRenderEntries_l071240 {
    u8 data[0x28];

    UnkRenderEntries_l071240& operator=(const UnkRenderEntries_l071240& other) {
        fn_7101071694(reinterpret_cast<UnkRenderEntryVector_l071694*>(this),
                      reinterpret_cast<const UnkRenderEntryVector_l071694*>(&other));
        return *this;
    }
};

struct alignas(8) UnkTunables_l071240 {
    u8 data[0x28];

    UnkTunables_l071240& operator=(const UnkTunables_l071240& other) {
        fn_710106f5cc(reinterpret_cast<base::global::CRntVector<Holder>*>(this),
                      reinterpret_cast<const base::global::CRntVector<Holder>*>(&other));
        return *this;
    }
};

struct alignas(8) UnkCutsceneState_l071240 {
    u8 data[0x178];

    UnkCutsceneState_l071240& operator=(const UnkCutsceneState_l071240& other) {
        fn_71010718fc(reinterpret_cast<UnkBigState_b0181*>(this), reinterpret_cast<const UnkBigState_b0181*>(&other));
        return *this;
    }
};

struct UnkCutsceneName_l071240 : UnkBufferString<0x80, g_unk_7101a1b750> {
    UnkCutsceneName_l071240& operator=(const UnkCutsceneName_l071240& other) {
        CRntString_Assign(Str(), other.Str());
        return *this;
    }
};

// A vector copied a component at a time.
struct UnkVec3_l071240 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3_l071240& operator=(const UnkVec3_l071240& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

// A weak reference: assigning takes the weak handle of what the other one refers to.
struct UnkWeakRef_l071240 {
    UnkWeakHandle* handle;

    UnkWeakRef_l071240& operator=(const UnkWeakRef_l071240& other);
};

inline UnkWeakRef_l071240& UnkWeakRef_l071240::operator=(const UnkWeakRef_l071240& other) {
    if (this != &other)
        CopyWeakRef(&handle, other.handle);
    return *this;
}

struct UnkCutsceneStream_l071240 : UnkCountedMedia_l071240 {
    UnkCutsceneRecord_l071240 record;  // 0x10
    UnkResourceRef_l071240 resource;  // 0x120
    UnkRenderEntries_l071240 field_0x128;
    UnkTunables_l071240 field_0x150;
    u64 field_0x178;
    UnkCutsceneState_l071240 state;  // 0x180
    u64 field_0x2f8;
    UnkSoundHandle sound;  // 0x300
    CFilePathStrId path;  // 0x310
    CFilePathStrId field_0x318;
    UnkCutsceneName_l071240 name;  // 0x320
    UnkCutsceneName_l071240 field_0x3e8;
    s32 field_0x4b0;
    s32 field_0x4b4;
    s32 field_0x4b8;
    f32 field_0x4bc;
    f32 field_0x4c0;
    f32 field_0x4c4;
    f32 field_0x4c8;
    bool field_0x4cc;
    bool field_0x4cd;
    UnkVec3_l071240 field_0x4d0;
    bool field_0x4dc;
    f32 field_0x4e0;
    f32 field_0x4e4;
    f32 field_0x4e8;
    f32 field_0x4ec;
    UnkWeakRef_l071240 field_0x4f0;
    UnkWeakRef_l071240 field_0x4f8;
    bool field_0x500;
    bool field_0x501;
    bool field_0x502;
    bool field_0x503;
    bool field_0x504;
    bool field_0x505;
    UnkVec3_l071240 field_0x508;
    UnkVec3_l071240 field_0x514;
    bool field_0x520;
    bool field_0x521;
    bool field_0x522;
    bool field_0x523;
    bool field_0x524;

    UnkCutsceneStream_l071240& operator=(const UnkCutsceneStream_l071240& other) asm("fn_7101071240");  // fn_7101071240
};
static_assert(__builtin_offsetof(UnkCutsceneStream_l071240, field_0x128) == 0x128, "layout");
static_assert(__builtin_offsetof(UnkCutsceneStream_l071240, path) == 0x310, "layout");
static_assert(__builtin_offsetof(UnkCutsceneStream_l071240, field_0x4b0) == 0x4b0, "layout");
static_assert(__builtin_offsetof(UnkCutsceneStream_l071240, field_0x4f0) == 0x4f0, "layout");
static_assert(__builtin_offsetof(UnkCutsceneStream_l071240, field_0x524) == 0x524, "layout");

UnkCutsceneStream_l071240& UnkCutsceneStream_l071240::operator=(const UnkCutsceneStream_l071240& other) = default;
