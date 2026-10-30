// Adding a reflected function to a class (fn_7100097550): the keyed array at CClass+0x110
// (UnkReflArray: functions, 0xb8 bytes each, and a parallel array of their names), a
// reflected function's move constructor, and a class attribute read with a default.
//
// The types here are local views of layouts the rest of the code only declares
// (UnkReflArray in CClass.h, UnkReflFunction and UnkReflAttrs in UnkReflAttributes.h); the
// static_asserts tie them together. Names are placeholders.
#include <new>

#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflAttrKey;
using base::reflection::UnkReflAttrs;
using base::reflection::UnkReflFunction;

extern char g_unk_7101a1b460[];  // UnkReflFunction's virtual table (its address point is +0x10)
extern u32 g_unk_7101d5bdd8;     // the default of a class's "_EditorOnly" attribute (nonzero: true)
const UnkReflAttrKey& fn_710000d120();  // "_EditorOnly"

namespace refl_add_function {

// UnkReflArray (CClass.h) with typed elements: values and their names, side by side.
template <class T>
struct UnkKeyedArray {
    T* data;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 field_0x1c;
    base::UnkAllocator* allocator;

    UnkKeyedArray(const UnkKeyedArray& other)
        : data(other.data), keys(other.keys), count(other.count), capacity(other.capacity), step(other.step),
          field_0x1c(other.field_0x1c), allocator(other.allocator) {}

    // The element named `key`, or End().
    T* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &data[i];
        return &data[count];
    }
    T* End() { return &data[count]; }
};
static_assert(sizeof(UnkKeyedArray<int>) == sizeof(base::reflection::UnkReflArray), "UnkReflArray layout");

// base::UnkVector's layout, with the move constructor UnkReflFunction's uses.
template <class T>
struct UnkVectorView {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkVectorView(UnkVectorView&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};
static_assert(sizeof(UnkVectorView<void*>) == sizeof(base::UnkVector<void*>), "UnkVector layout");

// A reflected function (UnkReflFunction) with its whole layout.
struct UnkReflFunctionData {
    // The name: moved, the source gets the pool's empty id.
    struct Name {
        UnkStringData* data;
        Name(Name&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    };
    // The two fields after the keyed array. Their copy is read before the array's source is
    // cleared and written after it (the order the original's move constructor has).
    struct Pair {
        void* field_0x0;
        u32 field_0x8;
        template <class A>
        Pair(const Pair& other, A& moved_array) {
            moved_array.count = 0;
            void* a = other.field_0x0;
            u32 b = other.field_0x8;
            moved_array.data = nullptr;
            moved_array.keys = nullptr;
            field_0x8 = b;
            field_0x0 = a;
        }
    };

    void* vtbl;
    UnkVectorView<const CType*> params;
    Name name;
    const CType* return_type;
    void* field_0x40;
    void* invoker;
    UnkKeyedArray<void> field_0x50;
    Pair field_0x78;
    UnkVectorView<void*> param_attrs;  // 0x38-byte parameter attribute containers
    union {
        u32 field_0xb0_w;
        struct {
            u16 field_0xb0;
            u16 field_0xb2;
        };
    };

    UnkReflFunctionData() asm("fn_710009a5b4");   // fn_710009a5b4
    ~UnkReflFunctionData() asm("fn_710009aae8");  // fn_710009aae8
    UnkReflFunctionData(UnkReflFunctionData&& other)
        : vtbl(g_unk_7101a1b460 + 0x10), params(static_cast<UnkVectorView<const CType*>&&>(other.params)),
          name(static_cast<Name&&>(other.name)), return_type(other.return_type), field_0x40(other.field_0x40),
          invoker(other.invoker), field_0x50(other.field_0x50), field_0x78(other.field_0x78, other.field_0x50),
          param_attrs(static_cast<UnkVectorView<void*>&&>(other.param_attrs)), field_0xb0_w(other.field_0xb0_w) {}

    UnkReflFunction* Public() { return reinterpret_cast<UnkReflFunction*>(this); }
};
static_assert(sizeof(UnkReflFunctionData) == sizeof(UnkReflFunction), "UnkReflFunction layout");
static_assert(__builtin_offsetof(UnkReflFunctionData, return_type) == __builtin_offsetof(UnkReflFunction, return_type),
              "UnkReflFunction layout");
static_assert(__builtin_offsetof(UnkReflFunctionData, field_0xb2) == __builtin_offsetof(UnkReflFunction, field_0xb2),
              "UnkReflFunction layout");

// An attribute value: stored in place, or through `ptr` when flagged (field_0x14 == 1) or
// larger than 8 bytes.
struct UnkReflAttrValue {
    void* ptr;
    const CType* type;
    u32 field_0x10;
    u32 field_0x14;
};

// UnkReflAttrs: values keyed by the attribute key's name.
struct UnkReflAttrsData {
    UnkKeyedArray<UnkReflAttrValue> values;

    // The value of `key` if there is one of type T (or derived), else `def`.
    template <class T>
    const T& Get(const UnkReflAttrKey& key, const T& def) {
        UnkReflAttrValue* v = values.Find(key.name);
        if (v == values.End())
            return def;
        if (!v->type || !fn_710009d50c(v->type, base::reflection::UnkReflTypeOf<T>::Get()))
            return def;
        if (v->field_0x14 == 1 || v->type->m_size >= 9)
            return *static_cast<const T*>(v->ptr);
        return *reinterpret_cast<const T*>(v);
    }
};

}  // namespace refl_add_function

using namespace refl_add_function;

// Grow the keyed array to `capacity`.
void fn_7100097f80(UnkKeyedArray<UnkReflFunctionData>* array, u32 capacity);

// Add a function named `name` to a class; null if the class already has one by that name. A
// class whose "_EditorOnly" attribute is set marks the new function the same way.
UnkReflFunction* fn_7100097550(CClass* cls, const CStrId& name) {
    auto& fns = *reinterpret_cast<UnkKeyedArray<UnkReflFunctionData>*>(&cls->m_array_0x110);
    if (fns.Find(name) != fns.End())
        return nullptr;
    UnkReflFunctionData* fn;
    {
        UnkReflFunctionData init;
        if (fns.count == fns.capacity)
            fn_7100097f80(&fns, fns.step + fns.count);
        // The name's copy: the pointer is stored bytewise (the original reloads the count and
        // the array after it on both paths), then the string is retained.
        CStrId* key = &fns.keys[fns.count];
        UnkStringData* data = name.data;
        __builtin_memcpy(key, &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
        new (&fns.data[fns.count]) UnkReflFunctionData(static_cast<UnkReflFunctionData&&>(init));
        ++fns.count;
        fn = fns.count ? fns.End() - 1 : fns.data;
    }
    // NOTE: the volatile read keeps the default in memory, as the original reads the flag
    // through the selected pointer on every path; the behaviour is the same.
    auto* attrs = reinterpret_cast<UnkReflAttrsData*>(fn_710009dd6c(cls));
    if (*reinterpret_cast<const volatile u8*>(&attrs->Get<bool>(fn_710000d120(), g_unk_7101d5bdd8 != 0)))
        fn->Public()->Attrs()->Add<bool>(fn_710000d120(), true);
    return fn->Public();
}
