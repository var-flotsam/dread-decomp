// The rumble manager's unit (tools/tu_map.py unit 51, base::input::CRumbleManager::CTunableRumbleManager):
// removes the finished effects (every effect when asked to stop all), stopping the playing ones and dropping
// their rumble files, then frees the cached files no effect uses any more (every cached file when asked to
// unload all). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/global/CRntVector.h"
#include "game/operator_new.h"
#include "types.h"

struct UnkRumbleManager;

using FileKey = base::UnkStrIdMemberOf<g_unk_7101d47200>;

struct UnkFileHashNode {
    FileKey key;
    u8 unk_0x8[0x8];
    // Union members: the original reloads them after each link store.
    union {
        UnkFileHashNode* next;  // list order
    };
    union {
        UnkFileHashNode* prev;
    };
    UnkFileHashNode* chain;  // bucket chain
};

struct UnkFileHashMap {
    base::UnkAllocator* allocator;
    u32 field_0x8;
    u32 count;
    UnkFileHashNode** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 unk_0x20[0x18];
    UnkFileHashNode* tail;
    UnkFileHashNode* head;
};

void* fn_71000741d4(UnkFileHashMap* m, const FileKey* key);

// A handle to a playing rumble: current while the target's serial is the handle's.
struct UnkHandleTarget {
    void* field_0x0;
    u64 field_0x8;
};

UnkHandleTarget* fn_7100075b9c(void* slots, u32 index);
void fn_7100083834(UnkVoicePool_b56* self, UnkSoundVoice2_b56* voice);

// A loaded rumble file: its resource and its path (the cache's key).
struct UnkRumbleFile_l073f48 {
    UnkCountedResource* resource;
    u8 pad_0x8[0x10];
    FileKey path;  // +0x18
};

// The node of the file cache as this function reads it: the file after the key.
struct UnkCachedFile_l073f48 {
    FileKey key;
    UnkRumbleFile_l073f48* file;  // +0x8
};

struct UnkRumbleEffect_l073f48 {
    u8 pad_0x0[0x68];
    UnkRumbleFile_l073f48* file;  // +0x68
    u8 pad_0x70[0x2c];
    bool playing;  // +0x9c
    u8 pad_0x9d[6];
    bool finished;  // +0xa3
};

void fn_7100070548(UnkRumbleManager* self, UnkRumbleEffect_l073f48* effect);
void fn_7100071e9c(UnkRumbleEffect_l073f48* effect);

struct UnkRumbleManager_l073f48 {
    u8 pad_0x0[0x20d0];
    UnkFileHashMap files;  // +0x20d0
    u8 pad_0x2118[0x40];
    base::global::CRntVector<UnkRumbleEffect_l073f48*> effects;  // +0x2158
    u8 pad_0x2180[0x10];
    u8 effect_blocks[0x28];  // +0x2190: the effects' pool
    u8 slots[0x28];          // +0x21b8: a vector of 16-byte slots (UnkHandleTarget)
    u64 field_0x21e0;        // effects freed
};
static_assert(__builtin_offsetof(UnkRumbleManager_l073f48, effects) == 0x2158, "layout");
static_assert(__builtin_offsetof(UnkRumbleManager_l073f48, field_0x21e0) == 0x21e0, "layout");

// Whether the cache holds a file under `key`.
static inline bool Contains(const UnkFileHashMap* m, const FileKey* key) {
    if (!m->field_0x18)
        return false;
    base::global::UnkStringData* k = key->data;
    u32 h = k->hash[0] ^ k->hash[1];
    u32 i = h % m->bucket_count;
    UnkFileHashNode** slot = m->buckets + i;
    if (!slot)
        return false;
    for (UnkFileHashNode* n = *slot; n; n = n->chain) {
        if (n->key.data == k)
            return true;
    }
    return false;
}

// Erases the element at `it` and returns where the next one now is.
template <class T>
static inline T* Erase(base::global::CRntVector<T>& v, T* it) {
    u32 index = it - v.data;
    v.EraseAt(index);
    return v.data + index;
}

void fn_7100073f48(UnkRumbleManager* manager, u32 stopAll, u32 unloadAll) {
    UnkRumbleManager_l073f48* self = reinterpret_cast<UnkRumbleManager_l073f48*>(manager);
    if (self->effects.count) {
        UnkRumbleEffect_l073f48** it = self->effects.data;
        do {
            UnkRumbleEffect_l073f48* effect = *it;
            if (!(stopAll & 1) && !(unloadAll & 1) && !effect->finished) {
                ++it;
                continue;
            }
            if (effect->playing) {
                UnkRumbleFile_l073f48* file = effect->file;
                if (Contains(&self->files, &file->path)) {
                    UnkCountedResource* resource = file->resource;
                    if (--resource->ref_count == 0) {
                        fn_710001737c(resource);
                        fn_71000741d4(&self->files, &file->path);
                        GetDefaultAllocator()->vf_0x48(file);
                    }
                }
                fn_7100070548(manager, effect);
                fn_7100071e9c(effect);
            }
            it = Erase(self->effects, it);
            if (effect) {
                u32 index = fn_7100083854(reinterpret_cast<UnkVoicePool_b56*>(self->effect_blocks),
                                          reinterpret_cast<UnkSoundVoice2_b56*>(effect));
                ++fn_7100075b9c(self->slots, index)->field_0x8;
                fn_7100083834(reinterpret_cast<UnkVoicePool_b56*>(self->effect_blocks),
                              reinterpret_cast<UnkSoundVoice2_b56*>(effect));
                ++self->field_0x21e0;
            }
        } while (self->effects.data + self->effects.count != it);
    }

    UnkFileHashNode* node = self->files.head;
    if (!node)
        return;
    if (unloadAll & 1) {
        while (node) {
            UnkRumbleFile_l073f48* file = reinterpret_cast<UnkCachedFile_l073f48*>(node)->file;
            if (UnkCountedResource* resource = file->resource) {
                fn_710001737c(resource);
                node = static_cast<UnkFileHashNode*>(fn_71000741d4(&self->files, &file->path));
                GetDefaultAllocator()->vf_0x48(file);
            } else {
                node = node->next;
            }
        }
    } else {
        while (node) {
            UnkRumbleFile_l073f48* file = reinterpret_cast<UnkCachedFile_l073f48*>(node)->file;
            UnkCountedResource* resource = file->resource;
            if (resource && resource->ref_count == 0) {
                resource->ref_count = -1;
                node = static_cast<UnkFileHashNode*>(fn_71000741d4(&self->files, &file->path));
                GetDefaultAllocator()->vf_0x48(file);
            } else {
                node = node->next;
            }
        }
    }
}
