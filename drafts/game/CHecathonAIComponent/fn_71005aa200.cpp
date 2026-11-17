// CHecathonAIComponent's unit: after fn_710110ca7c, sets a flag and points the direction at +0x14788
// along the path's end nearest the owner (first two points or last two), normalized.
// Names and layouts are placeholders.
#include <math.h>

#include "types.h"

void fn_710110ca7c(void* self);

struct UnkHecathonVec_l5aa200 {
    f32 x;
    f32 y;
    f32 z;

    f32 LengthSq() const { return x * x + y * y + z * z; }
};

static inline UnkHecathonVec_l5aa200 operator-(const UnkHecathonVec_l5aa200& a, const UnkHecathonVec_l5aa200& b) {
    UnkHecathonVec_l5aa200 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

struct UnkHecathonPoint_l5aa200 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkHecathonVec_l5aa200* GetPos();
};

struct UnkHecathonPath_l5aa200 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual s32 GetCount();
    virtual UnkHecathonPoint_l5aa200* GetPoint(s32 index);
};

struct UnkHecathonPathSet_l5aa200 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual UnkHecathonPath_l5aa200* GetPath(s32 index);
};

struct UnkHecathonPathRes_l5aa200 {
    u8 unk_0x0[0x30];
    UnkHecathonPathSet_l5aa200 paths;
};

struct UnkHecathonPathHandle_l5aa200 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Load();
    s32 loaded;
    UnkHecathonPathRes_l5aa200* res;

    UnkHecathonPathSet_l5aa200* GetPaths() {
        UnkHecathonPathSet_l5aa200* p = &res->paths;
        if (loaded == 0)
            Load();
        return p;
    }
};

struct UnkHecathonMatrix_l5aa200 {
    f32 m[3][4];

    UnkHecathonVec_l5aa200 GetTranslation() const {
        UnkHecathonVec_l5aa200 t;
        t.x = m[0][3];
        t.y = m[1][3];
        t.z = m[2][3];
        return t;
    }
};

struct UnkHecathonOwner_l5aa200 {
    u8 unk_0x0[0x38];
    UnkHecathonMatrix_l5aa200 transform;
};

struct UnkHecathonAIComponent_l5aa200 {
    u8 unk_0x0[0x30];
    UnkHecathonOwner_l5aa200* owner;
    u8 unk_0x38[0x14788 - 0x38];
    UnkHecathonVec_l5aa200 pathDir;
    u8 unk_0x14794[0x147c9 - 0x14794];
    bool field_0x147c9;
    u8 unk_0x147ca[0x147d8 - 0x147ca];
    UnkHecathonPathHandle_l5aa200* pathHandle;
};

void fn_71005aa200(UnkHecathonAIComponent_l5aa200* self) {
    UnkHecathonVec_l5aa200* dir = &self->pathDir;
    fn_710110ca7c(self);
    self->field_0x147c9 = true;

    UnkHecathonPathHandle_l5aa200* handle = self->pathHandle;
    UnkHecathonPathSet_l5aa200* paths = handle ? handle->GetPaths() : nullptr;

    UnkHecathonVec_l5aa200 pos = self->owner->transform.GetTranslation();

    UnkHecathonPoint_l5aa200* first = paths->GetPath(0)->GetPoint(0);
    UnkHecathonPath_l5aa200* path = paths->GetPath(0);
    UnkHecathonPoint_l5aa200* last = path->GetPoint(paths->GetPath(0)->GetCount() - 1);

    f32 firstDist = (pos - *first->GetPos()).LengthSq();
    f32 lastDist = (pos - *last->GetPos()).LengthSq();

    UnkHecathonPath_l5aa200* nearPath = paths->GetPath(0);
    UnkHecathonVec_l5aa200 from;
    UnkHecathonPoint_l5aa200* to;
    if (firstDist < lastDist) {
        from = *nearPath->GetPoint(1)->GetPos();
        to = first;
    } else {
        from = *nearPath->GetPoint(paths->GetPath(0)->GetCount() - 2)->GetPos();
        to = last;
    }

    *dir = *to->GetPos() - from;
    f32 inv = 1.0f / (sqrtf(dir->LengthSq()) + 0.0001f);
    dir->x = dir->x * inv;
    dir->y = inv * dir->y;
    dir->z = inv * dir->z;
}
