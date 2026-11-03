// More of the GUI manager's unit (src/game/GUI/CGUIManager/fn_71011b7f48.cpp has the frame helpers).
// Names and layouts are placeholders.
#include <math.h>

#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkGuiRoundFrame {
    u8 pad_0x0[0x8];
    s32 key;
    u8 pad_0xc[0x18];
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    u8 pad_0x30[0x18];
};
static_assert(sizeof(UnkGuiRoundFrame) == 0x48, "UnkGuiRoundFrame layout");

struct UnkGuiRoundFrames {
    u8 pad_0x0[0x40];
    UnkGuiRoundFrame* frames;
};

// The value scaled by the frame `key` names (+0x24 and +0x2c), each rounded down after adding a half.
UnkLabelVec2 fn_71011b80b8(s32 key, const UnkLabelVec2& v) {
    UnkGuiRoundFrame* f = reinterpret_cast<UnkGuiRoundFrames*>(g_unk_717271e8f8)->frames;
    while (f->key != key)
        ++f;
    return UnkLabelVec2(floorf(v.x * f->field_0x24 + 0.5f), floorf(v.y * f->field_0x2c + 0.5f));
}

// A cache of objects keyed by file-path ids (a hashed map at +0x100: nodes with the key first,
// the object at +0x8 and the chain at +0x20; ids hash by their data's 64-bit hash at +0x30, the two
// halves xored, modulo the divisor at +0x114).
using base::global::CFilePathStrId;
using base::global::UnkStringData;

struct UnkGuiPathKeyData {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkGuiPathNode {
    UnkGuiPathKeyData* key;
    void* value;
    u8 pad_0x10[0x10];
    UnkGuiPathNode* next;
};

struct UnkGuiPathMap {
    u64 field_0x0;
    UnkGuiPathNode** buckets;
    u32 count;
    u32 divisor;

    UnkGuiPathNode** BucketFor(UnkGuiPathKeyData* key) {
        if (count == 0)
            return nullptr;
        u32 idx = (key->hash_lo ^ key->hash_hi) % divisor;
        return buckets + idx;
    }
    UnkGuiPathNode* Find(UnkGuiPathKeyData* key) {
        UnkGuiPathNode** bucket = BucketFor(key);
        if (!bucket)
            return nullptr;
        for (UnkGuiPathNode* n = *bucket; n; n = n->next)
            if (n->key == key)
                return n;
        return nullptr;
    }
};

struct UnkGuiPathCache {
    u8 pad_0x0[0x100];
    UnkGuiPathMap map;
};

CFilePathStrId fn_71011b85fc(UnkGuiPathCache* self);
void* fn_71011b8470(UnkGuiPathCache* self, const CFilePathStrId* key);

// The object cached for the current path (fn_71011b85fc), loading it (fn_71011b8470) when it is
// not cached yet; null for the empty path.
void* fn_71011b882c(UnkGuiPathCache* self) {
    CFilePathStrId key = fn_71011b85fc(self);
    if (key.data == &g_unk_7101d47200->empty)
        return nullptr;
    if (UnkGuiPathNode* n = self->map.Find(reinterpret_cast<UnkGuiPathKeyData*>(key.data)))
        return n->value;
    return fn_71011b8470(self, &key);
}
