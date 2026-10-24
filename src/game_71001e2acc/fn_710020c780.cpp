// Two copies of an update that hands the object at 0x78, cast to the class from fn_71001b122c (null
// when it isn't one), to a function of the object at 0x108, then copies that object's settings
// (from 0x28 and 0x38) into this one (0x88 and 0x98) and passes them to vtable slot 0x30:
// fn_710020c780 (fn_71001bdc9c), fn_71001f8750 (fn_71001bcb50).
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkSettings_710020c780 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    bool field_0x34;
    bool field_0x35;

    UnkSettings_710020c780& operator=(const UnkSettings_710020c780& o) {
        field_0x0 = o.field_0x0;
        field_0x4 = o.field_0x4;
        field_0x8 = o.field_0x8;
        field_0xc = o.field_0xc;
        field_0x10 = o.field_0x10;
        field_0x14 = o.field_0x14;
        field_0x18 = o.field_0x18;
        field_0x20 = o.field_0x20;
        field_0x28 = o.field_0x28;
        field_0x2c = o.field_0x2c;
        field_0x30 = o.field_0x30;
        field_0x34 = o.field_0x34;
        field_0x35 = o.field_0x35;
        return *this;
    }
};

struct UnkSource_710020c780 {
    u8 unk_0x0[0x28];
    f32 field_0x28;
    u8 unk_0x2c[0xc];
    UnkSettings_710020c780 settings;
    u8 field_0x70;
    u8 unk_0x71[0x7];
    void* field_0x78;
    u8 unk_0x80[0x2d8];
    u8 field_0x358;
};

struct UnkUpdater_710020c780 {
    void** vtable;
    u8 unk_0x8[0x10];
    u8 field_0x18[0x60];
    UnkReflObject* field_0x78;
    u8 field_0x80[0x8];
    f32 field_0x88;
    u8 unk_0x8c[0xc];
    UnkSettings_710020c780 settings;
    u8 unk_0xd0[0x29];
    bool field_0xf9;
    bool field_0xfa;
    u8 unk_0xfb;
    bool field_0xfc;
    u8 unk_0xfd[0xb];
    UnkSource_710020c780* field_0x108;
    u8 unk_0x110[0x8];
    f32 field_0x118;
    f32 field_0x11c;
    f32 field_0x120;
    f32 field_0x124;

    void vf_0xa8() { reinterpret_cast<void (*)(UnkUpdater_710020c780*)>(vtable[0x15])(this); }
    void vf_0x30(UnkSettings_710020c780* s) {
        reinterpret_cast<void (*)(UnkUpdater_710020c780*, UnkSettings_710020c780*)>(vtable[6])(this, s);
    }
};

void fn_71001bdc9c(UnkSource_710020c780* source, UnkReflObject* obj);
void fn_71001bcb50(UnkSource_710020c780* source, UnkReflObject* obj);

template <void (*Apply)(UnkSource_710020c780*, UnkReflObject*)>
__attribute__((always_inline)) static inline void Update(UnkUpdater_710020c780* self) {
    UnkSource_710020c780* source = self->field_0x108;
    UnkReflObject* obj = self->field_0x78;
    UnkReflObject* cast = nullptr;
    if (obj)
        cast = fn_710009d50c(obj->vf_0x0(), fn_71001b122c()) ? obj : nullptr;
    Apply(source, cast);
    UnkSource_710020c780* s = self->field_0x108;
    self->field_0x88 = s->field_0x28;
    self->settings = s->settings;
    self->vf_0x30(&self->settings);
}

void fn_710020c780(UnkUpdater_710020c780* self) { Update<fn_71001bdc9c>(self); }

void fn_71001f8750(UnkUpdater_710020c780* self) { Update<fn_71001bcb50>(self); }

// The same cast handed to fn_71001bbd6c, then a reset of the value at 0x80 and a lookup of a
// float (fn_71001bc0b4, key unk_7101648bf0): when found, 0xfc records whether it is outside
// +/-0.001 (fn_7100206d8c).
void fn_71001bbd6c(UnkSource_710020c780* source, UnkReflObject* obj);
void fn_710021693c(void* value, f32 f);
bool fn_71001bc0b4(UnkSource_710020c780* source, const void* key, u32 flag, f32* out);
extern u8 g_unk_7101648bf0[] __attribute__((visibility("hidden")));

void fn_7100206d8c(UnkUpdater_710020c780* self) {
    UnkSource_710020c780* source = self->field_0x108;
    UnkReflObject* obj = self->field_0x78;
    UnkReflObject* cast = nullptr;
    if (obj)
        cast = fn_710009d50c(obj->vf_0x0(), fn_71001b122c()) ? obj : nullptr;
    fn_71001bbd6c(source, cast);
    fn_710021693c(self->field_0x80, 0.0f);
    f32 value = 0.0f;
    if (fn_71001bc0b4(self->field_0x108, g_unk_7101648bf0, 1, &value))
        self->field_0xfc = !(value - 0.001f <= 0.0f) | !(value + 0.001f >= 0.0f);
}

