// CSluggerSpitAttack's unit: runs the aim query fn_71006c1c04 and, when it succeeds or no aim is held
// yet, keeps its answer (the track, three points and three angles). Names and layouts are placeholders.
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSluggerSpitAttack_b46a2;

struct UnkSluggerRefCounted_l6c0e38 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    s32 ref_count;
};

// A counted reference; assigned by fn_71002a5d1c.
struct UnkSluggerHandle_l6c0e38 {
    UnkSluggerRefCounted_l6c0e38* ptr;

    UnkSluggerHandle_l6c0e38() : ptr(nullptr) {}
    UnkSluggerHandle_l6c0e38& operator=(const UnkSluggerHandle_l6c0e38& o) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&o));
        return *this;
    }
    ~UnkSluggerHandle_l6c0e38() {
        UnkSluggerRefCounted_l6c0e38* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkSluggerAimTrack_l6c0e38 {
    UnkSluggerHandle_l6c0e38 a;
    UnkSluggerHandle_l6c0e38 b;
    s32 field_0x10;
    u32 field_0x14[12];

    UnkSluggerAimTrack_l6c0e38() : field_0x10(-1) {
        for (int i = 0; i < 12; i++)
            field_0x14[i] = 0;
    }
    UnkSluggerAimTrack_l6c0e38& operator=(const UnkSluggerAimTrack_l6c0e38& o) {
        if (&o != this) {
            a = o.a;
            b = o.b;
            field_0x10 = o.field_0x10;
            for (int i = 0; i < 12; i++)
                field_0x14[i] = o.field_0x14[i];
        }
        return *this;
    }
};

struct UnkSluggerAim_l6c0e38 {
    u8 pad_0x0[0x5c];
    UnkVec3f field_0x5c;
    UnkVec3f field_0x68;
    UnkVec3f field_0x74;
    bool field_0x80;
    u8 pad_0x81[7];
    UnkSluggerAimTrack_l6c0e38 field_0x88;
    f32 field_0xd0;
    u8 pad_0xd4[4];
    f32 field_0xd8;
    f32 field_0xdc;
};

void fn_71006c1c04(UnkSluggerAim_l6c0e38* self, bool mode, UnkVec3f* angles, bool* ok,
                   UnkSluggerAimTrack_l6c0e38* track, UnkVec3f* p0, UnkVec3f* p1, f32* f0, f32* f1, UnkVec3f* p2,
                   f32* f2, bool flag);

void fn_71006c0e38(UnkSluggerSpitAttack_b46a2* attack, bool mode, UnkVec3f* angles) {
    auto* self = reinterpret_cast<UnkSluggerAim_l6c0e38*>(attack);
    bool ok = false;
    UnkSluggerAimTrack_l6c0e38 track;
    UnkVec3f p0(0.0f, 0.0f, 0.0f);
    UnkVec3f p1(0.0f, 0.0f, 0.0f);
    f32 f0, f1;
    UnkVec3f p2(0.0f, 0.0f, 0.0f);
    f32 f2;
    fn_71006c1c04(self, mode, angles, &ok, &track, &p0, &p1, &f0, &f1, &p2, &f2, true);
    if (ok || !self->field_0x80) {
        self->field_0x80 = ok;
        self->field_0x88 = track;
        self->field_0x5c = p0;
        self->field_0x68 = p1;
        self->field_0xd8 = f0;
        self->field_0xdc = f1;
        self->field_0x74 = p2;
        self->field_0xd0 = f2;
    }
}
