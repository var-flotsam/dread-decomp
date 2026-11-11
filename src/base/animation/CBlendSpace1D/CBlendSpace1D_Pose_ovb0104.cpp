// base::animation::CBlendSpace1D's unit (unit 13): the blend space's constructor and destructor
// (its entries are a vector of positions with shared sources; the mask at +0x40 comes from two
// guarded statics), the pose blend of its two current inputs, and the two shared_ptr constructors
// from a unique_ptr with a std::function deleter whose control blocks UnkSharedFunctionDtors.cpp
// covers. Names and layouts are placeholders.
#include <memory>
#include <functional>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/global/CStrId.h"
#include "game/UnkTU23.h"
#include "types.h"

extern void* g_unk_7101a19158[];

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkBlendStaticZero13 {
    u32 value;
    UnkBlendStaticZero13() : value(0) {}
};

inline const UnkBlendStaticZero13& UnkBlendStaticZero() {
    static UnkBlendStaticZero13 s_value asm("g_unk_7101d4bbd8");
    return s_value;
}

// The complement of that zero's low three bits.
inline u32 UnkBlendDefaultMask() {
    static u32 s_mask asm("g_unk_7101d4bbc8") = ~UnkBlendStaticZero().value & 7;
    return s_mask;
}

struct UnkBlendVector13 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkBlendSpace1D13 {
    void** vtable;
    u32 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    u64 field_0x20;
    base::global::CStrId name;  // 0x28
    u64 field_0x30;
    u64 field_0x38;
    u32 mask;  // 0x40
    UnkBlendVector13 entries;  // 0x48
};
static_assert(__builtin_offsetof(UnkBlendSpace1D13, entries) == 0x48, "layout");

void fn_7100006bc8(UnkBlendSpace1D13* self) {
    self->vtable = &g_unk_7101a19158[2];
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->name.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name.data = &g_unk_7101d47108->empty;
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->mask = UnkBlendDefaultMask();
    self->entries.data = nullptr;
    self->entries.count = 0;
    self->entries.capacity = 0;
    self->entries.step = 10;
    self->entries.field_0x14 = true;
    self->entries.flags = 0xf;
    self->entries.allocator = fn_7100083990();
}

extern void* g_unk_7101a19158[];

struct UnkBlendSourceShared13;

struct UnkBlendSpaceSlot13 {
    f32 position;
    std::shared_ptr<UnkBlendSourceShared13> source;  // 0x8
};

struct UnkBlendSlots13 {
    UnkBlendSpaceSlot13* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkBlendSlots13() {
        for (u64 i = 0; i < count; ++i)
            data[i].~UnkBlendSpaceSlot13();
        count = 0;
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkBlendSpace1DDtor13 {
    void** vtable;
    u8 pad_0x8[0x20];
    base::UnkStrIdMember name;  // 0x28
    u8 pad_0x30[0x18];
    UnkBlendSlots13 slots;  // 0x48

    __attribute__((always_inline)) ~UnkBlendSpace1DDtor13() { vtable = &g_unk_7101a19158[2]; }
};
static_assert(__builtin_offsetof(UnkBlendSpace1DDtor13, slots) == 0x48, "layout");

void fn_7100006d50(UnkBlendSpace1DDtor13* self) {
    self->~UnkBlendSpace1DDtor13();
}

struct UnkBlend13;
void fn_71000157f4(void* out, const void* a, const void* b, f32 t);

struct UnkBlendPose13 {
    u8 data[0x38];
    UnkBlendPose13(const UnkBlendPose13& other) { fn_7100014dfc(this, &other); }
    ~UnkBlendPose13() { fn_7100014e88(this); }
};

struct UnkBlendPoseSource13 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68(UnkBlendPose13* pose);
};

struct UnkBlendPoseTrack13 {
    u64 field_0x0;
    UnkBlendPoseSource13* source;  // 0x8
};

struct UnkBlendPoseInput13 {
    f32 weight;
    UnkBlendPoseTrack13* track;
};

struct UnkBlendPoseBlend13 {
    u8 pad_0x0[0x8];
    UnkBlendPoseInput13 inputs[2];  // 0x8
};

void fn_7100007638(UnkBlendPoseBlend13* self, UnkBlendPose13* pose) {
    fn_7100006e98(reinterpret_cast<UnkBlend13*>(self), 0);
    UnkBlendPoseInput13& a = self->inputs[0];
    UnkBlendPoseInput13& b = self->inputs[1];
    if (a.weight > 0.0f && a.track) {
        if (b.weight > 0.0f && b.track) {
            a.track->source->vf_0x68(pose);
            UnkBlendPose13 other(*pose);
            b.track->source->vf_0x68(&other);
            fn_71000157f4(pose, pose, &other, b.weight);
            return;
        }
        a.track->source->vf_0x68(pose);
    } else if (b.weight > 0.0f && b.track) {
        b.track->source->vf_0x68(pose);
    }
}

struct UnkSharedFnPointee;
struct UnkSharedFnPointee2;

using UnkSharedFnDeleter = std::function<void(UnkSharedFnPointee*)>;

// libc++'s __shared_ptr_pointer<T*, std::function<void(T*)>, allocator<T>> (as
// UnkSharedFunctionDtors.cpp lays it out), with the vtable each copy names.
// __shared_weak_count's counts (both zero), then the block's vtable.
template <char* Vtable>
struct UnkSharedFnCounts {
    char* vtable;
    long shared_owners;
    long shared_weak_owners;

    UnkSharedFnCounts() {
        shared_weak_owners = 0;
        vtable = Vtable + 0x10;
        shared_owners = 0;
    }
};

template <class T, char* Vtable>
struct UnkSharedFnCtrl : UnkSharedFnCounts<Vtable> {
    std::__compressed_pair<std::__compressed_pair<T*, std::function<void(T*)>>, std::allocator<T>> data;

    UnkSharedFnCtrl(T* p, std::function<void(T*)> d, std::allocator<T> a)
        : data(std::__compressed_pair<T*, std::function<void(T*)>>(p, std::move(d)), std::move(a)) {}
};

template <class T>
struct UnkSharedFnPtr {
    T* ptr;
    void* ctrl;
};

// shared_ptr<T>'s constructor from a unique_ptr<T, std::function<void(T*)>>&&.
template <class T, char* Vtable>
__attribute__((always_inline)) static inline void ConstructShared(UnkSharedFnPtr<T>* self,
                                                                  std::unique_ptr<T, std::function<void(T*)>>* r) {
    self->ptr = r->get();
    if (self->ptr == nullptr)
        self->ctrl = nullptr;
    else
        self->ctrl = new UnkSharedFnCtrl<T, Vtable>(r->get(), r->get_deleter(), std::allocator<T>());
    r->release();
}

extern char g_unk_7101a19230[];
extern char g_unk_7101a19268[];

void fn_7100008bbc(UnkSharedFnPtr<UnkSharedFnPointee>* self,
                   std::unique_ptr<UnkSharedFnPointee, std::function<void(UnkSharedFnPointee*)>>* r) {
    ConstructShared<UnkSharedFnPointee, g_unk_7101a19230>(self, r);
}

void fn_7100008ea4(UnkSharedFnPtr<UnkSharedFnPointee2>* self,
                   std::unique_ptr<UnkSharedFnPointee2, std::function<void(UnkSharedFnPointee2*)>>* r) {
    ConstructShared<UnkSharedFnPointee2, g_unk_7101a19268>(self, r);
}
