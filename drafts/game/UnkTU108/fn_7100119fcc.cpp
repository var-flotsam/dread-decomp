// Unit 108: moves every point of a polygon by an offset, passing each through an optional mapping
// function first, then marks the polygon dirty. The mapping function is held the way a std::function
// holds it (a small buffer, a pointer to the callable that is the buffer when it fits). Names and
// layouts are placeholders.
#include <new>

#include "game/UnkTU55.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern char g_unk_7101a1bdc0[] __attribute__((visibility("hidden")));

struct UnkVec2_108 {
    f32 x;
    f32 y;
};

struct UnkPoint108 {
    f32 x, y, z;
};

// The callable's interface: the slots of a std::function's callable base.
struct UnkCallableBase108 {
    virtual void vf_0x0() = 0;
    virtual void vf_0x8() = 0;
    virtual void vf_0x10() = 0;
    virtual void vf_0x18() = 0;
    virtual void DestroyInPlace() = 0;
    virtual void DestroyAndFree() = 0;
    virtual UnkVec3f Call(const UnkVec3f& v) = 0;
};

struct UnkPointMapper108 {
    u8 pad_0x0[0x20];
    UnkCallableBase108* callable;  // 0x20
};

// What the mapper's lambda holds: the mapper itself.
struct UnkMapperCallable108 {
    char* vtable;
    UnkPointMapper108* mapper;
    explicit UnkMapperCallable108(UnkPointMapper108* m) : vtable(g_unk_7101a1bdc0), mapper(m) {}
};

struct UnkMapFunction108 {
    alignas(8) u8 buffer[0x20];
    UnkCallableBase108* callable;  // 0x20
};

struct UnkPolyPoints108 {
    UnkPoint108* data;
    u32 count;  // 0x8
    u8 pad_0xc[0x1c];
    f32 field_0x28;
    u8 pad_0x2c;
    u8 dirty;  // 0x2d
};

void fn_7100119fcc(UnkPolyPoints108* poly, const UnkVec2_108* offset, UnkPointMapper108* mapper) {
    f32 dx = offset->x;
    f32 dy = offset->y;
    UnkMapFunction108 map;
    map.callable = nullptr;
    if (mapper->callable) {
        new (map.buffer) UnkMapperCallable108(mapper);
        map.callable = reinterpret_cast<UnkCallableBase108*>(map.buffer);
    }
    for (UnkPoint108* p = poly->data; poly->count && p < poly->data + poly->count; ++p) {
        UnkVec3f v(p->x + dx, p->y + dy, 0.0f);
        if (map.callable)
            v = map.callable->Call(v);
        fn_710007b444(p, reinterpret_cast<const UnkVec3*>(&v));
    }
    poly->field_0x28 = poly->field_0x28 + 0.0f;
    poly->dirty = 1;
    if (reinterpret_cast<void*>(map.callable) == map.buffer)
        map.callable->DestroyInPlace();
    else if (map.callable)
        map.callable->DestroyAndFree();
}
