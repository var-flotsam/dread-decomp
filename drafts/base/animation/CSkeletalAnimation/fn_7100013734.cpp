// The base::animation::CSkeletalAnimation unit: creates a playback of the animation resource a holder
// points to, taken from the unit's block pool and owned by a pointer whose std::function-style deleter
// gives it back, and sets its step from the resource's length. Names and layouts are placeholders.
#include <new>
#include <utility>

#include "base/UnkCountedResource.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/operator_new.h"
#include "types.h"

[[noreturn]] void fn_7100014160();  // throws bad_function_call

extern char g_unk_7101a19380[];
extern char g_unk_7101a19410[];
extern f32 g_unk_7101647568;

// The pool the playbacks come from: its blocks at +0x10 and how many may still be taken at +0x60.
struct UnkPlaybackPool_l013734 {
    u8 pad_0x0[0x10];
    u8 blocks[0x50];  // 0x10
    u64 free_count;   // 0x60
};
extern UnkPlaybackPool_l013734 g_unk_7101d4d418;

// A counted reference to a resource (count at +0x54).
struct UnkResourceRef_l013734 {
    UnkCountedResource* resource;

    explicit UnkResourceRef_l013734(UnkCountedResource* r) : resource(r) {
        if (resource)
            resource->ref_count++;
    }
    ~UnkResourceRef_l013734() {
        if (resource && --resource->ref_count == 0)
            fn_710001737c(resource);
    }
    UnkResourceRef_l013734& operator=(const UnkResourceRef_l013734& other) {
        if (resource != other.resource) {
            if (other.resource)
                other.resource->ref_count++;
            if (resource && --resource->ref_count == 0)
                fn_710001737c(resource);
            resource = other.resource;
        }
        return *this;
    }
};

struct UnkPlayback_l013734;

struct UnkPlaybackVtable_l013734 {
    void* slots[7];
    f32 (*GetLength)(UnkPlayback_l013734* self);  // 0x38
};

struct UnkPlayback_l013734 {
    const UnkPlaybackVtable_l013734* vtable;  // 0x0
    UnkResourceRef_l013734 resource;          // 0x8
    f32 rate;                                 // 0x10
    f32 step;                                 // 0x14
    u32 field_0x18;                           // 0x18

    UnkPlayback_l013734()
        : vtable(reinterpret_cast<const UnkPlaybackVtable_l013734*>(g_unk_7101a19380 + 0x10)), resource(nullptr),
          rate(1.0f), step(0.0f), field_0x18(0) {}
};

// The callable a std::function<void(UnkPlayback*)> holds, as libc++ lays out its interface.
struct UnkFnTarget_l013734;

struct UnkFnTargetVtable_l013734 {
    void* dtors[2];
    void* clone;
    void (*clone_into)(const UnkFnTarget_l013734* self, UnkFnTarget_l013734* dst);  // 0x18
    void (*destroy)(UnkFnTarget_l013734* self);                                     // 0x20
    void (*destroy_deallocate)(UnkFnTarget_l013734* self);                          // 0x28
    void (*call)(UnkFnTarget_l013734* self, UnkPlayback_l013734** arg);             // 0x30
};

struct UnkFnTarget_l013734 {
    const UnkFnTargetVtable_l013734* vtable;

    explicit UnkFnTarget_l013734(char* table) : vtable(reinterpret_cast<const UnkFnTargetVtable_l013734*>(table)) {}
};

// The deleter's callable: gives a playback back to the pool it captures.
struct UnkPoolReturn_l013734 : UnkFnTarget_l013734 {
    UnkPlaybackPool_l013734* pool;

    explicit UnkPoolReturn_l013734(UnkPlaybackPool_l013734* p) : UnkFnTarget_l013734(g_unk_7101a19410 + 0x10), pool(p) {}
};

// std::function<void(UnkPlayback*)>: a small buffer, then the target.
struct UnkPlaybackDeleter_l013734 {
    alignas(16) u8 buf[0x20];
    UnkFnTarget_l013734* target;

