// Unit 2730 (tools/tu_map.py): removes from the manager's list of counted object references the one whose object
// has the given file path (the first such, keeping the order of the rest). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using UnkPathIdMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// A listed object: counted (atomically) and named by a file path.
struct UnkPathObject_l19a110 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
    u8 pad_0xc[0x304];
    base::global::UnkStringData* path;  // 0x310
};
static_assert(__builtin_offsetof(UnkPathObject_l19a110, path) == 0x310, "layout");

// A counted reference to one.
struct UnkPathObjectRef_l19a110 {
    UnkPathObject_l19a110* ptr;

    UnkPathObjectRef_l19a110(const UnkPathObjectRef_l19a110& other) : ptr(nullptr) { *this = other; }
    UnkPathObjectRef_l19a110& operator=(const UnkPathObjectRef_l19a110& other) {
        UnkPathObject_l19a110* p = other.ptr;
        if (p)
            __atomic_fetch_add(&p->ref_count, 1, __ATOMIC_SEQ_CST);
        if (UnkPathObject_l19a110* old = ptr)
            Release(old);
        ptr = p;
        return *this;
    }
    ~UnkPathObjectRef_l19a110() {
        if (UnkPathObject_l19a110* p = ptr)
            Release(p);
    }
    static void Release(UnkPathObject_l19a110* p) {
        __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
        if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
            p->vf_0x10();
    }
};

using UnkPathObjectList_l19a110 = base::global::CRntVector<UnkPathObjectRef_l19a110>;

struct UnkPathObjectOwner_l19a110 {
    u8 pad_0x0[0xe0];
    UnkPathObjectList_l19a110 objects;  // 0xe0
};

static inline UnkPathObjectRef_l19a110* FindByPath(UnkPathObjectRef_l19a110* it, UnkPathObjectRef_l19a110* end,
                                                   CFilePathStrId key) {
    if (it != end) {
        if (it->ptr->path == key.data)
            return it;
        for (++it; it != end; ++it)
            if (it->ptr->path == key.data)
                return it;
    }
    return end;
}

// Erases the reference at `index`, moving the later ones down one by one.
static inline void EraseAt(UnkPathObjectList_l19a110& list, u32 index) {
    list.data[index].~UnkPathObjectRef_l19a110();
    for (u64 i = index; i < list.count - 1; ++i) {
        UnkPathObjectRef_l19a110* p = &list.data[i];
        new (p) UnkPathObjectRef_l19a110(p[1]);
        list.data[i + 1].~UnkPathObjectRef_l19a110();
    }
    list.count = list.count - 1;
}

// Erases the first reference to `object`.
static inline void Remove(UnkPathObjectList_l19a110& list, UnkPathObject_l19a110* object) {
    for (u32 i = 0; i < list.count; ++i) {
        if (list.data[i].ptr == object) {
            EraseAt(list, i);
            return;
        }
    }
}

void fn_710119a110(void* root, const UnkPathIdMember* id) {
    UnkPathObjectList_l19a110& list = static_cast<UnkPathObjectOwner_l19a110*>(root)->objects;
    UnkPathObjectRef_l19a110* it =
        FindByPath(list.data, list.data + list.count, reinterpret_cast<const CFilePathStrId&>(*id));
    if (list.count == 0 || it == list.data + list.count)
        return;
    Remove(list, it->ptr);
}
