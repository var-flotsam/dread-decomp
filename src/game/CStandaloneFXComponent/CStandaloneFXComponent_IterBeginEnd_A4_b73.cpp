// CStandaloneFXComponent unit: the reflected begin (+0x160) and end (+0x168) slots of the array of 0x40-byte
// effect records whose destructor is fn_710081348c (src/game/CStandaloneFXComponent/fn_710081348c.cpp). The
// iterator caches its (index, record) pair in a holder that is empty, owns a copy, or refers to the iterator's
// own index and record (base/UnkCollectionIter.h's shape); the assignment resets an owned copy through the
// out-of-line destructor, while the temporary's destructor has it inlined. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/UnkModelManager.h"
#include "types.h"

void fn_71011a91c4(UnkModelManager* self, void* handle, u32 id);
struct UnkStruct_710081348c;
void fn_710081348c(UnkStruct_710081348c*);  // as CStandaloneFXComponent.cpp declares it

struct UnkFxRecordDesc_A4_b73 {
    u8 pad_0x0[0x40];
    u32 field_0x40;
};

struct UnkFxRecord_A4_b73 {
    UnkFxRecordDesc_A4_b73* desc;
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;  // 0x8
    u8 pad_0x10[0x28];
    void* handle;  // 0x38

    __attribute__((always_inline)) void Destroy() {
        if (handle) {
            if (!g_unk_71726b0cd8) {
                UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
                fn_71011a870c(m);
                g_unk_71726b0cd8 = m;
            }
            fn_71011a91c4(g_unk_71726b0cd8, handle, desc->field_0x40);
            handle = nullptr;
        }
        path.~UnkStrIdMemberOf();
    }
};
static_assert(sizeof(UnkFxRecord_A4_b73) == 0x40, "layout");

struct UnkFxRecordVector_A4_b73 {
    UnkFxRecord_A4_b73* data;
    u32 count;
};

struct UnkFxIterValue_A4_b73 {
    union {
        struct {
            u32 index;
            UnkFxRecord_A4_b73 element;
        } value;
        struct {
            const u32* index;
            UnkFxRecord_A4_b73* element;
        } ref;
    };
    s32 state;

    UnkFxIterValue_A4_b73() : state(0) {}
    __attribute__((always_inline)) ~UnkFxIterValue_A4_b73() {
        if (state == 2) {
        } else if (state == 1) {
            value.element.Destroy();
        } else {
            return;
        }
        state = 0;
    }
    __attribute__((always_inline)) void Reset() {
        if (state == 2) {
        } else if (state == 1) {
            fn_710081348c(reinterpret_cast<UnkStruct_710081348c*>(&value.element));
        } else {
            return;
        }
        state = 0;
    }
    __attribute__((always_inline)) void SetRef(const u32* i, UnkFxRecord_A4_b73* e) {
        Reset();
        state = 2;
        ref.index = i;
        ref.element = e;
    }
};

struct UnkFxIter_A4_b73 {
    u32 index;
    UnkFxRecord_A4_b73* element;
    UnkFxIterValue_A4_b73 current;

    __attribute__((always_inline)) UnkFxIter_A4_b73(u32 i, UnkFxRecord_A4_b73* e) : index(i), element(e) {
        if (e)
            current.SetRef(&index, e);
    }
    __attribute__((always_inline)) UnkFxIter_A4_b73& operator=(const UnkFxIter_A4_b73& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            if (element)
                current.SetRef(&index, element);
            else
                current.Reset();
        }
        return *this;
    }
};

void fn_7100814d00(UnkFxRecordVector_A4_b73* self, UnkFxIter_A4_b73* out) {
    *out = UnkFxIter_A4_b73(0, self->data);
}

void fn_7100814e60(UnkFxRecordVector_A4_b73* self, UnkFxIter_A4_b73* out) {
    *out = UnkFxIter_A4_b73(self->count, self->data + self->count);
}

// The record class's reflected move-assignment (class descriptor slot +0x50): the path id is released and taken
// over (the moved-from one gets the empty id of the pool loaded first), the rest copied, the handle included.
struct UnkFxRecordMove_A4_b73 {
    UnkFxRecordDesc_A4_b73* desc;
    base::global::UnkStringData* path;
    u32 field_0x10[9];
    void* handle;
};
static_assert(sizeof(UnkFxRecordMove_A4_b73) == 0x40, "layout");

void* fn_7100815068(void* dst, const void* src) {
    UnkFxRecordMove_A4_b73* self = static_cast<UnkFxRecordMove_A4_b73*>(dst);
    UnkFxRecordMove_A4_b73* other = static_cast<UnkFxRecordMove_A4_b73*>(const_cast<void*>(src));
    if (self != other) {
        self->desc = other->desc;
        base::global::UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (self->path && self->path != &pool->empty) {
                --self->path->ref_count;
                if (self->path->ref_count == 0) {
                    fn_71000803a4(pool, self->path);
                    pool = g_unk_7101d47200;
                }
            }
            self->path = &pool->empty;
        }
        self->path = other->path;
        other->path = &pool->empty;
        for (int i = 0; i < 9; ++i)
            self->field_0x10[i] = other->field_0x10[i];
        self->handle = other->handle;
    }
    return self;
}