    UnkPlaybackDeleter_l013734() : target(nullptr) {}
    explicit UnkPlaybackDeleter_l013734(UnkPlaybackPool_l013734* pool) { target = new (buf) UnkPoolReturn_l013734(pool); }
    UnkPlaybackDeleter_l013734(UnkPlaybackDeleter_l013734&& other) { Take(other); }
    ~UnkPlaybackDeleter_l013734() {
        if (target == Buffer())
            target->vtable->destroy(target);
        else if (target)
            target->vtable->destroy_deallocate(target);
    }

    UnkFnTarget_l013734* Buffer() { return reinterpret_cast<UnkFnTarget_l013734*>(buf); }

    void Clear() {
        UnkFnTarget_l013734* t = target;
        target = nullptr;
        if (t == Buffer())
            t->vtable->destroy(t);
        else if (t)
            t->vtable->destroy_deallocate(t);
    }

    void Take(UnkPlaybackDeleter_l013734& other) {
        if (!other.target) {
            target = nullptr;
        } else if (other.target == other.Buffer()) {
            target = Buffer();
            other.target->vtable->clone_into(other.target, target);
        } else {
            target = other.target;
            other.target = nullptr;
        }
    }

    UnkPlaybackDeleter_l013734& operator=(UnkPlaybackDeleter_l013734&& other) {
        Clear();
        Take(other);
        return *this;
    }

    void operator()(UnkPlayback_l013734* playback) {
        if (!target)
            fn_7100014160();
        target->vtable->call(target, &playback);
    }
};

// std::unique_ptr<UnkPlayback, std::function<void(UnkPlayback*)>>.
struct UnkPlaybackPtr_l013734 {
    UnkPlayback_l013734* ptr;
    UnkPlaybackDeleter_l013734 deleter;

    UnkPlaybackPtr_l013734() : ptr(nullptr) {}
    UnkPlaybackPtr_l013734(UnkPlayback_l013734* p, UnkPlaybackPool_l013734* pool) : ptr(p), deleter(pool) {}
    UnkPlaybackPtr_l013734(UnkPlaybackPtr_l013734&& other) : ptr(other.Release()), deleter(std::move(other.deleter)) {}
    ~UnkPlaybackPtr_l013734() { Reset(nullptr); }

    void Reset(UnkPlayback_l013734* p) {
        UnkPlayback_l013734* old = ptr;
        ptr = p;
        if (old)
            deleter(old);
    }
    UnkPlayback_l013734* Release() {
        UnkPlayback_l013734* p = ptr;
        ptr = nullptr;
        return p;
    }
    UnkPlaybackPtr_l013734& operator=(UnkPlaybackPtr_l013734&& other) {
        Reset(other.Release());
        deleter = std::move(other.deleter);
        return *this;
    }
};

struct UnkResourceHolder_l013734 {
    base::reflection::UnkReflObject* resource;
};

static inline UnkCountedResource* AsAnimationResource(base::reflection::UnkReflObject* object) {
    if (object && fn_710009d50c(object->vf_0x0(), fn_710000430c()))
        return reinterpret_cast<UnkCountedResource*>(object);
    return nullptr;
}

// Takes a playback from the pool (while it allows), owned by a pointer that gives it back.
static inline UnkPlaybackPtr_l013734 NewPlayback() {
    UnkPlaybackPool_l013734* pool = &g_unk_7101d4d418;
    void* mem = nullptr;
    if (pool->free_count != 0) {
        pool->free_count--;
        mem = fn_7100083804(reinterpret_cast<UnkVoicePool_b56*>(pool->blocks));
    }
    return UnkPlaybackPtr_l013734(new (mem) UnkPlayback_l013734(), pool);
}

UnkPlaybackPtr_l013734 fn_7100013734(const UnkResourceHolder_l013734* holder) {
    UnkPlaybackPtr_l013734 playback;
    if (holder->resource) {
        playback = NewPlayback();
        playback.ptr->resource = UnkResourceRef_l013734(AsAnimationResource(holder->resource));
        UnkPlayback_l013734* p = playback.ptr;
        f32 length = p->vtable->GetLength(p);
        playback.ptr->step = length / (g_unk_7101647568 * p->rate);
    }
    return playback;
}
