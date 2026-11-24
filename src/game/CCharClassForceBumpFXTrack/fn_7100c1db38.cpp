// Destructors of two classes holding a string id at 0x18 and an array of string ids at 0x20:
// the class's vtable is set, the array's ids are released and the array freed, the id at 0x18
// is released, then the base destructor (fn_710006b5dc) runs. Also two destructors of a key
// with an array of id/word records.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "base/global/timeline/CTrack.h"
#include "game/CCharClassCooldownXBossDamageShapesTrack.h"
#include "game/CCharClassForceBumpFXTrack.h"
#include "game/CCharClassIgnoreBumpFXTrack.h"
#include "game/CCharClassModifyDamageTrack.h"
#include "game/UnkIdWord.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;


template <class T>
struct UnkReleasedArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    __attribute__((always_inline)) ~UnkReleasedArray() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~T();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

template <void** Vtable>
struct UnkIdListHolder {
    void** vtable;
    u8 unk_0x8[0x10];
    UnkStrIdTwoStep id;
    UnkReleasedArray<UnkStrIdTwoStep> ids;

    __attribute__((always_inline)) ~UnkIdListHolder() { vtable = Vtable + 2; }
};

template <void** Vtable>
__attribute__((always_inline)) static inline void Destroy(UnkIdListHolder<Vtable>* self) {
    self->~UnkIdListHolder();
    fn_710006b5dc(self);
}

void fn_7100c1db38(UnkIdListHolder<g_unk_7101aa3030>* self) { Destroy(self); }
void fn_7100c274e8(UnkIdListHolder<g_unk_7101aa3440>* self) { Destroy(self); }

struct UnkIdWordList {
    UnkStrIdTwoStep key;
    UnkReleasedArray<UnkIdWord> entries;
};

void fn_7100b480a0(UnkIdWordList* self) { self->~UnkIdWordList(); }
void fn_7100ea8cd4(UnkIdWordList* self) { self->~UnkIdWordList(); }

// The same with path ids (the entries' after a word), in another unit.
struct UnkPathIdWord {
    u64 field_0x0;
    UnkPathIdTwoStep id;
};

struct UnkPathIdWordList {
    UnkPathIdTwoStep key;
    UnkReleasedArray<UnkPathIdWord> entries;
};

void fn_71011aa9d8(UnkPathIdWordList* self) { self->~UnkPathIdWordList(); }

// The same with a second string id at 0x48 and a word (released first).
template <void** Vtable>
struct UnkIdListIdHolder {
    void** vtable;
    u8 unk_0x8[0x10];
    UnkStrIdTwoStep id;
    UnkReleasedArray<UnkStrIdTwoStep> ids;
    UnkStrIdTwoStep id_0x48;
    u32 field_0x50;

    __attribute__((always_inline)) ~UnkIdListIdHolder() { vtable = Vtable + 2; }
};

template <void** Vtable>
__attribute__((always_inline)) static inline void Destroy(UnkIdListIdHolder<Vtable>* self) {
    self->~UnkIdListIdHolder();
    fn_710006b5dc(self);
}

void fn_7100c12a84(UnkIdListIdHolder<g_unk_7101aa2b80>* self) { Destroy(self); }
void fn_7100d21ff8(UnkIdListIdHolder<g_unk_7101aa7d00>* self) { Destroy(self); }
