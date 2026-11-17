// Registers the render resource types: each is described by a record built from its type
// (fn_71000e9a7c), given a type number and, for some, an id, file paths and callbacks, then
// handed to the registry and destroyed. Names and layouts are placeholders.
#include <new>

#include "base/UnkResDesc.h"
#include "base/UnkResKey.h"
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTU204.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

using base::global::CFilePathStrId;
using base::global::CStrId;

// An element of the record's list of path ids, constructed in place: a pushed temporary is moved in
// bitwise (the original reloads `count` after storing it), a stored id is copied.
static void Construct(CFilePathStrId* p, CFilePathStrId&& value) {
    __builtin_memcpy(p, &value, sizeof(CFilePathStrId));
    value.data = &g_unk_7101d47200->empty;
}
static void Construct(CFilePathStrId* p, const CFilePathStrId& value) { new (p) CFilePathStrId(value); }

// CRntVector's PushBack (base/global/CRntVector.h) on the record's list, inlined: growth moves the
// elements one by one.
template <class U>
__attribute__((always_inline)) static void PushBack(UnkResPathVec* paths, U&& value) {
    u32 n = paths->count;
    u32 old_capacity = paths->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && paths->step != 0)
        new_capacity = (n / paths->step + 1) * paths->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    CFilePathStrId* p;
    if (old_capacity < new_capacity) {
        p = static_cast<CFilePathStrId*>(
            paths->allocator->vf_0x40((u64)new_capacity * sizeof(CFilePathStrId), 8, paths->flags));
        if (paths->data) {
            CFilePathStrId* src = paths->data;
            CFilePathStrId* dst = p;
            for (u32 i = 0; i < paths->count; ++i, ++src, ++dst) {
                Construct(dst, static_cast<CFilePathStrId&&>(*src));
                src->~CFilePathStrId();
            }
            paths->allocator->vf_0x48(paths->data);
        }
        n = paths->count;
        paths->data = p;
        paths->capacity = new_capacity;
    } else {
        p = paths->data;
    }
    Construct(&p[n], static_cast<U&&>(value));
    ++paths->count;
}

// base::global::CRntVector::operator[].
static const CFilePathStrId& At(const UnkResPathVec& paths, s32 i) {
    if (!((u32)i < paths.count))
        __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
    return paths.data[i];
}

void fn_71000ea29c(UnkResRegistry* registry, UnkResDesc* desc);
void fn_71001b2e44();
void fn_71001b2e4c();
void fn_71001b2e54();

// This file's statics (laid out together by the compiler).
struct UnkResStatics {
    CFilePathStrId field_0x0;
    CFilePathStrId field_0x8;
    UnkResPathVec paths;  // 0x10
    UnkResFactory* factory[1];  // 0x38: used by address only
};
extern UnkResStatics g_unk_716e5543a0 __attribute__((visibility("hidden")));
#define FACTORY reinterpret_cast<UnkResFactory*>(&g_unk_716e5543a0.factory)

// The other registered types (models, textures and the sonar's in base/UnkResKey.h).
extern const UnkResType g_unk_716e453ef8, g_unk_716e454178, g_unk_716e4542f8, g_unk_716e4541f8;

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
        desc.field_0x20 = CFilePathStrId("system/shd/", true);
        desc.field_0x28.context = nullptr;
        desc.field_0x28.function = fn_71001b2e44;
        desc.field_0x38.context = nullptr;
        desc.field_0x38.function = fn_71001b2e4c;
        fn_71000ea29c(registry, &desc);
    }
    {
        fn_71001b122c();
        UnkResDesc desc(&g_unk_716e453f78, FACTORY);
        PushBack(&desc.paths, CFilePathStrId("system/engine/surfaces/", true));
        PushBack(&desc.paths, CFilePathStrId("system/engine/surfaces/instances/", true));
        desc.field_0x88 = CFilePathStrId("imats", true);
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
            PushBack(&desc.paths, At(g_unk_716e5543a0.paths, i));
        desc.field_0x88 = g_unk_716e5543a0.field_0x0;
        desc.type = 0x14;
        desc.name = g_unk_716e4540f8.name;
        desc.field_0x18 = g_unk_716e5543a0.field_0x8;
        desc.field_0x38.context = nullptr;
        desc.field_0x38.function = fn_71001b2e54;
        fn_71000ea29c(registry, &desc);
    }
}
