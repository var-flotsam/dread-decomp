// Unit 1120 (tools/tu_map.py, no reflected class): the update. After the base step it restarts a finished
// point list or frame list, then moves the path query at +0x150 toward the current target and, each time it
// arrives, advances to the next point (or frame) until the list ends. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU1124.h"
#include "types.h"

using base::math::CVector3D;

struct UnkSelf_np41;
struct UnkSelf_nq41;
void fn_710070699c(UnkSelf_np41* self, const CVector3D* target);
void fn_71007071e0(UnkSelf_nq41* self);

template <class T>
struct UnkOpt_l7011fc {
    T value;
    bool has;
};

// Two plane points the query keeps beside its target.
struct UnkSpan_l7011fc {
    base::math::CVector2D from;
    base::math::CVector2D to;
};

// The path query embedded at +0x150; only what this update reads is named.
struct UnkPathQuery_l7011fc {
    u8 pad_0x0[0x37fdc];
    bool moved;  // 0x37fdc
    u8 pad_0x37fdd[3];
    UnkSpan_l7011fc field_0x37fe0;
    u8 pad_0x37ff0[0x50];
};
static_assert(sizeof(UnkPathQuery_l7011fc) == 0x38040, "layout");

void fn_71007077c8(UnkPathQuery_l7011fc* query, f32 dt, f32 scale, f32 speed, bool flag, bool update);

// A point on a flat list: its position in the plane.
struct UnkPlanePos_l7011fc {
    f32 x, y;
    UnkPlanePos_l7011fc(const UnkPlanePos_l7011fc& o) : x(o.x), y(o.y) {}
};

struct UnkPoint_l7011fc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkPlanePos_l7011fc GetPosition();  // 0x10
};

struct UnkPointList_l7011fc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u32 GetCount();                     // 0x18
    virtual UnkPoint_l7011fc* GetPoint(u32 i);  // 0x20
};

// A frame source: its frame lists, their lengths and each frame's position.
struct UnkFrameSource_l7011fc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* GetPosition();              // 0x18
    virtual void vf_0x20();
    virtual u32 GetCount();                              // 0x28
    virtual UnkFrameSource_l7011fc* GetChild(u32 index);  // 0x30
};

// A callback with its context, cleared once it has run.
struct UnkCallback_l701560 {
    void* ctx;
    void (*fn)(void* ctx, void* arg);

    bool IsSet() const { return fn != nullptr; }
    void Invoke(void* arg) { fn(ctx, arg); }
    void Clear() {
        ctx = nullptr;
        fn = nullptr;
    }
};

// Run after each step with the index of the point just left; false stops the walk.
struct UnkStepCallback_l7011fc {
    void* ctx;
    bool (*fn)(void* ctx, void* arg, u32 index);

    bool IsSet() const { return fn != nullptr; }
    bool Invoke(void* arg, u32 index) { return fn(ctx, arg, index); }
    void Clear() {
        ctx = nullptr;
        fn = nullptr;
    }
};

struct UnkMover_l7011fc {
    u8 pad_0x0[0x78];
    f32 speed;
};

struct UnkUnit1120Object_l701560;
struct UnkFrameSource_l701560;
bool fn_7100701560(UnkUnit1120Object_l701560* self, UnkFrameSource_l701560* source);

