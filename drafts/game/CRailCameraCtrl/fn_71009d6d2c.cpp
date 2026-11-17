// CRailCameraCtrl unit (tools/tu_map.py, unit 1497): moves the camera's place on its rail toward the
// requested segment and parameter, walking the segments in between and stopping where the speed-scaled
// distance runs out. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector3D;

struct UnkRailNode_l9d6d2c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* GetPosition();  // 0x18
    u8 pad_0x8[0x10];
    u8 field_0x18[0x8];  // the part fn_71002a76a4 reads
};

struct UnkRailPath_l9d6d2c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual UnkRailNode_l9d6d2c* GetNode(s32 index);  // 0x30
};

struct UnkRail_l9d6d2c {
    u8 pad_0x0[0x10];
    UnkRailPath_l9d6d2c* path;  // 0x10
    u8 pad_0x18[0x20];
    f32 field_0x38;  // speed factor
    f32 field_0x3c;  // speed factor where the segments change part
    f32 field_0x40;  // distance added to the first
};

struct UnkRailCameraCtrl_l9d6d2c {
    u8 pad_0x0[0x2a0];
    UnkRail_l9d6d2c* rail;  // 0x2a0
    s32 index;              // 0x2a8, the segment; negative before the first placement
    f32 t;                  // 0x2ac, the parameter along it
    CVector3D position;     // 0x2b0
};

struct UnkRailPartOwner_l9d6d2c {
    u8 pad_0x0[0x10];
    void* id;
};

// A list of distances, freed when it goes out of scope.
struct UnkRailDistances_l9d6d2c : base::global::CRntVector<f32> {
    UnkRailDistances_l9d6d2c() : CRntVector(0xf) {}
    ~UnkRailDistances_l9d6d2c() { Free(); }
};

static inline bool NearlyEqual_l9d6d2c(f32 a, f32 b) {
    return a + 0.001f >= b && a - 0.001f <= b;
}

static inline f32 Distance_l9d6d2c(const CVector3D& a, const CVector3D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    f32 dz = a.z - b.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

// What the node's part belongs to (null when it has none).
static inline void* PartOwner_l9d6d2c(UnkRailNode_l9d6d2c* node) {
    UnkBreakableTarget* owner = *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(node->field_0x18));
    return owner ? reinterpret_cast<UnkRailPartOwner_l9d6d2c*>(owner)->id : nullptr;
}

static inline void Place_l9d6d2c(UnkRailCameraCtrl_l9d6d2c* self, const CVector3D* pos, f32 t) {
    self->position.x = pos->x;
    self->position.y = pos->y;
    self->position.z = pos->z;
    self->t = t;
}

// Places the camera at `tt` along the segment from p0 to p1.
static inline void PlaceBetween_l9d6d2c(UnkRailCameraCtrl_l9d6d2c* self, const CVector3D& p0, const CVector3D& p1,
                                        f32 tt) {
    self->position.x = p0.x + (p1.x - p0.x) * tt;
    self->position.y = p0.y + (p1.y - p0.y) * tt;
    self->position.z = p0.z + (p1.z - p0.z) * tt;
    self->t = tt;
}

s32 fn_71009d6d2c(UnkRailCameraCtrl_l9d6d2c* self, const CVector3D* pos, s32 index, f32 t, f32 speed) {
    if (self->index < 0 || (self->index == index && NearlyEqual_l9d6d2c(t, self->t))) {
        Place_l9d6d2c(self, pos, t);
        return index;
    }

    s32 step;
    f32 from;
    if (self->index < index || (self->index == index && self->t < t)) {
        step = -1;
        from = 0.0f;
    } else {
        step = 1;
        from = 1.0f;
    }
    f32 scaled = self->rail->field_0x38 * speed;
    f32 remaining = self->rail->field_0x40 + scaled;
    UnkRailDistances_l9d6d2c lengths;
    UnkRailDistances_l9d6d2c weights;
    UnkRailPath_l9d6d2c* path = self->rail->path;
    f32 to = 1.0f - from;
    f32 seg_t = t;
    s32 i = index;

    // Forward from the requested segment to the current one.
    for (;;) {
        s32 current = self->index;
        UnkRailNode_l9d6d2c* node0 = path->GetNode(i);
        UnkRailNode_l9d6d2c* node1 = path->GetNode(i + 1);
        CVector3D p0 = *node0->GetPosition();
        CVector3D p1 = *node1->GetPosition();
        f32 length = Distance_l9d6d2c(p0, p1);
        if (i == current) {
            f32 d = length * fabsf(self->t - seg_t);
            void* owner0 = PartOwner_l9d6d2c(node0);
            void* owner1 = PartOwner_l9d6d2c(node1);
            if (d > remaining && owner0 != owner1) {
                PlaceBetween_l9d6d2c(self, p0, p1, seg_t + remaining / length * (f32)step);
                return i;
            }
            lengths.PushBack(d == length ? d : -d);
            weights.PushBack(owner0 != owner1 ? d : 0.0f);
            break;
        }
        f32 d = fabsf(from - seg_t) * length;
        void* owner0 = PartOwner_l9d6d2c(node0);
        void* owner1 = PartOwner_l9d6d2c(node1);
        if (d > remaining && owner0 != owner1) {
            PlaceBetween_l9d6d2c(self, p0, p1, seg_t + remaining / length * (f32)step);
            return i;
        }
        if (owner0 != owner1)
            remaining -= d;
        lengths.PushBack(d == length ? d : -d);
        weights.PushBack(owner0 != owner1 ? d : 0.0f);
        i += step;
        seg_t = to;
    }

    f32 total = 0.0f;
    for (f32 l : lengths)
        total += fabsf(l);
    f32 changing = 0.0f;
    for (f32 w : weights)
        changing += w;
    if (NearlyEqual_l9d6d2c(total, 0.0f)) {
        Place_l9d6d2c(self, pos, t);
        return index;
    }
    f32 ratio = changing / total;
    f32 budget = powf(scaled, ratio) * powf(self->rail->field_0x3c * speed, 1.0f - ratio);

    // Back from the current segment, spending the budget.
    seg_t = self->t;
    for (u32 k = lengths.count; k != 0; --k) {
        f32 w = weights[k - 1];
        f32 l = lengths[k - 1];
        f32 length;
        if (l < 0.0f) {
            l = -l;
            CVector3D p0 = *path->GetNode(i)->GetPosition();
            CVector3D p1 = *path->GetNode(i + 1)->GetPosition();
            length = Distance_l9d6d2c(p0, p1);
        } else {
            length = l;
        }
        if (w != 0.0f && l > budget) {
            CVector3D p0 = *path->GetNode(i)->GetPosition();
            CVector3D p1 = *path->GetNode(i + 1)->GetPosition();
            f32 tt = seg_t + budget / length * (f32)-step;
            self->position.x = p0.x + tt * (p1.x - p0.x);
            self->position.y = p0.y + tt * (p1.y - p0.y);
            self->position.z = p0.z + tt * (p1.z - p0.z);
            self->t = tt;
            return i;
        }
        budget -= w;
        seg_t = from;
        i -= step;
    }
    Place_l9d6d2c(self, pos, t);
    return index;
}
