// The game manager (CGameManager): the lookup that replaces a string id by another.
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"

using base::global::CStrId;

CStrId fn_7100dfbda4(UnkGameManager* self, const CStrId& id) {
    CStrId* it = self->field_0x1000.Find(id);
    if (it != self->field_0x1000.values + self->field_0x1000.count)
        return *it;
    return id;
}

// The object behind +0x498 starts with a pointer-sized field, then a string.
bool fn_7100deabe4(UnkGameManager* self) {
    char* object = static_cast<char*>(self->field_0x498);
    if (object)
        return fn_710007f140(reinterpret_cast<const base::global::CRntString*>(object + 8), "EDITOR");
    return false;
}

void* fn_7100defcec(UnkGameManager* self) {
    UnkGameManagerTarget* target = fn_7100decab0(self);
    return target ? target->field_0x248 : nullptr;
}

__attribute__((always_inline)) static inline void* FindInMap(UnkGameManagerNode** buckets, u32 count, u32 capacity, u64 id) {
    if (count) {
        UnkGameManagerNode** bucket = buckets + (((u32)(id >> 32) ^ (u32)id) % capacity);
        if (bucket) {
            for (UnkGameManagerNode* n = *bucket; n; n = n->next) {
                if (n->key->id == id)
                    return n->value;
            }
        }
    }
    return nullptr;
}

// NON_MATCHING(minor): same behaviour; the original merges the two maps' hits before the checks of
// the flags and schedules the loads of each map's capacity ahead of the hash (64 of 79 instructions
// differ). The maps are read through byte offsets because a nested member access makes Clang compute
// the capacity's address separately.
void* fn_7100deba8c(UnkGameManager* self, const u64* id, const u32* flags) {
    char* base = reinterpret_cast<char*>(self);
    if (*id == (u64)-1)
        return nullptr;
    {
        void* found = FindInMap(*reinterpret_cast<UnkGameManagerNode***>(base + 0x2608), *reinterpret_cast<u32*>(base + 0x2610), *reinterpret_cast<u32*>(base + 0x2614), *id);
        if (!found)
            found = FindInMap(*reinterpret_cast<UnkGameManagerNode***>(base + 0x2650), *reinterpret_cast<u32*>(base + 0x2658), *reinterpret_cast<u32*>(base + 0x265c), *id);
        if (found) {
            if (!(*flags & 8)) {
                for (UnkGameManagerListNode* n = self->field_0x2778; n; n = n->next) {
                    if (n->object == found)
                        goto fallback;
                }
            }
            return found;
        }
        if (*flags & 4) {
            for (UnkGameManagerListNode* n = self->field_0x2698; n; n = n->next) {
                if (n->object) {
                    char* entry = static_cast<char*>(n->object);
                    if (*reinterpret_cast<u64*>(*reinterpret_cast<char**>(entry + 0x30) + 0x30) == *id)
                        return entry;
                }
            }
        }
    }
fallback:
    if (*flags & 2) {
        if (self->field_0x608)
            return fn_71011a3d74(self->field_0x608, id);
    }
    return nullptr;
}
