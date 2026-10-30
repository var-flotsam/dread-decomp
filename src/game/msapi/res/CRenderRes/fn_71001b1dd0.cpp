// Registers the render resource types: each is described by a record built from its type
// (fn_71000e9a7c), given a type number and, for some, an id, file paths and callbacks, then
// handed to the registry and destroyed. Names and layouts are placeholders.
#include <new>

#include "base/UnkResKey.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A file-path id held raw (the record's constructor and destructor are out of line), with the
// assignments and the move and copy constructors CFilePathStrId inlines.
struct UnkPathId {
    UnkStringData* data;

    struct NoInit {};
    UnkPathId(NoInit) {}
    UnkPathId(UnkPathId&& other) {
        data = other.data;
        other.data = &g_unk_7101d47200->empty;
    }
    UnkPathId(const UnkPathId& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkPathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
    UnkPathId& operator=(UnkPathId&& other) {
        this->~UnkPathId();
        new (this) UnkPathId(static_cast<UnkPathId&&>(other));
        return *this;
    }
    UnkPathId& operator=(const UnkPathId& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

// A path id built from a string: CFilePathStrId's constructor (fn_71000166c8, base/global/CFilePathStrId.h)
// run on this storage.
struct UnkNewPathId : UnkPathId {
    UnkNewPathId(const char* str, bool flag) : UnkPathId(UnkPathId::NoInit()) {
        new (static_cast<UnkPathId*>(this)) base::global::CFilePathStrId(str, flag);
    }
};

// The engine's vector of path ids, whose growth moves the elements one by one.
struct UnkPathVec {
    UnkPathId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // The pushed element is moved in bitwise (the original reloads `count` after storing it).
    static void Construct(UnkPathId* p, UnkPathId&& value) {
        __builtin_memcpy(p, &value, sizeof(UnkPathId));
        value.data = &g_unk_7101d47200->empty;
    }
    static void Construct(UnkPathId* p, const UnkPathId& value) { new (p) UnkPathId(value); }

    template <class U>
    __attribute__((always_inline)) void PushBack(U&& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        UnkPathId* p;
        if (old_capacity < new_capacity) {
            p = static_cast<UnkPathId*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkPathId), 8, flags));
            if (data) {
                UnkPathId* src = data;
                UnkPathId* dst = p;
                for (u32 i = 0; i < count; ++i, ++src, ++dst) {
                    Construct(dst, static_cast<UnkPathId&&>(*src));
                    src->~UnkPathId();
                }
                allocator->vf_0x48(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        Construct(&p[n], static_cast<U&&>(value));
        ++count;
    }

    // base::global::CRntVector::operator[].
    const UnkPathId& operator[](s32 i) const {
        if (!((u32)i < count))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return data[i];
    }
};

struct UnkResFactory;

// The record a type is registered with.
struct UnkResDesc {
    u8 unk_0x0[0x10];
    CStrId name;              // 0x10
    UnkPathId field_0x18;
    UnkPathId field_0x20;
    void* field_0x28;
    void (*field_0x30)();
    void* field_0x38;
    void (*field_0x40)();
    u8 unk_0x48[0x8];
    u32 type;                 // 0x50
    u8 unk_0x54[0xc];
    UnkPathVec paths;         // 0x60
    UnkPathId field_0x88;

    UnkResDesc(const UnkResType* type, UnkResFactory* factory) asm("fn_71000e9a7c");
    ~UnkResDesc() asm("fn_71000e8dd0");
};
static_assert(sizeof(UnkResDesc) == 0x90, "");

void fn_71000ea29c(UnkResRegistry* registry, UnkResDesc* desc);
UnkPathId fn_7100169c74(const CStrId& name);
void fn_71001b2e44();
void fn_71001b2e4c();
void fn_71001b2e54();

// This file's statics (laid out together by the compiler).
struct UnkResStatics {
    UnkPathId field_0x0;
    UnkPathId field_0x8;
    UnkPathVec paths;  // 0x10
    UnkResFactory* factory[1];  // 0x38: used by address only
};
extern UnkResStatics g_unk_716e5543a0 __attribute__((visibility("hidden")));
#define FACTORY reinterpret_cast<UnkResFactory*>(&g_unk_716e5543a0.factory)

// The other registered types (models, textures and the sonar's in base/UnkResKey.h).
extern const UnkResType g_unk_716e453ef8, g_unk_716e454178, g_unk_716e4542f8, g_unk_716e4541f8,
    g_unk_716e454278;

// NON_MATCHING(minor): in the last type's copy loop the original keeps the vector's count in a
// register across iterations (one load before the loop); ours reloads it with the capacity at the
// top of each iteration (one instruction more there, one fewer before the loop).
void fn_71001b1dd0() {
    UnkResRegistry* registry = g_unk_7101d472f8;
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e453ff8, FACTORY);
        desc.type = 0x1e;
        fn_71000ea29c(registry, &desc);
    }
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e454078, FACTORY);
        desc.type = 0x1a;
        fn_71000ea29c(registry, &desc);
    }
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e453ef8, FACTORY);
        desc.type = 0x2f;
        desc.name = g_unk_716e453ef8.name;
        desc.field_0x18 = fn_7100169c74(CStrId("error", true));
        desc.field_0x20 = UnkNewPathId("system/shd/", true);
        desc.field_0x28 = nullptr;
        desc.field_0x30 = fn_71001b2e44;
        desc.field_0x38 = nullptr;
        desc.field_0x40 = fn_71001b2e4c;
        fn_71000ea29c(registry, &desc);
    }
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e453f78, FACTORY);
        desc.paths.PushBack(UnkNewPathId("system/engine/surfaces/", true));
        desc.paths.PushBack(UnkNewPathId("system/engine/surfaces/instances/", true));
        desc.field_0x88 = UnkNewPathId("imats", true);
        desc.type = 0x35;
        fn_71000ea29c(registry, &desc);
    }
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e454178, FACTORY);
        desc.type = 0x2b;
        fn_71000ea29c(registry, &desc);
    }
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e4542f8, FACTORY);
        desc.type = 0x33;
        fn_71000ea29c(registry, &desc);
    }
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e4541f8, FACTORY);
        desc.type = 0xc;
        fn_71000ea29c(registry, &desc);
    }
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e454278, FACTORY);
        desc.type = 0xc;
        fn_71000ea29c(registry, &desc);
    }
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e4540f8, FACTORY);
        s32 n = g_unk_716e5543a0.paths.count;
        for (s32 i = 0; i < n; i++)
            desc.paths.PushBack(g_unk_716e5543a0.paths[i]);
        desc.field_0x88 = g_unk_716e5543a0.field_0x0;
        desc.type = 0x14;
        desc.name = g_unk_716e4540f8.name;
        desc.field_0x18 = g_unk_716e5543a0.field_0x8;
        desc.field_0x38 = nullptr;
        desc.field_0x40 = fn_71001b2e54;
        fn_71000ea29c(registry, &desc);
    }
}
