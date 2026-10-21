// Dispatches a query against a collision shape by the shape's type, forwarding to the
// type-specific query; one type is handled inline with the query box moved into the shape's space.
#include "types.h"

struct UnkFlags {
    u32 bits;
    UnkFlags() : bits(0) {}
    UnkFlags(const UnkFlags& o) : bits(o.bits) {}
    UnkFlags& operator=(const UnkFlags& o) {
        bits = o.bits;
        return *this;
    }
};

inline const UnkFlags& NoFlags_d56604() {
    static UnkFlags s;
    return s;
}

struct Vec3 {
    f32 x, y, z;
    Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    Vec3(const Vec3& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkMtx34 {
    f32 m[12];
    Vec3 GetTranslation() const { return Vec3{m[3], m[7], m[11]}; }
};

struct UnkQueryBox {
    f32 minX, minY, maxX, maxY;
    u32 field_0x10;
    u32 field_0x14;
    UnkQueryBox(const UnkQueryBox& o)
        : minX(o.minX), minY(o.minY), maxX(o.maxX), maxY(o.maxY), field_0x10(o.field_0x10), field_0x14(o.field_0x14) {}
};

struct UnkShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u32 GetType();
};

struct UnkShape1;
struct UnkShape2;
struct UnkShape4;
struct UnkShape8;
struct UnkShape40;
struct UnkShape10 {
    char pad[0xa8];
    char field_0xa8[8];
};

template <class T, u32 Type>
static inline T* ShapeCast(UnkShape* s) {
    return s->GetType() == Type ? reinterpret_cast<T*>(s) : nullptr;
}

struct UnkLocalQuery {
    virtual void vf_0x0();  // vtable at 0x7101aa8870
    Vec3 offset;
    u16 field_0x14;
    void* field_0x18;
    void* field_0x20;
    UnkLocalQuery(const Vec3& o, void* p, void* a) : offset(o), field_0x14(0), field_0x18(p), field_0x20(a) {}
};

struct UnkWorld;
UnkFlags fn_7100d56cec(UnkWorld* self, const UnkQueryBox* box, UnkShape1* shape, const UnkMtx34* xf, void* arg);
UnkFlags fn_7100d56930(UnkWorld* self, const UnkQueryBox* box, UnkShape2* shape, const UnkMtx34* xf, void* arg, u32 arg2);
UnkFlags fn_7100d56b08(UnkWorld* self, const UnkQueryBox* box, UnkShape4* shape, const UnkMtx34* xf, void* arg, u32 arg2);
UnkFlags fn_7100d56ec8(UnkWorld* self, const UnkQueryBox* box, UnkShape8* shape, const UnkMtx34* xf, void* arg, u32 arg2);
UnkFlags fn_7100d570cc(UnkWorld* self, const UnkQueryBox* box, UnkShape40* shape, const UnkMtx34* xf, void* arg, u32 arg2);
UnkFlags fn_7100d65b70(UnkShape10* shape, const UnkQueryBox* box, UnkLocalQuery* query);

// NON_MATCHING(minor): in the inline type-0x10 case the local box translation, the query object's
// field stores and the vtable load are scheduled in a different order; same instructions otherwise.
UnkFlags fn_7100d56604(UnkWorld* self, const UnkQueryBox* box, UnkShape* shape, const UnkMtx34* xf, void* arg, u32 arg2) {
    switch (shape->GetType()) {
    case 1:
        return fn_7100d56cec(self, box, ShapeCast<UnkShape1, 1>(shape), xf, arg);
    case 2:
        return fn_7100d56930(self, box, ShapeCast<UnkShape2, 2>(shape), xf, arg, arg2);
    case 4:
        return fn_7100d56b08(self, box, ShapeCast<UnkShape4, 4>(shape), xf, arg, arg2);
    case 8:
        return fn_7100d56ec8(self, box, ShapeCast<UnkShape8, 8>(shape), xf, arg, arg2);
    case 0x40:
        return fn_7100d570cc(self, box, ShapeCast<UnkShape40, 0x40>(shape), xf, arg, arg2);
    case 0x10: {
        UnkShape10* s = ShapeCast<UnkShape10, 0x10>(shape);
        UnkFlags result = NoFlags_d56604();
        UnkQueryBox local = *box;
        Vec3 t = xf->GetTranslation();
        local.minX -= t.x;
        local.minY -= t.y;
        local.maxX -= t.x;
        local.maxY -= t.y;
        UnkLocalQuery q(t, s->field_0xa8, arg);
        result = fn_7100d65b70(s, &local, &q);
        return UnkFlags(result);
    }
    }
    return NoFlags_d56604();
}