struct UnkUnit1120Object_l7011fc {
    u8 pad_0x0[0x30];
    void* field_0x30;
    CVector3D target;  // 0x38
    u8 pad_0x44[0x14];
    UnkCallback_l701560 finished;  // 0x58
    UnkMover_l7011fc* mover;       // 0x68
    u8 pad_0x70[0x58];
    UnkOpt_l7011fc<f32> scale;                     // 0xc8
    u8 pad_0xd0[0x18];
    UnkOpt_l7011fc<UnkPointList_l7011fc*> points;  // 0xe8
    UnkOpt_l7011fc<UnkFrameSource_l7011fc*> frames;  // 0xf8
    u8 pad_0x108;
    UnkOpt_l7011fc<bool> flag;  // 0x109
    u8 pad_0x10b[5];
    UnkOpt_l7011fc<f32> speed;  // 0x110
    u8 pad_0x118[0x38];
    UnkPathQuery_l7011fc query;  // 0x150
    u32 frame;                   // 0x38190
    u8 pad_0x38194[4];
    UnkSpan_l7011fc field_0x38198;
    UnkStepCallback_l7011fc stepped;  // 0x381a8
};
static_assert(__builtin_offsetof(UnkUnit1120Object_l7011fc, flag) == 0x109, "layout");
static_assert(__builtin_offsetof(UnkUnit1120Object_l7011fc, speed) == 0x110, "layout");
static_assert(__builtin_offsetof(UnkUnit1120Object_l7011fc, query) == 0x150, "layout");
static_assert(__builtin_offsetof(UnkUnit1120Object_l7011fc, frame) == 0x38190, "layout");
static_assert(__builtin_offsetof(UnkUnit1120Object_l7011fc, stepped) == 0x381a8, "layout");

static inline u32 FrameCount(UnkFrameSource_l7011fc* source) {
    return source->GetChild(0)->GetCount();
}

static inline void SetTarget(UnkUnit1120Object_l7011fc* self, UnkPoint_l7011fc* point) {
    UnkPlanePos_l7011fc pos = point->GetPosition();
    self->target.x = pos.x;
    self->target.y = pos.y;
    self->target.z = 0.0f;
}

void fn_71007011fc(UnkUnit1120Object_l7011fc* self, f32 dt) {
    fn_71007038c0(reinterpret_cast<UnkTU1123_b257c*>(self));
    UnkPointList_l7011fc* points = self->points.has ? self->points.value : nullptr;
    UnkFrameSource_l7011fc* frames = self->frames.has ? self->frames.value : nullptr;
    bool hasPoints = points && points->GetCount() != 0;
    bool hasFrames = frames && FrameCount(frames) != 0;
    if (!hasPoints && !hasFrames)
        return;

    if (points && self->frame >= points->GetCount()) {
        self->frame = 0;
        SetTarget(self, points->GetPoint(0));
    }
    if (frames && self->frame >= FrameCount(frames)) {
        self->frame = 0;
        const CVector3D* pos = frames->GetChild(0)->GetChild(self->frame)->GetPosition();
        self->target.x = pos->x;
        self->target.y = pos->y;
        self->target.z = pos->z;
    }

    f32 speed = self->speed.has ? self->speed.value : self->mover->speed;
    bool flag = self->flag.has ? self->flag.value : false;
    f32 scale = self->scale.has ? self->scale.value : 1.0f;
    for (;;) {
        fn_710070699c(reinterpret_cast<UnkSelf_np41*>(&self->query), &self->target);
        self->query.field_0x37fe0.from = self->field_0x38198.from;
        self->query.field_0x37fe0.to = self->field_0x38198.to;
        fn_71007077c8(&self->query, dt, scale, speed, flag, true);
        if (!self->query.moved)
            return;
        if (!points) {
            if (!frames || !fn_7100701560(reinterpret_cast<UnkUnit1120Object_l701560*>(self),
                                          reinterpret_cast<UnkFrameSource_l701560*>(frames)))
                return;
            continue;
        }
        u32 next = ++self->frame;
        if (next >= points->GetCount()) {
            UnkCallback_l701560& finished = self->finished;
            if (finished.IsSet()) {
                finished.Invoke(self->field_0x30);
                finished.Clear();
            }
            return;
        }
        SetTarget(self, points->GetPoint(self->frame));
        fn_71007071e0(reinterpret_cast<UnkSelf_nq41*>(&self->query));
        UnkStepCallback_l7011fc& stepped = self->stepped;
        if (stepped.IsSet()) {
            bool keepGoing = stepped.Invoke(self->field_0x30, self->frame - 1);
            stepped.Clear();
            if (!keepGoing)
                return;
        }
    }
}
