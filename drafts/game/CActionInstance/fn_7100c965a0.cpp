// CActionInstance's unit: starts an action instance. When its source holds an asset and its target
// holds a blend skeleton transformer, the transformer gets a shared pointer made from the asset; then
// the two event lists are notified and the instance hands itself out to its target.
// Names and layouts are placeholders.
#include <functional>
#include <memory>

#include "base/UnkCountedResource.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkClassGetters.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkEventDispatchA1;
void fn_7100068e08(UnkEventDispatchA1* self);

struct UnkBlendTransformer_b66a1;
void fn_7100003a40(UnkBlendTransformer_b66a1* self, const std::shared_ptr<void>& shared);

struct UnkSharedFnPointee2;

template <class T>
struct UnkSharedFnPtr {
    T* ptr;
    void* ctrl;
};

void fn_7100008ea4(UnkSharedFnPtr<UnkSharedFnPointee2>* self,
                   std::unique_ptr<UnkSharedFnPointee2, std::function<void(UnkSharedFnPointee2*)>>* r);

struct UnkAssetResRef {
    UnkCountedResource* ptr;
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

using UnkMadeUnique_lc965a0 = std::unique_ptr<UnkSharedFnPointee2, std::function<void(UnkSharedFnPointee2*)>>;

// Makes the object the transformer shares from the asset (null when there is none).
UnkMadeUnique_lc965a0 fn_7100013734(UnkAssetResRef asset);

// The made object, shared (std::shared_ptr's layout), built from the unique one.
struct UnkMadeShared_lc965a0 {
    UnkSharedFnPointee2* ptr;
    std::__shared_weak_count* ctrl;

    explicit UnkMadeShared_lc965a0(UnkMadeUnique_lc965a0&& made) {
        fn_7100008ea4(reinterpret_cast<UnkSharedFnPtr<UnkSharedFnPointee2>*>(this), &made);
    }
    ~UnkMadeShared_lc965a0() {
        if (ctrl)
            ctrl->__release_shared();
    }

    const std::shared_ptr<void>& Get() const { return reinterpret_cast<const std::shared_ptr<void>&>(*this); }
};

struct UnkActionSource_lc965a0 {
    u8 pad_0x0[0x50];
    UnkCountedResource* asset;  // 0x50
};

struct UnkActionTarget_lc965a0 {
    u8 pad_0x0[0x18];
    UnkReflObject* transformer;  // 0x18
};

struct UnkActionInstance_lc965a0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // the last reference was dropped
    s32 ref_count;           // 0x8
    UnkActionSource_lc965a0* source;  // 0x10
    u8 pad_0x18[0x8];
    UnkActionTarget_lc965a0* target;  // 0x20
    u8 pad_0x28[0x8];
    u8 events_0x30[0x48];
    u8 events_0x78[0x48];
};

void fn_710070da30(UnkActionTarget_lc965a0* target, UnkActionInstance_lc965a0** instance);

static inline bool IsAsset(UnkCountedResource* res) {
    auto* obj = reinterpret_cast<UnkReflObject*>(res);
    return fn_710009d50c(obj->vf_0x0(), fn_7100016894());
}

// A counted reference to the resource when it is an asset, else null.
static inline UnkAssetResRef AsAsset(UnkCountedResource* res) {
    UnkAssetResRef ref{nullptr};
    if (res && IsAsset(res)) {
        ++res->ref_count;
        ref.ptr = res;
    }
    return ref;
}

void fn_7100c965a0(UnkActionInstance_lc965a0* self, UnkActionInstance_lc965a0** out) {
    if (self->source->asset) {
        UnkReflObject* transformer = self->target->transformer;
        if (transformer && fn_710009d50c(transformer->vf_0x0(), fn_7100003520()))
            fn_7100003a40(reinterpret_cast<UnkBlendTransformer_b66a1*>(transformer),
                          UnkMadeShared_lc965a0(fn_7100013734(AsAsset(self->source->asset))).Get());
    }
    fn_7100068e08(reinterpret_cast<UnkEventDispatchA1*>(self->events_0x78));
    fn_7100068e08(reinterpret_cast<UnkEventDispatchA1*>(self->events_0x30));
    if (*out != self) {
        ++self->ref_count;
        if (UnkActionInstance_lc965a0* old = *out) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        *out = self;
    }
    fn_710070da30(self->target, out);
}
