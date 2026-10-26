#pragma once

// Getters of guarded function-local statics that are templates in the original: their guard
// and object are weak, so both are reached through the GOT, and the object's destructor is
// registered with __cxa_atexit. G is a tag naming one instantiation (tools/gen_statics.py);
// UnkReflStatic<G> gives its constants.

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTimelineCalls.h"

namespace base::reflection {

template <class G>
struct UnkReflStatic;

// An attribute key (e.g. "Description"): name, value type, a u32 (0xf).
template <class G>
const UnkReflAttrKey& refl_GetAttrKey() {
    static UnkReflAttrKey s_key(base::global::CStrId(UnkReflStatic<G>::name, true), UnkReflStatic<G>::type(),
                                UnkReflStatic<G>::field_0x10);
    return s_key;
}

// The same for keys with a non-zero u16 at +0x14 (0x100 seen).
struct UnkReflAttrKeyFlags : UnkReflAttrKey {
    UnkReflAttrKeyFlags(const base::global::CStrId& name, const CType* type, u32 unk, u16 flags)
        : UnkReflAttrKey(name, type, unk) {
        field_0x14 = flags;
    }
};

template <class G>
const UnkReflAttrKey& refl_GetAttrKeyFlags() {
    static UnkReflAttrKeyFlags s_key(base::global::CStrId(UnkReflStatic<G>::name, true), UnkReflStatic<G>::type(),
                                     UnkReflStatic<G>::field_0x10, UnkReflStatic<G>::field_0x14);
    return s_key;
}

// A string id built once from a literal.
template <class G>
const base::global::CStrId& refl_GetStaticStrId() {
    static base::global::CStrId s_id(UnkReflStatic<G>::name, true);
    return s_id;
}

// A string id copied once from a global one.
template <class G>
const base::global::CStrId& refl_GetStaticStrIdCopy() {
    static base::global::CStrId s_id(UnkReflStatic<G>::source());
    return s_id;
}

// A typed lookup by a static name: find(owner, name) returns an object, kept only if
// is_a(its class, type()) holds (a component or child of type T, by name).
template <class G>
UnkReflObject* refl_FindTyped(void* owner) {
    static base::global::CStrId s_name(UnkReflStatic<G>::name, true);
    UnkReflObject* obj = UnkReflStatic<G>::find(owner, s_name);
    return obj && UnkReflStatic<G>::is_a(obj->vf_0x0(), UnkReflStatic<G>::type()) ? obj : nullptr;
}

// The same with a class whose descriptor has internal linkage elsewhere (reached directly, not
// through the GOT): init() is its getter, called for the initialisation only.
template <class G>
UnkReflObject* refl_FindTypedHidden(void* owner) {
    using S = UnkReflStatic<G>;
    static base::global::CStrId s_name(S::name, true);
    UnkReflObject* obj = S::find(owner, s_name);
    if (!obj)
        return nullptr;
    const CClass* cls = obj->vf_0x0();
    S::init();
    return S::is_a(cls, &S::type) ? obj : nullptr;
}

// A list that items register themselves in (allocation flags 0xf, not the default 0x3e).
struct UnkReflRegistry {
    base::UnkVector<void*> items;

    explicit UnkReflRegistry(u32 flags) : items(flags) {}
    ~UnkReflRegistry();
};

template <class G>
UnkReflRegistry& refl_GetRegistry() {
    static UnkReflRegistry s_registry(UnkReflStatic<G>::flags);
    return s_registry;
}

template <class G>
void refl_Register(void* item) {
    refl_GetRegistry<G>().items.PushBack(item);
}

}  // namespace base::reflection

// refl_Register<UnkRefl_fn_71004fd2cc> (src/reflection_statics/, flags 0xf): adds a reflected
// property object (UnkPropertyEx<> of the TU initialisers, game/UnkTUProperty.h) to its
// registry; the on_init callback of InitPropertyEx.
void fn_71004fd2cc(void* item);

