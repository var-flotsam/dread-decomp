// CAnimationComponent unit (tools/tu_map.py, unit 2720): fn_7101175df4 copies the owner's character
// class animation resources whose mask bits are all enabled into the component's list, then sets the
// tracks up when the animation class names a path. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1643.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::reflection::UnkReflObject;

// An animation resource: counted, with the mask of the states it is for.
struct UnkAnimRes_l175df4 : UnkCountedResource {
    u8 pad_0x58[0x8];
    u32 mask;  // 0x60
};

// A counted reference to an animation resource.
struct UnkAnimRef_l175df4 {
    UnkAnimRes_l175df4* ptr;

    UnkAnimRef_l175df4(const UnkAnimRef_l175df4& other) : ptr(nullptr) { *this = other; }
    UnkAnimRef_l175df4& operator=(const UnkAnimRef_l175df4& other) {
        if (ptr != other.ptr) {
            UnkAnimRes_l175df4* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkAnimRes_l175df4* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkAnimRef_l175df4() {
        UnkAnimRes_l175df4* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

// The character class's animation resources by name (a CRntSmallDictionary of CStrId keys).
struct UnkAnimResDict_l175df4 {
    UnkAnimRef_l175df4* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// The dictionary's copy constructor, out of line.
void fn_71002a6ab8(UnkAnimResDict_l175df4* self, const UnkAnimResDict_l175df4* other);

namespace {

// A copy of a dictionary that lives for one scope: built by the copy constructor, destroyed (keys, then
// values) and freed at the end.
struct ScopedAnimResDict : UnkAnimResDict_l175df4 {
    explicit ScopedAnimResDict(const UnkAnimResDict_l175df4& other) { fn_71002a6ab8(this, &other); }
    ~ScopedAnimResDict() {
        if (keys) {
            for (u64 i = 0; i < count; ++i)
                keys[i].~CStrId();
        }
        if (values) {
            for (u64 i = 0; i < count; ++i)
                values[i].~UnkAnimRef_l175df4();
        }
        count = 0;
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

}  // namespace

struct UnkAnimCharClass_l175df4 {
    u8 pad_0x0[0xd0];
    UnkAnimResDict_l175df4 anims;  // 0xd0
};

struct UnkAnimClass_l175df4 {
    u8 pad_0x0[0x68];
    CFilePathStrId path;  // 0x68
};

struct UnkAnimOwner_l175df4 {
    u8 pad_0x0[0x28];
    UnkReflObject* char_class;  // 0x28
};

// As src/game/CAnimationComponent/CAnimationComponent_Tracks_ovb0009.cpp declares them.
struct UnkAnimTracks_617c {
    virtual void vf_0x0();
    virtual ~UnkAnimTracks_617c();
};

template <class T>
struct UnkOwnedPtr {
    T* ptr;

    UnkOwnedPtr() : ptr(nullptr) {}
    ~UnkOwnedPtr() { Reset(nullptr); }
    void Reset(T* p) {
        T* old = ptr;
        ptr = p;
        if (old)
            delete old;
    }
    T* Release() {
        T* p = ptr;
        ptr = nullptr;
        return p;
    }
    UnkOwnedPtr& operator=(UnkOwnedPtr&& o) {
        Reset(o.Release());
        return *this;
    }
};

UnkOwnedPtr<UnkAnimTracks_617c> fn_7100db27a8(void* factory);

struct UnkAnimResRef_b52;
void fn_710117c2e4(base::global::CRntVector<UnkAnimResRef_b52>* self, u32 capacity);
void fn_71002d589c(char* p0, char* p1);

struct UnkAnimComponent_l175df4 {
    u8 pad_0x0[0x10];
    UnkReflObject* anim_class;  // 0x10
    u8 pad_0x18[0x18];
    UnkAnimOwner_l175df4* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkOwnedPtr<UnkAnimTracks_617c> tracks;     // 0x40
    base::global::CRntVector<UnkAnimRef_l175df4> anims;  // 0x48
    u32 enabled_mask;                           // 0x70
};
static_assert(__builtin_offsetof(UnkAnimComponent_l175df4, enabled_mask) == 0x70, "");

static inline UnkAnimCharClass_l175df4* CharClassOf(UnkAnimOwner_l175df4* owner) {
    UnkReflObject* obj = owner->char_class;
    return obj && fn_710009d50c(obj->vf_0x0(), fn_71011abbd4()) ? reinterpret_cast<UnkAnimCharClass_l175df4*>(obj)
                                                                : nullptr;
}

// Appends a copy of `value`, growing the array by its step (CRntVector's PushBackCopy, with the reserve
// out of line).
inline void PushBack(base::global::CRntVector<UnkAnimRef_l175df4>& v, const UnkAnimRef_l175df4& value) {
    u32 n = v.count;
    u32 new_capacity = v.capacity;
    if (new_capacity <= n && v.step != 0)
        new_capacity = (n / v.step + 1) * v.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_710117c2e4(reinterpret_cast<base::global::CRntVector<UnkAnimResRef_b52>*>(&v), new_capacity);
    new (&v.data[v.count]) UnkAnimRef_l175df4(value);
    ++v.count;
}

bool fn_7101175df4(UnkAnimComponent_l175df4* self) {
    if (!CharClassOf(self->owner))
        return false;
    ScopedAnimResDict anims(CharClassOf(self->owner)->anims);
    // Newest first.
    UnkAnimRef_l175df4* first = anims.values;
    for (UnkAnimRef_l175df4* it = first + anims.count; it != first;) {
        UnkAnimRef_l175df4 anim(*--it);
        // Only resources whose states are all enabled.
        if ((~self->enabled_mask & anim.ptr->mask) != 0)
            continue;
        PushBack(self->anims, anim);
    }
    UnkReflObject* obj = self->anim_class;
    UnkAnimClass_l175df4* anim_class =
        obj && fn_710009d50c(obj->vf_0x0(), fn_71007105e4()) ? reinterpret_cast<UnkAnimClass_l175df4*>(obj) : nullptr;
    if (anim_class->path.data != &g_unk_7101d47200->empty) {
        if (!self->tracks.ptr)
            self->tracks = fn_7100db27a8(g_unk_7172758b18);
        fn_71002d589c(reinterpret_cast<char*>(self->tracks.ptr), reinterpret_cast<char*>(self->owner));
    }
    return true;
}
