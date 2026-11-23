// CStandaloneFXComponent's unit: fn_710081588c moves an effect instance (0x40 bytes) in at an index of the
// component's instance array, moving the later instances up one place; at the end it appends (fn_7100815a20).
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CStandaloneFXComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkFxScale_ovb0103 {
    f32 x, y, z;
    UnkFxScale_ovb0103(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkFxScale_ovb0103(const UnkFxScale_ovb0103& o) : x(o.x), y(o.y), z(o.z) {}
    f32& At(s32 i) { return (&x)[i]; }
};

struct UnkFxHandle_ovb0103;
struct UnkFxDesc_ovb0103;

// A file-path id (CFilePathStrId's pool) released in two steps, as base::UnkStrIdMemberOf.
struct UnkFxPath_ovb0103 {
    UnkStringData* data;

    UnkFxPath_ovb0103() {
        data = nullptr;
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
    UnkFxPath_ovb0103(UnkFxPath_ovb0103&& other) : data(other.data) { other.data = &g_unk_7101d47200->empty; }
    UnkFxPath_ovb0103& operator=(UnkFxPath_ovb0103&& other) {
        UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &g_unk_7101d47200->empty;
        return *this;
    }
    ~UnkFxPath_ovb0103() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
    const CFilePathStrId& Id() const { return reinterpret_cast<const CFilePathStrId&>(*this); }
};

struct UnkFxVec3_ovb0103 {
    f32 x, y, z;
    UnkFxVec3_ovb0103(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkFxVec3_ovb0103(const UnkFxVec3_ovb0103& o) : x(o.x), y(o.y), z(o.z) {}
};


struct UnkFxInstance_ovb0103 {
    UnkFxDesc_ovb0103* desc;  // 0x0
    UnkFxPath_ovb0103 path;  // 0x8
    u32 field_0x10;
    UnkFxVec3_ovb0103 offset;  // 0x14
    u32 field_0x20;
    u32 field_0x24;
    UnkFxScale_ovb0103 scale;  // 0x28
    UnkFxHandle_ovb0103* handle;  // 0x38

    explicit UnkFxInstance_ovb0103(UnkFxDesc_ovb0103* desc_)
        : desc(desc_), path(), field_0x10(0), offset(0.0f, 0.0f, 0.0f), field_0x20(0), field_0x24(0),
          scale(1.0f, 1.0f, 1.0f), handle(nullptr) {}
    UnkFxInstance_ovb0103(UnkFxInstance_ovb0103&& o) = default;
    ~UnkFxInstance_ovb0103();

    UnkFxInstance_ovb0103& operator=(UnkFxInstance_ovb0103&& o) {
        desc = o.desc;
        path = static_cast<UnkFxPath_ovb0103&&>(o.path);
        field_0x10 = o.field_0x10;
        offset.x = o.offset.x;
        offset.y = o.offset.y;
        offset.z = o.offset.z;
        field_0x20 = o.field_0x20;
        field_0x24 = o.field_0x24;
        scale.x = o.scale.x;
        scale.y = o.scale.y;
        scale.z = o.scale.z;
        handle = o.handle;
        return *this;
    }
};
static_assert(sizeof(UnkFxInstance_ovb0103) == 0x40, "UnkFxInstance_ovb0103 layout");

struct UnkFxVector_ovb0103;
void fn_7100815758(UnkFxVector_ovb0103* self, u32 n);
void fn_7100815a20(UnkFxVector_ovb0103* self, UnkFxInstance_ovb0103* value);

struct UnkFxVector_ovb0103 {
    UnkFxInstance_ovb0103* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710081588c(UnkFxVector_ovb0103* self, UnkFxInstance_ovb0103* value, u32 index) {
    if (index == self->count) {
        fn_7100815a20(self, value);
        return;
    }
    u32 n = self->count;
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    fn_7100815758(self, new_capacity);
    for (u32 i = self->count - 1; i >= index;) {
        UnkFxInstance_ovb0103* e = &self->data[i];
        new (&e[1]) UnkFxInstance_ovb0103(static_cast<UnkFxInstance_ovb0103&&>(*e));
        fn_710081348c(reinterpret_cast<UnkStruct_710081348c*>(&self->data[i]));
        if (i-- == 0)
            break;
    }
    new (&self->data[index]) UnkFxInstance_ovb0103(static_cast<UnkFxInstance_ovb0103&&>(*value));
    ++self->count;
}