namespace base::reflection {

// What registers itself: its name is virtual slot 0x28.
struct UnkReflRegistryItem {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual const base::global::CStrId& vf_0x28();
};

// A table built from a registry (UnkReflStatic<G>::Registry names its refl_Register): one
// 16-byte value per registered item, constructed from the item and the table's name, and the
// item's name as its key.
struct UnkReflRegistryMap {
    struct Value {
        u8 unk_0x0[0x10];
    };

    bool initialized;
    base::global::CStrId name;
    Value* values;
    base::global::CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
};

// UnkReflStatic<G>::grow(&values, n) resizes both arrays; construct(value, item, name) builds a
// value. The key is copied as raw bytes, then its string referenced: the original reloads
// `count` after the copy on both paths, which only an untyped copy gives.
template <class G>
bool refl_RegistryMapInit(UnkReflRegistryMap* self, const base::global::CStrId& name) {
    using S = UnkReflStatic<G>;
    self->name = name;
    UnkReflRegistry& registry = refl_GetRegistry<typename S::Registry>();
    for (void **p = registry.items.data, **e = p + registry.items.count; p != e; ++p) {
        UnkReflRegistryItem* item = static_cast<UnkReflRegistryItem*>(*p);
        const base::global::CStrId& key = item->vf_0x28();
        if (self->count == self->capacity)
            S::grow(&self->values, self->count + self->step);
        base::global::CStrId* k = &self->keys[self->count];
        __builtin_memcpy(k, &key, sizeof(*k));
        if (&g_unk_7101d47108->empty != k->data)
            ++k->data->ref_count;
        S::construct(&self->values[self->count], item, name);
        ++self->count;
    }
    self->initialized = true;
    return true;
}

// A u32 id per type T, set once from another weak static: a zeroed word read back as a byte
// and inverted. Copies across many TUs share one pair of statics.
struct UnkReflZeroWord {
    u32 value;
    UnkReflZeroWord() : value(0) {}
};

template <class T>
struct UnkReflTypeId {
    static UnkReflZeroWord& base() {
        static UnkReflZeroWord s_base;
        return s_base;
    }
    static u32& id() {
        static u32 s_id = (u8)~*reinterpret_cast<u8*>(&base());
        return s_id;
    }
};

struct UnkReflTypeIdTag;

struct UnkReflTypeIdArg {
    u32 id;
    u32 unk_0x4;
};

}  // namespace base::reflection

bool fn_7101176690(void* target, u64 hash, u64 kind, base::reflection::UnkReflTypeIdArg* value, const void* a,
                   const void* b, base::reflection::UnkReflTypeIdArg* id);

namespace base::reflection {

// Sends a u32 (or the bits of another 4-byte value) under a hash, with the value's kind hash and
// two descriptors; the type id goes along. Game code inlines it (tools/gen_statics.py parses the
// callers).
inline bool refl_EmitU32(void* target, u64 hash, u64 kind, u32 v, const void* a, const void* b) {
    UnkReflTypeIdArg value;
    value.id = v;
    UnkReflTypeIdArg arg;
    arg.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
    return fn_7101176690(target, hash, kind, &value, a, b, &arg);
}

// Sends the type id to the object at +0x2e8 of a context (at UnkReflStatic<G>::offset of
// `self`), under UnkReflStatic<G>::key().
template <class G>
void refl_SendTypeId(void* self) {
    char* ctx = *reinterpret_cast<char**>(static_cast<char*>(self) + UnkReflStatic<G>::offset);
    void* target = *reinterpret_cast<void**>(ctx + 0x2e8);
    if (target) {
        UnkReflTypeIdArg arg;
        arg.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
        fn_7100a2cb7c(static_cast<UnkTimelineComponent*>(target), static_cast<const u64*>(UnkReflStatic<G>::key()),
                      &arg.id, false, -1.0f);
    }
}

// A large object built once (fn_7100031894); getters return one of its parts.
struct UnkReflSingleton {
    UnkReflSingleton();  // fn_7100031894
    ~UnkReflSingleton();
};

inline UnkReflSingleton& refl_GetSingleton() {
    static UnkReflSingleton s_singleton;
    return s_singleton;
}

template <class G>
void* refl_GetSingletonPart() {
    return reinterpret_cast<char*>(&refl_GetSingleton()) + UnkReflStatic<G>::offset;
}

}  // namespace base::reflection