// The same cast handed to fn_71001c3474, then the source's object at 0x78 gets the 0x40-byte value
// fn_710007916c builds from (1.5, 1.5, 1.0) and this object's four floats at 0x118
// (fn_71002098a4).
struct UnkVector3_71002098a4 {
    f32 x;
    f32 y;
    f32 z;

    UnkVector3_71002098a4(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
};

struct UnkTransform_71002098a4 {
    u8 data[0x40];
};

void fn_71001c3474(UnkSource_710020c780* source, UnkReflObject* obj);
UnkTransform_71002098a4 fn_710007916c(const UnkVector3_71002098a4& v);
void fn_710019c474(void* target, const UnkTransform_71002098a4* t);
void fn_710019c564(void* target, f32 a, f32 b, f32 c, f32 d);

void fn_71002098a4(UnkUpdater_710020c780* self) {
    UnkSource_710020c780* source = self->field_0x108;
    UnkReflObject* obj = self->field_0x78;
    UnkReflObject* cast = nullptr;
    if (obj)
        cast = fn_710009d50c(obj->vf_0x0(), fn_71001b122c()) ? obj : nullptr;
    fn_71001c3474(source, cast);
    UnkTransform_71002098a4 t = fn_710007916c(UnkVector3_71002098a4(1.5f, 1.5f, 1.0f));
    fn_710019c474(self->field_0x108->field_0x78, &t);
    fn_710019c564(self->field_0x108->field_0x78, self->field_0x118, self->field_0x11c, self->field_0x120,
                  self->field_0x124);
}

// Steps of the source by `dt` while active (0xf9), first flushing the pending state at 0x18 when
// 0xfa is set; then the settings are copied back and the value at 0x80 is told the previous
// first one. fn_71001fb0c4 (fn_71001bfd5c, fn_71001bffc4), fn_710020da1c (fn_71001c2480,
// fn_71001c1030; it also calls vtable slot 0xa8 either way).
void fn_71001bfd5c(UnkSource_710020c780* source, void* state);
void fn_71001bffc4(UnkSource_710020c780* source, f32 dt);
void fn_71001c2480(UnkSource_710020c780* source, void* state);
void fn_71001c1030(UnkSource_710020c780* source, u32 index, f32 dt);

__attribute__((always_inline)) static inline void CopyBack(UnkUpdater_710020c780* self) {
    UnkSource_710020c780* s = self->field_0x108;
    f32 old = self->field_0x88;
    self->field_0x88 = s->field_0x28;
    self->settings = s->settings;
    fn_710021693c(self->field_0x80, old);
}

bool fn_71001fb0c4(UnkUpdater_710020c780* self, f32 dt) {
    if (!self->field_0xf9)
        return false;
    if (self->field_0xfa) {
        self->field_0xfa = false;
        fn_71001bfd5c(self->field_0x108, self->field_0x18);
    }
    fn_71001bffc4(self->field_0x108, dt);
    CopyBack(self);
    return self->field_0xf9;
}

bool fn_710020da1c(UnkUpdater_710020c780* self, f32 dt) {
    if (self->field_0xf9) {
        if (self->field_0xfa) {
            self->field_0xfa = false;
            fn_71001c2480(self->field_0x108, self->field_0x18);
        }
        fn_71001c1030(self->field_0x108, 0, dt);
        CopyBack(self);
    }
    self->vf_0xa8();
    return self->field_0xf9;
}

// The same cast handed to fn_71001c09c4 and the settings copied back, then the active flag (0xf9)
// is stored in the source (0x70 and 0x358) and the value at 0x80 gets the source's
// "BSphereRadius" when fn_71001c32bc finds it (fn_710020dbd8).
void fn_71001c09c4(UnkSource_710020c780* source, UnkReflObject* obj);
bool fn_71001c32bc(UnkSource_710020c780* source, const u64* key, u32 flag, f32* out);

void fn_710020dbd8(UnkUpdater_710020c780* self) {
    UnkSource_710020c780* source = self->field_0x108;
    UnkReflObject* obj = self->field_0x78;
    UnkReflObject* cast = nullptr;
    if (obj)
        cast = fn_710009d50c(obj->vf_0x0(), fn_71001b122c()) ? obj : nullptr;
    fn_71001c09c4(source, cast);
    UnkSource_710020c780* s = self->field_0x108;
    self->field_0x88 = s->field_0x28;
    self->settings = s->settings;
    bool active = self->field_0xf9;
    // Plain byte stores, so the source pointer is reloaded after them.
    *reinterpret_cast<u8*>(&s->field_0x358) = active;
    *reinterpret_cast<u8*>(&s->field_0x70) = active;
    f32 radius = 0.0f;
    UnkSource_710020c780* lookup = self->field_0x108;
    u64 key = fn_7100001570("BSphereRadius", 13);
    if (fn_71001c32bc(lookup, &key, 1, &radius))
        fn_710021693c(self->field_0x80, radius);
}
