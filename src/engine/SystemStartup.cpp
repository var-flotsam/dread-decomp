// The system start-up and shutdown that nnMain's main loop (fn_7100132d30) and its shutdown
// (fn_7100132e90) run around the game. fn_71001325d0 initialises the SDK heap over 32 MB of the
// .bss and hands it to the graphics driver with 8 MB of memory, mounts the RomFS as "rom" over a
// cache from the engine's allocator (aborting if the mount fails), opens the preselected user and
// keeps its id, makes a session UUID, and sets up nn::oe (both change notifications, the
// performance configuration, the operation mode) and nn::time. fn_7100132730 undoes it in reverse.
//
// The SDK calls are declared here in the form the imports carry (their mangled names): lib/nnsdk
// lacks several of them and the port's runtime defines the same names (port/runtime/include/nn/).
// Results are taken as s32, which is how they come back in w0.

#include "base/UnkVector.h"
#include "types.h"

namespace nn {
class Result;

namespace account {
struct Uid;
struct UserHandle;
void Initialize();
bool TryOpenPreselectedUser(UserHandle* out);
s32 GetUserId(Uid* out, const UserHandle& user);
void CloseUser(const UserHandle& user);
}  // namespace account

namespace diag::detail {
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line,
                            const Result* result, const char* format, ...);
}  // namespace diag::detail

namespace fs {
void SetAllocator(void* (*allocate)(u64 size), void (*deallocate)(void* pointer, u64 size));
s32 QueryMountRomCacheSize(u64* out_size);
s32 MountRom(const char* name, void* cache, u64 cache_size);
void Unmount(const char* name);
}  // namespace fs

namespace mem {
class StandardAllocator {
public:
    void Initialize(void* address, u64 size);
    void Finalize();
};
}  // namespace mem

namespace oe {
enum PerformanceMode : s32 {};
void Initialize();
void SetOperationModeChangedNotificationEnabled(bool enabled);
void SetPerformanceModeChangedNotificationEnabled(bool enabled);
void SetPerformanceConfiguration(PerformanceMode mode, s32 configuration);
s32 GetOperationMode();
void EnterExitRequestHandlingSection();
void LeaveExitRequestHandlingSection();
}  // namespace oe

namespace time {
void Initialize();
void Finalize();
}  // namespace time

namespace util {
struct Uuid {
    u64 words[2];
};
Uuid GenerateUuid();
}  // namespace util
}  // namespace nn

namespace nv {
void SetGraphicsAllocator(void* (*allocate)(u64 size, u64 alignment, void* user),
                          void (*free)(void* pointer, void* user),
                          void* (*reallocate)(void* pointer, u64 size, void* user), void* user);
void InitializeGraphics(void* memory, u64 size);
void FinalizeGraphics();
}  // namespace nv

// The SDK heap (constructed by the unit's initialiser fn_7100132b80) and what it manages.
extern nn::mem::StandardAllocator g_unk_710ab70000 __attribute__((visibility("hidden")));
extern u8 g_unk_7108b70000[];  // the heap's 32 MB
extern u8 g_unk_710ab71000[];  // the graphics driver's 8 MB
extern u64 g_unk_710b371018;   // the RomFS cache's size
extern void* g_unk_710b371020;  // the RomFS cache
extern nn::account::UserHandle g_unk_710b371040;
extern u64 g_unk_710b371030[2];  // the user id
extern nn::util::Uuid g_unk_710b371058;  // the session id
extern u8 g_unk_710b37106c;
extern s32 g_unk_710b371068;  // the operation mode (refreshed by fn_71001327c0)

// The graphics allocator's callbacks (the heap as `user`) and nn::fs's allocator: this unit's own
// (their addresses are taken PC-relatively, not through the GOT).
void* fn_71001322b0(u64 size, u64 alignment, void* user) __attribute__((visibility("hidden")));
void fn_71001322d0(void* pointer, void* user) __attribute__((visibility("hidden")));
void* fn_71001322e0(void* pointer, u64 size, void* user) __attribute__((visibility("hidden")));
void* fn_7100132b20(u64 size) __attribute__((visibility("hidden")));
void fn_7100132b54(void* pointer, u64 size) __attribute__((visibility("hidden")));

namespace nn::os {
// What QueryMemoryInfo reports; the game reads the first two sizes.
struct MemoryInfo {
    u64 total_available_memory_size;
    u64 total_used_memory_size;
    u8 rest[0x28];
};
void QueryMemoryInfo(MemoryInfo* info);
s32 SetMemoryHeapSize(u64 size);
s32 AllocateMemoryBlock(u64* address, u64 size);
}  // namespace nn::os

// The memory heap's block (reached through the GOT).
extern u64 g_unk_7108b6fed0;

// The SDK calls this before the static initialisers: the memory heap takes everything the
// application has not used (in 2 MB units), and its first 0x4e700000 bytes become the default
// allocator (fn_7100083870, include/base/UnkDefaultAllocator.h).
extern "C" void nninitStartup() {
    nn::os::MemoryInfo info;
    nn::os::QueryMemoryInfo(&info);
    static u64 s_available = info.total_available_memory_size;
    static u64 s_used = info.total_used_memory_size;
    const u64 size = (s_available - s_used) & ~0x1FFFFFull;
    nn::os::SetMemoryHeapSize(size);
    nn::os::AllocateMemoryBlock(&g_unk_7108b6fed0, size);
    fn_7100083870(reinterpret_cast<void*>(g_unk_7108b6fed0), 0x4e700000);
}

void fn_71001325d0() {
    g_unk_710ab70000.Initialize(g_unk_7108b70000, 0x2000000);
    nv::SetGraphicsAllocator(fn_71001322b0, fn_71001322d0, fn_71001322e0, &g_unk_710ab70000);
    nv::InitializeGraphics(g_unk_710ab71000, 0x800000);
    nn::fs::SetAllocator(fn_7100132b20, fn_7100132b54);
    nn::fs::QueryMountRomCacheSize(&g_unk_710b371018);
    void* cache = fn_7100083ba0(g_unk_710b371018, 0x16);
    g_unk_710b371020 = cache;
    if (cache) {
        alignas(8) s32 result = nn::fs::MountRom("rom", cache, g_unk_710b371018);
        if (result != 0)
            nn::diag::detail::AbortImpl("", "", "", 0, reinterpret_cast<const nn::Result*>(&result), "");
    }
    nn::oe::EnterExitRequestHandlingSection();
    nn::account::Initialize();
    if (nn::account::TryOpenPreselectedUser(&g_unk_710b371040))
        nn::account::GetUserId(reinterpret_cast<nn::account::Uid*>(g_unk_710b371030), g_unk_710b371040);
    g_unk_710b371058 = nn::util::GenerateUuid();
    nn::oe::Initialize();
    nn::oe::SetOperationModeChangedNotificationEnabled(true);
    nn::oe::SetPerformanceModeChangedNotificationEnabled(true);
    nn::oe::SetPerformanceConfiguration(nn::oe::PerformanceMode(0), 0x20003);
    g_unk_710b37106c = 0;
    g_unk_710b371068 = nn::oe::GetOperationMode();
    nn::time::Initialize();
}

void fn_7100132730() {
    nn::time::Finalize();
    if (g_unk_710b371030[0] || g_unk_710b371030[1]) {
        nn::account::CloseUser(g_unk_710b371040);
        g_unk_710b371030[0] = 0;
        g_unk_710b371030[1] = 0;
    }
    if (g_unk_710b371058.words[1] || g_unk_710b371058.words[0])
        g_unk_710b371058 = nn::util::Uuid{};
    nv::FinalizeGraphics();
    g_unk_710ab70000.Finalize();
    nn::oe::LeaveExitRequestHandlingSection();
    nn::fs::Unmount("rom");
}

// ---------------------------------------------------------------------------------------------
// The same unit's 2D collision tests (the unit starts with the segment helpers of
// src/game/malloc/fn_7100125c64.cpp and its neighbours; src/game/UnkCollisionContacts.cpp calls them):
// a moving circle against a box and back (fn_710012deb0, fn_710012e5c0), a segment against an
// oriented box (fn_7100127bb0, fn_710012bb00, fn_710012d960), an oriented box's edges against a
// path set (fn_710012c580), and a polygon's corner angles (fn_71001306b0).

#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"

using base::math::CVector2D;
using base::math::CVector3D;

// What the 2D tests fill on a hit (the layout of src/game/malloc/fn_710012be70.cpp's contact): the
// point, the normal of the first shape, the normal of the second, and which of them are set.
struct UnkContact2D {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    f32 param;  // where along the moving shape's path (0x200)
    u32 flags;  // 8 point, 0x10 normal_b, 0x20 normal_a, 0x200 param

    void SetPoint(const CVector3D& p) {
        point = p;
        flags |= 8;
    }
    void SetNormalA(const CVector3D& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = n.z;
        flags |= 0x20;
    }
    void SetNormalB(const CVector3D& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = n.z;
        flags |= 0x10;
    }
    void SetNormalA(const UnkVec2Result& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const UnkVec2Result& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
};

// An axis-aligned box: its corners go (x0, y0), (x0, y1), (x1, y1), (x1, y0).
struct UnkBox2D {
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    UnkVec2Result Corner(s32 index) const asm("fn_710010d02c");
};

struct UnkCircle2D {
    UnkVec2Result center;
    f32 radius;
    UnkCircle2D(const UnkVec2Result& c, f32 r) : center(c), radius(r) {}
};

// A moving point: where it starts, where it ends and the step between them.
struct UnkSweep2D {
    UnkVec2Result start;
    UnkVec2Result end;
    UnkVec2Result delta;
    UnkSweep2D() : start(0.0f, 0.0f), end(0.0f, 0.0f), delta(0.0f, 0.0f) {}
    UnkSweep2D(UnkVec2Result s, UnkVec2Result d)
        : start(s), end(d.x + s.x, d.y + s.y), delta(end.x - start.x, end.y - start.y) {}
    UnkSweep2D(const CVector2D& s, const CVector2D& e)
        : start(s.x, s.y), end(e.x, e.y), delta(end.x - start.x, end.y - start.y) {}
    UnkSweep2D& operator=(const UnkSweep2D& other) asm("fn_71001259f4");
};

// The segment's copy assignment (start, end and the cached length).
UnkSegment2D* fn_7100125b44(UnkSegment2D* self, const UnkSegment2D* other);
// The moving point against a segment; against a circle (`mode` 2 at the corners).
bool fn_710012bcd0(const UnkSweep2D* sweep, const UnkSegment2D* seg, UnkContact2D* out);
bool fn_710012c840(const UnkSweep2D* sweep, const UnkCircle2D* circle, UnkContact2D* out, s32 mode);

static inline CVector2D UnkVec(f32 x, f32 y) {
    CVector2D v;
    v.x = x;
    v.y = y;
    return v;
}

static inline CVector2D operator+(const UnkVec2Result& a, const UnkVec2Result& b) {
    return UnkVec(a.x + b.x, a.y + b.y);
}

static inline CVector2D operator-(const UnkVec2Result& a, const UnkVec2Result& b) {
    return UnkVec(a.x - b.x, a.y - b.y);
}

static inline void UnkAssign(UnkSegment2D* dst, const UnkSegment2D& src) {
    fn_7100125b44(dst, &src);
}

static inline bool UnkHitCircle(const UnkSweep2D* sweep, const UnkCircle2D& c, UnkContact2D* out) {
    return fn_710012c840(sweep, &c, out, 2);
}

// Whether normal_b's direction lies in [lo, hi].
static inline bool UnkAngleIn(const UnkContact2D* out, f32 lo, f32 hi) {
    f32 angle = atan2f(out->normal_b.y, out->normal_b.x);
    return angle >= lo && angle <= hi;
}

static inline f32 UnkSign(f32 v) {
    return (f32)((s32)(v > 0.0f) - (s32)(v < 0.0f));
}

static inline bool UnkHitCorner(UnkContact2D* out, UnkVec2Result hit) {
    out->point.x = hit.x;
    out->point.y = hit.y;
    out->point.z = 0.0f;
    out->flags |= 8;
    f32 nx = out->normal_b.x;
    f32 ny = out->normal_b.y;
    f32 sx;
    f32 sy;
    if (fabsf(nx) > fabsf(ny)) {
        sx = UnkSign(nx);
        sy = 0.0f;
    } else {
        sx = 0.0f;
        sy = UnkSign(ny);
    }
    out->normal_b.x = sx;
    out->normal_b.y = sy;
    out->flags |= 0x10;
    return true;
}

// A circle moving by `delta` against a box: the four edges pushed out by the radius first (left,
// top, right, bottom), then the four corners, each corner only for the quarter of directions it
// faces. On an edge hit the contact's point is moved back onto the edge and normal_a is the
// reverse of normal_b; on a corner hit the point is the corner and normal_b snaps to an axis.
// NON_MATCHING(minor): the entry block loads the radius after the end point; tail merging keeps
// the third edge's hit code instead of the first's; the corner hit folds its two flag updates
// into one `| 0x18` and stores the snapped normal as two separate floats.
bool fn_710012deb0(const UnkCircle2D* circle, const UnkVec2Result* delta, const UnkBox2D* box,
                   UnkContact2D* out) {
    f32 r = circle->radius;
    UnkSweep2D sweep(circle->center, *delta);
    UnkSegment2D edge(box->Corner(0) - UnkVec2Result(r, 0.0f),
                      box->Corner(1) - UnkVec2Result(r, 0.0f));
    if (fn_710012bcd0(&sweep, &edge, out)) {
        out->SetPoint(CVector3D{r + out->point.x, out->point.y, out->point.z});
        out->SetNormalA(CVector3D{-out->normal_b.x, -out->normal_b.y, -out->normal_b.z});
        return true;
    }
    UnkAssign(&edge, UnkSegment2D(UnkVec2Result(0.0f, r) + box->Corner(1),
                                  UnkVec2Result(0.0f, r) + box->Corner(2)));
    if (fn_710012bcd0(&sweep, &edge, out)) {
        out->SetPoint(CVector3D{out->point.x, out->point.y - r, out->point.z});
        out->SetNormalA(CVector3D{-out->normal_b.x, -out->normal_b.y, -out->normal_b.z});
        return true;
    }
    UnkAssign(&edge, UnkSegment2D(UnkVec2Result(r, 0.0f) + box->Corner(2),
                                  UnkVec2Result(r, 0.0f) + box->Corner(3)));
    if (fn_710012bcd0(&sweep, &edge, out)) {
        out->SetPoint(CVector3D{out->point.x - r, out->point.y, out->point.z});
        out->SetNormalA(CVector3D{-out->normal_b.x, -out->normal_b.y, -out->normal_b.z});
        return true;
    }
    UnkAssign(&edge, UnkSegment2D(box->Corner(3) - UnkVec2Result(0.0f, r),
                                  box->Corner(0) - UnkVec2Result(0.0f, r)));
    if (fn_710012bcd0(&sweep, &edge, out)) {
        out->SetPoint(CVector3D{out->point.x, r + out->point.y, out->point.z});
        out->SetNormalA(CVector3D{-out->normal_b.x, -out->normal_b.y, -out->normal_b.z});
        return true;
    }

    const f32 kPi = 3.1415927f;
    const f32 kHalfPi = 1.5707964f;
    if (UnkHitCircle(&sweep, UnkCircle2D(box->Corner(0), r), out)) {
        f32 angle = atan2f(out->normal_b.y, out->normal_b.x);
        if (angle <= -kHalfPi && angle >= -kPi)
            return UnkHitCorner(out, box->Corner(0));
    }
    if (UnkHitCircle(&sweep, UnkCircle2D(box->Corner(3), r), out)) {
        f32 angle = atan2f(out->normal_b.y, out->normal_b.x);
        if (angle <= 0.0f && angle >= -kHalfPi)
            return UnkHitCorner(out, box->Corner(3));
    }
    if (UnkHitCircle(&sweep, UnkCircle2D(box->Corner(2), r), out)) {
        f32 angle = atan2f(out->normal_b.y, out->normal_b.x);
        if (angle <= kHalfPi && angle >= 0.0f)
            return UnkHitCorner(out, box->Corner(2));
    }
    if (UnkHitCircle(&sweep, UnkCircle2D(box->Corner(1), r), out)) {
        f32 angle = atan2f(out->normal_b.y, out->normal_b.x);
        if (angle <= kPi && angle >= kHalfPi)
            return UnkHitCorner(out, box->Corner(1));
    }
    return false;
}

// The 2D tests' tolerance (0.001).
extern const f32 g_unk_7101647774;

// An oriented box: its centre, its two unit axes and its full size along them.
struct UnkOrientedBox2D {
    CVector2D center;
    CVector2D axis_x;
    CVector2D axis_y;
    CVector2D size;
    f32 field_0x20;  // the rotation is the sum of these two angles
    f32 field_0x24;
    // A vector in the box's frame: its dot products with the two axes.
    UnkVec2Result ToLocal(const CVector2D& v) const asm("fn_71001106cc");
    // A vector of the box's frame back in the world's: rotated by the box's angle.
    UnkVec2Result ToWorld(const CVector2D& v) const asm("fn_71001106f8");

    // A segment in the box's frame, as a moving point from its start to its end.
    UnkSweep2D ToLocal(const UnkSegment2D& seg) const {
        UnkSweep2D r;
        f32 mx = (seg.end.x + seg.start.x) * 0.5f;
        f32 my = (seg.end.y + seg.start.y) * 0.5f;
        CVector2D d = UnkVec(seg.end.x - seg.start.x, seg.end.y - seg.start.y);
        CVector2D rel = UnkVec(mx - center.x, my - center.y);
        UnkVec2Result mid = ToLocal(rel);
        UnkVec2Result dir = ToLocal(d);
        f32 hx = dir.x * 0.5f;
        f32 hy = dir.y * 0.5f;
        r = UnkSweep2D(UnkVec(mid.x - hx, mid.y - hy), UnkVec(hx + mid.x, hy + mid.y));
        return r;
    }
    // The same as a segment.
    UnkSegment2D ToLocalSegment(const UnkSegment2D& seg) const;
};

// A list of contacts, filled by the tests that can report several.
struct UnkContactList2D {
    u32 count;
    UnkContact2D items[1];  // the real capacity is unknown

    // Appends a cleared contact and returns its index (the count itself once it reaches ~0).
    u32 Add() {
        u32 i = count;
        if (i != 0xFFFFFFFF) {
            items[i] = UnkContact2D();
            count = i + 1;
        }
        return i;
    }
};

static inline bool UnkNearEps(f32 a, f32 b) {
    f32 lo = a - g_unk_7101647774;
    f32 hi = a + g_unk_7101647774;
    return hi >= b && lo <= b;
}

// The unit vector along (x, y), or (x, y) itself when it is zero.
static inline void UnkNormalize(f32& x, f32& y) {
    f32 len = sqrtf(x * x + y * y);
    if (len != 0.0f) {
        x = x / len;
        y = y / len;
    }
}

// A segment against an oriented box: the segment is taken into the box's frame and clipped
// against its slabs. Each end of the clipped part that lies on the segment adds a contact (the
// point, the direction from the box's centre, the segment's normal and the parameter). With no
// crossing and `inside` set, a segment that starts inside the box gives one contact at its
// middle (the first one, added if the list is empty). Degenerate segments never hit. Returns
// whether the list holds any contact.
// NON_MATCHING(minor): register allocation and scheduling in the degenerate check and the slab
// test (the step computed before the middle), the four containment comparisons tested in another
// order, and the empty list's append in the containment path placed between the two crossings
// instead of at the end.
bool fn_7100127bb0(const UnkOrientedBox2D* box, const UnkSegment2D* seg, UnkContactList2D* list,
                   bool inside) {
    if (UnkNearEps(seg->start.x, seg->end.x) && UnkNearEps(seg->start.y, seg->end.y))
        return false;
    CVector2D d = UnkVec(seg->end.x - seg->start.x, seg->end.y - seg->start.y);
    CVector2D rel = UnkVec((seg->start.x + seg->end.x) * 0.5f - box->center.x,
                           (seg->start.y + seg->end.y) * 0.5f - box->center.y);
    UnkVec2Result mid = box->ToLocal(rel);
    UnkVec2Result dir = box->ToLocal(d);
    f32 hx = box->size.x * 0.5f;
    f32 hy = box->size.y * 0.5f;
    f32 nhx = box->size.x * -0.5f;
    f32 nhy = box->size.y * -0.5f;
    f32 px = mid.x - dir.x * 0.5f;
    f32 py = mid.y - dir.y * 0.5f;
    f32 ix = 1.0f / dir.x;
    f32 iy = 1.0f / dir.y;
    f32 tx0 = ix * (nhx - px);
    f32 tx1 = ix * (hx - px);
    f32 ty0 = iy * (nhy - py);
    f32 ty1 = iy * (hy - py);
    f32 tymin = ty0 < ty1 ? ty0 : ty1;
    f32 tymax = ty0 > ty1 ? ty0 : ty1;
    f32 txmin = tx0 < tx1 ? tx0 : tx1;
    f32 txmax = tx0 > tx1 ? tx0 : tx1;
    f32 tenter = txmin > tymin ? txmin : tymin;
    f32 texit = txmax < tymax ? txmax : tymax;
    if (!(texit < 0.0f || tenter > texit || tenter > 1.0f)) {
        UnkVec2Result n = fn_7100125e78(seg);
        UnkNormalize(n.x, n.y);
        if (tenter >= 0.0f) {
            f32 hitx = tenter * d.x + seg->start.x;
            f32 hity = tenter * d.y + seg->start.y;
            f32 nx = hitx - box->center.x;
            f32 ny = hity - box->center.y;
            UnkNormalize(nx, ny);
            UnkContact2D c = {{hitx, hity, 0.0f}, {nx, ny, 0.0f}, {n.x, n.y, 0.0f}, tenter, 0x238};
            list->items[list->Add()] = c;
        }
        if (texit >= 0.0f && texit <= 1.0f) {
            f32 hitx = texit * d.x + seg->start.x;
            f32 hity = texit * d.y + seg->start.y;
            f32 nx = hitx - box->center.x;
            f32 ny = hity - box->center.y;
            UnkNormalize(nx, ny);
            UnkContact2D c = {{hitx, hity, 0.0f}, {nx, ny, 0.0f}, {n.x, n.y, 0.0f}, texit, 0x238};
            list->items[list->Add()] = c;
        }
    } else {
        if (!inside)
            return false;
        if (px >= nhx && px <= hx && py >= nhy && py <= hy) {
            if (list->count == 0)
                list->Add();
            list->items[0].SetPoint(CVector3D{(seg->start.x + seg->end.x) * 0.5f,
                                              (seg->start.y + seg->end.y) * 0.5f, 0.0f});
        }
    }
    return list->count != 0;
}

// The contact seen from the other shape: normal_a takes normal_b, which is reversed.
static inline void UnkSwapNormals(UnkContact2D* out, const CVector3D& point) {
    out->SetPoint(point);
    out->SetNormalA(out->normal_b);
    out->SetNormalB(CVector3D{-out->normal_b.x, -out->normal_b.y, -out->normal_b.z});
}

// A box moving by `delta` against a circle: fn_710012deb0 with the circle moving the other way.
// On a hit the contact's point is the circle's point that touches the box (moved back by the
// step for a corner), normal_a is the box's normal and normal_b its reverse.
// NON_MATCHING(minor): the centre's y is moved to a float register before its x; the corner hit
// schedules its flag update and the negations in another order, and the last corner shares the
// other corners' hit code instead of keeping its own copy.
bool fn_710012e5c0(const UnkBox2D* box, const UnkVec2Result* delta, const UnkCircle2D* circle,
                   UnkContact2D* out) {
    f32 r = circle->radius;
    UnkSweep2D sweep(circle->center, UnkVec2Result(-delta->x, -delta->y));
    f32 cy = circle->center.y;
    f32 cx = circle->center.x;
    UnkSegment2D edge(box->Corner(0) - UnkVec2Result(r, 0.0f),
                      box->Corner(1) - UnkVec2Result(r, 0.0f));
    if (fn_710012bcd0(&sweep, &edge, out)) {
        UnkSwapNormals(out, CVector3D{r + cx, cy, 0.0f});
        return true;
    }
    UnkAssign(&edge, UnkSegment2D(UnkVec2Result(0.0f, r) + box->Corner(1),
                                  UnkVec2Result(0.0f, r) + box->Corner(2)));
    if (fn_710012bcd0(&sweep, &edge, out)) {
        UnkSwapNormals(out, CVector3D{cx, cy - r, 0.0f});
        return true;
    }
    UnkAssign(&edge, UnkSegment2D(UnkVec2Result(r, 0.0f) + box->Corner(2),
                                  UnkVec2Result(r, 0.0f) + box->Corner(3)));
    if (fn_710012bcd0(&sweep, &edge, out)) {
        UnkSwapNormals(out, CVector3D{cx - r, cy, 0.0f});
        return true;
    }
    UnkAssign(&edge, UnkSegment2D(box->Corner(3) - UnkVec2Result(0.0f, r),
                                  box->Corner(0) - UnkVec2Result(0.0f, r)));
    if (fn_710012bcd0(&sweep, &edge, out)) {
        UnkSwapNormals(out, CVector3D{cx, r + cy, 0.0f});
        return true;
    }

    const f32 kPi = 3.1415927f;
    const f32 kHalfPi = 1.5707964f;
    {
        UnkCircle2D corner(box->Corner(0), r);
        if (fn_710012c840(&sweep, &corner, out, 2) && UnkAngleIn(out, -kPi, -kHalfPi)) {
            f32 ox;
            f32 oy;
            {
                CVector2D p;
                fn_710007b430(&p, &out->point);
                ox = cx - p.x;
                oy = cy - p.y;
            }
            UnkVec2Result c = box->Corner(0);
            CVector3D n = {-out->normal_b.x, -out->normal_b.y, -out->normal_b.z};
            out->SetPoint(CVector3D{ox + c.x, oy + c.y, 0.0f});
            out->SetNormalA(CVector3D{-n.x, -n.y, -n.z});
            out->SetNormalB(n);
            return true;
        }
    }
    {
        UnkCircle2D corner(box->Corner(3), r);
        if (fn_710012c840(&sweep, &corner, out, 2) && UnkAngleIn(out, -kHalfPi, 0.0f)) {
            f32 ox;
            f32 oy;
            {
                CVector2D p;
                fn_710007b430(&p, &out->point);
                ox = cx - p.x;
                oy = cy - p.y;
            }
            UnkVec2Result c = box->Corner(3);
            CVector3D n = {-out->normal_b.x, -out->normal_b.y, -out->normal_b.z};
            out->SetPoint(CVector3D{ox + c.x, oy + c.y, 0.0f});
            out->SetNormalA(CVector3D{-n.x, -n.y, -n.z});
            out->SetNormalB(n);
            return true;
        }
    }
    {
        UnkCircle2D corner(box->Corner(2), r);
        if (fn_710012c840(&sweep, &corner, out, 2) && UnkAngleIn(out, 0.0f, kHalfPi)) {
            f32 ox;
            f32 oy;
            {
                CVector2D p;
                fn_710007b430(&p, &out->point);
                ox = cx - p.x;
                oy = cy - p.y;
            }
            UnkVec2Result c = box->Corner(2);
            CVector3D n = {-out->normal_b.x, -out->normal_b.y, -out->normal_b.z};
            out->SetPoint(CVector3D{ox + c.x, oy + c.y, 0.0f});
            out->SetNormalA(CVector3D{-n.x, -n.y, -n.z});
            out->SetNormalB(n);
            return true;
        }
    }
    {
        UnkCircle2D corner(box->Corner(1), r);
        if (fn_710012c840(&sweep, &corner, out, 2) && UnkAngleIn(out, kHalfPi, kPi)) {
            f32 ox;
            f32 oy;
            {
                CVector2D p;
                fn_710007b430(&p, &out->point);
                ox = cx - p.x;
                oy = cy - p.y;
            }
            UnkVec2Result c = box->Corner(1);
            CVector3D n = {-out->normal_b.x, -out->normal_b.y, -out->normal_b.z};
            out->SetPoint(CVector3D{ox + c.x, oy + c.y, 0.0f});
            out->SetNormalA(CVector3D{-n.x, -n.y, -n.z});
            out->SetNormalB(n);
            return true;
        }
    }
    return false;
}

// A 3D vector's x and y (fn_710007b430), as a temporary.
struct UnkProjected : CVector2D {
    explicit UnkProjected(const CVector3D& v) { fn_710007b430(this, &v); }
};

static inline CVector3D UnkLiftAt(const UnkVec2Result& v, const CVector2D& origin) {
    return CVector3D{v.x + origin.x, v.y + origin.y, 0.0f};
}

// A segment and an aligned box, tested in the box's frame (`mode` 2).
bool fn_710012b7f0(const UnkSweep2D* seg, const UnkBox2D* box, UnkContact2D* out, s32 mode);

// A segment against an oriented box: fn_710012b7f0 in the box's frame, the contact (point and
// both normals) taken back to the world's.
// NON_MATCHING(minor): the arguments of fn_710012b7f0 are set up after the aligned box's four
// products instead of before them (instruction scheduling only).
bool fn_710012bb00(const UnkSegment2D* segment, const UnkOrientedBox2D* box, UnkContact2D* out) {
    UnkSweep2D seg = box->ToLocal(*segment);
    f32 x1 = box->size.x * 0.5f;
    f32 x0 = box->size.x * -0.5f;
    f32 y0 = box->size.y * -0.5f;
    f32 y1 = box->size.y * 0.5f;
    UnkBox2D aabb = {x0, y0, x1, y1};
    bool hit = fn_710012b7f0(&seg, &aabb, out, 2);
    if (out && hit) {
        out->SetPoint(UnkLiftAt(box->ToWorld(UnkProjected(out->point)), box->center));
        out->SetNormalA(box->ToWorld(UnkProjected(out->normal_a)));
        out->SetNormalB(box->ToWorld(UnkProjected(out->normal_b)));
    }
    return hit;
}

// A set of 2D paths (an array of 0x48-byte entries and their count at +8).
struct UnkPathSet2D;

// A segment against every edge of a path set, appending the contacts to the list.
bool fn_710012c3b0(const UnkSegment2D* seg, const UnkPathSet2D* paths, UnkContactList2D* list,
                   bool flag);

// An oriented box's four edges (its corners from the half size scaled by the cosine and sine of
// the box's angle) against a path set; whether any of them hit (all four are tested).
bool fn_710012c580(const UnkOrientedBox2D* box, const UnkPathSet2D* paths, UnkContactList2D* list) {
    f32 hw = box->size.x * 0.5f;
    f32 hh = box->size.y * 0.5f;
    f32 cx = box->center.x;
    f32 cy = box->center.y;
    f32 angle = box->field_0x24 + box->field_0x20;
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    f32 dy = hh * s;
    f32 dx = hw * c;
    CVector2D corners[4] = {
        {cx + dx, cy + dy}, {cx - dx, cy + dy}, {cx - dx, cy - dy}, {cx + dx, cy - dy}};
    UnkSegment2D e0(corners[0], corners[1]);
    UnkSegment2D e1(corners[1], corners[2]);
    UnkSegment2D e2(corners[2], corners[3]);
    UnkSegment2D e3(corners[3], corners[0]);
    bool hit = fn_710012c3b0(&e0, paths, list, false);
    hit |= fn_710012c3b0(&e1, paths, list, false);
    hit |= fn_710012c3b0(&e2, paths, list, false);
    hit |= fn_710012c3b0(&e3, paths, list, false);
    return hit;
}

// See UnkOrientedBox2D::ToLocal; the segment starts cleared (its cached length -1).
inline UnkSegment2D UnkOrientedBox2D::ToLocalSegment(const UnkSegment2D& seg) const {
    UnkSegment2D r;
    r.start.x = 0.0f;
    r.start.y = 0.0f;
    r.end.x = 0.0f;
    r.end.y = 0.0f;
    r.field_0x10 = -1.0f;
    f32 mx = (seg.end.x + seg.start.x) * 0.5f;
    f32 my = (seg.end.y + seg.start.y) * 0.5f;
    CVector2D d = UnkVec(seg.end.x - seg.start.x, seg.end.y - seg.start.y);
    CVector2D rel = UnkVec(mx - center.x, my - center.y);
    UnkVec2Result mid = ToLocal(rel);
    UnkVec2Result dir = ToLocal(d);
    f32 hx = dir.x * 0.5f;
    f32 hy = dir.y * 0.5f;
    UnkAssign(&r, UnkSegment2D(UnkVec(mid.x - hx, mid.y - hy), UnkVec(hx + mid.x, hy + mid.y)));
    return r;
}

// An aligned box moving by `step` against a segment (the contact's param is where along the step).
bool fn_710012ce70(const UnkBox2D* box, const UnkVec2Result& step, const UnkSegment2D* seg,
                   UnkContact2D* out);

static inline CVector3D operator+(const CVector3D& a, const CVector3D& b) {
    return CVector3D{a.x + b.x, a.y + b.y, a.z + b.z};
}

// An oriented box moving by `delta` against a segment: fn_710012ce70 in the box's frame (the
// segment moving the other way), then the contact moved along the step, its normals swapped and
// taken back to the world's frame.
// NON_MATCHING(minor): the local step and its negation take each other's stack slots, and the
// contact's move along the step and the normal swap are scheduled in another order.
bool fn_710012d960(const UnkSegment2D* segment, const CVector2D* delta, const UnkOrientedBox2D* box,
                   UnkContact2D* out) {
    UnkSegment2D seg = box->ToLocalSegment(*segment);
    f32 x1 = box->size.x * 0.5f;
    f32 x0 = box->size.x * -0.5f;
    f32 y0 = box->size.y * -0.5f;
    f32 y1 = box->size.y * 0.5f;
    UnkBox2D aabb = {x0, y0, x1, y1};
    UnkVec2Result step = box->ToLocal(*delta);
    bool hit = fn_710012ce70(&aabb, UnkVec2Result(-step.x, -step.y), &seg, out);
    if (out && hit) {
        f32 t = out->param;
        out->SetPoint(CVector3D{t * step.x, t * step.y, 0.0f} + out->point);
        f32 ax = out->normal_a.x;
        f32 ay = out->normal_a.y;
        f32 az = out->normal_a.z;
        out->normal_a = out->normal_b;
        out->normal_b.x = ax;
        out->normal_b.y = ay;
        out->normal_b.z = az;
        out->SetPoint(UnkLiftAt(box->ToWorld(UnkProjected(out->point)), box->center));
        out->SetNormalA(box->ToWorld(UnkProjected(out->normal_a)));
        out->SetNormalB(box->ToWorld(UnkProjected(out->normal_b)));
    }
    return hit;
}

// A polygon's vertex record: its angle and its position.
struct UnkPolygonVertex {
    u32 field_0x0;
    f32 angle;  // the angle at the vertex, in [0, 2 pi)
    const CVector3D* pos;
};

// A polygon of the same unit: its vertices (fn_7100130ab0, growing the storage on demand) and
// their count at +8.
struct UnkPolygon {
    u8 pad_0x0[0x8];
    u32 count;
};

UnkPolygonVertex* fn_7100130ab0(UnkPolygon* self, u32 index);

static inline CVector3D UnkSub(const CVector3D& a, const CVector3D& b) {
    return CVector3D{a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline f32 UnkDot(const CVector3D& a, const CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 UnkLength(const CVector3D& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Stores at vertex `index` (at p) the angle between its edges to `next` and `prev`, turning the
// way the polygon winds (pi for a straight or degenerate corner).
static inline void UnkSetCornerAngle(UnkPolygon* self, u32 index, const CVector3D* p,
                                     const CVector3D* next, const CVector3D* prev) {
    CVector3D a = UnkSub(*next, *p);
    CVector3D b = UnkSub(*prev, *p);
    f32 cross = a.y * b.x - a.x * b.y;
    f32 c = UnkDot(a, b) / (UnkLength(b) * UnkLength(a));
    f32 angle;
    if (c >= 1.0f)
        angle = 0.0f;
    else if (c <= -1.0f)
        angle = 3.1415927f;
    else
        angle = acosf(c);
    f32 v = angle == 0.0f ? 3.1415927f : angle;
    v = cross < 0.0f ? v : 6.2831855f - v;
    fn_7100130ab0(self, index)->angle = v;
}

// Stores at each vertex the angle between the edges to its neighbours.
// NON_MATCHING(minor): in each of the three copies of the corner computation the vertex
// coordinates are loaded in another order and the edge vectors' components get other registers.
void fn_71001306b0(UnkPolygon* self) {
    u32 n = self->count;
    for (u32 i = 0; i != n; ++i) {
        if (i == 0)
            UnkSetCornerAngle(self, 0, fn_7100130ab0(self, 0)->pos, fn_7100130ab0(self, 1)->pos,
                              fn_7100130ab0(self, n - 1)->pos);
        else if (i == n - 1)
            UnkSetCornerAngle(self, n - 1, fn_7100130ab0(self, i)->pos, fn_7100130ab0(self, 0)->pos,
                              fn_7100130ab0(self, n - 2)->pos);
        else
            UnkSetCornerAngle(self, i, fn_7100130ab0(self, i)->pos, fn_7100130ab0(self, i + 1)->pos,
                              fn_7100130ab0(self, i - 1)->pos);
    }
}

// ---------------------------------------------------------------------------------------------
// The unit's performance switch (src/game_s7/fn_71001587f0.cpp turns it on once the display is
// set up): g_unk_710b37106c keeps whether the boosted configuration (0x20004 instead of the
// normal 0x20003) is active, and the SDK is only called when that changes. The argument is a bool
// in the original (only its low bit counts); the caller declares it s32.
// NON_MATCHING(minor): with the s32 parameter the comparison takes its operands in the other order
// and the configuration is chosen with `cmp` on the masked value instead of `tst w0, #1`.
void fn_7100132a30(s32 value) {
    if (g_unk_710b37106c == (value & 1))
        return;
    g_unk_710b37106c = value & 1;
    nn::oe::SetPerformanceConfiguration(nn::oe::PerformanceMode(0), (value & 1) ? 0x20004 : 0x20003);
}

// ---------------------------------------------------------------------------------------------
// A box against a vertical capsule (fn_7100129ff0) and a circle against one (fn_710012a9a0): the
// capsule's two end circles and its two sides are tested one after the other and their contacts
// merged into the caller's list of four (src/game/UnkCollisionContacts.cpp calls both with its own
// names for these types: UnkShape8Geometry for the capsule, UnkContactBuffer<4> for the list).

#include <float.h>
#include <string.h>

// A capsule standing on the y axis: its centre, the half length of its straight part and its radius.
struct UnkCapsule2D {
    UnkVec2Result center;
    f32 half_height;
    f32 radius;
};

// The names src/game/UnkCollisionContacts.cpp declares the tests with.
struct UnkShape8Geometry;
template <u32 N>
struct UnkContactBuffer;

// The shape tests the passes use (declared as src/game/UnkCollisionContacts.cpp does): a box
// against a circle, against a segment.
bool fn_7100126bf0(const void* b, const UnkCircle2D* a, UnkContactBuffer<8>* out, bool flag);
bool fn_7100128510(const UnkBox2D* a, const void* b, UnkContactBuffer<2>* out, bool flag);
// Whether the point lies inside the capsule.
bool fn_710010f774(const UnkCapsule2D* capsule, const CVector2D* point);

// The contacts of one test: up to N (eight against a circle, two against a segment). Reading an
// element past the count first clears the new ones and takes the count up to it.
template <u32 N>
struct UnkContactArray2D {
    u32 count;
    UnkContact2D items[N];

    __attribute__((always_inline)) UnkContact2D& At(u64 index) {
        if (count <= index) {
            u32 n = index + 1;
            memset(&items[count], 0, (u64)(n - count) * sizeof(UnkContact2D));
            count = n;
        }
        return items[index];
    }
};

// Whether the list has a contact at the height `y` (within 0.001).
static inline bool UnkHasContactAt(const UnkContactList2D* list, f32 y) {
    bool found = false;
    for (u32 i = 0; i < list->count && !found; ++i)
        found = fabsf(list->items[i].point.y - y) < 0.001f;
    return found;
}

// A contact copied member by member.
static inline void UnkCopyContact(UnkContact2D* dst, const UnkContact2D& src) {
    dst->point.x = src.point.x;
    dst->point.y = src.point.y;
    dst->point.z = src.point.z;
    dst->normal_a.x = src.normal_a.x;
    dst->normal_a.y = src.normal_a.y;
    dst->normal_a.z = src.normal_a.z;
    dst->normal_b.x = src.normal_b.x;
    dst->normal_b.y = src.normal_b.y;
    dst->normal_b.z = src.normal_b.z;
    dst->param = src.param;
    dst->flags = src.flags;
}

__attribute__((always_inline)) static inline void UnkMergeContact(UnkContactList2D* out, const UnkContact2D& c) {
    if (!UnkHasContactAt(out, c.point.y))
        UnkCopyContact(&out->items[out->Add()], c);
}

// Copies the contacts `keep` accepts into `out` while it has room, skipping a contact at the height
// of one it already has.
template <u32 N, class F>
__attribute__((always_inline)) static inline void UnkMergeContacts(UnkContactList2D* out,
                                                                   UnkContactArray2D<N>& list, F keep) {
    for (u32 i = 0; i < list.count; ++i) {
        if (out->count > 3)
            break;
        if (keep(list.At(i)))
            UnkMergeContact(out, list.At(i));
    }
}

template <u32 N>
__attribute__((always_inline)) static inline void UnkMergeAll(UnkContactList2D* out, UnkContactArray2D<N>& list) {
    for (u32 i = 0; i < list.count; ++i) {
        if (out->count > 3)
            break;
        UnkMergeContact(out, list.At(i));
    }
}

// A box against a capsule: its upper circle (the contacts facing up), its lower circle (those
// facing down), its left side and its right side, stopping once the list has four contacts. With
// no contact and `flag` set, a capsule whose centre lies in the box, or that holds the box's centre,
// gives one contact at that centre. Returns whether any test hit (or the list's count after the
// centre test).
// NON_MATCHING(minor): the same calls, tests and stores in the same order; the frame is 0x20 bytes
// smaller (the original keeps the two lists, the shapes and the segment ends at other offsets),
// registers and the blocks of the merge loops are allocated and laid out differently, and the copy
// of a merged contact reads three of its words through the first access's address.
bool fn_7100129ff0(const UnkBox2D* box, const UnkShape8Geometry* shape, UnkContactBuffer<4>* buffer,
                   bool flag) {
    const UnkCapsule2D* a = reinterpret_cast<const UnkCapsule2D*>(shape);
    UnkContactList2D* out = reinterpret_cast<UnkContactList2D*>(buffer);
    bool hit = false;
    UnkContactArray2D<8> list;
    list.count = 0;
    {
        UnkCircle2D top(UnkVec2Result(0.0f + a->center.x, a->half_height + a->center.y), a->radius);
        if (fn_7100126bf0(box, &top, reinterpret_cast<UnkContactBuffer<8>*>(&list), false)) {
            hit = true;
            UnkMergeContacts(out, list, [](const UnkContact2D& c) { return c.normal_b.y > -FLT_MIN; });
        }
    }
    if (out->count == 4)
        return true;
    list.count = 0;
    {
        UnkCircle2D bottom(UnkVec2Result(a->center.x, a->center.y - a->half_height), a->radius);
        if (fn_7100126bf0(box, &bottom, reinterpret_cast<UnkContactBuffer<8>*>(&list), false)) {
            hit = true;
            UnkMergeContacts(out, list, [](const UnkContact2D& c) { return c.normal_b.y < FLT_MIN; });
        }
    }
    if (out->count == 4)
        return true;
    {
        UnkContactArray2D<2> sides;
        sides.count = 0;
        {
            f32 x = a->center.x - a->radius;
            f32 y = a->center.y;
            UnkSegment2D side(UnkVec(x, y - a->half_height), UnkVec(0.0f + x, a->half_height + y));
            if (fn_7100128510(box, &side, reinterpret_cast<UnkContactBuffer<2>*>(&sides), false)) {
                hit = true;
                UnkMergeAll(out, sides);
            }
        }
        if (out->count == 4)
            return true;
        sides.count = 0;
        {
            f32 x = a->radius + a->center.x;
            f32 y = 0.0f + a->center.y;
            UnkSegment2D side(UnkVec(0.0f + x, a->half_height + y), UnkVec(x, y - a->half_height));
            if (fn_7100128510(box, &side, reinterpret_cast<UnkContactBuffer<2>*>(&sides), false)) {
                hit = true;
                UnkMergeAll(out, sides);
            }
        }
    }
    if (out->count != 0 || !flag)
        return hit;
    if (a->center.x >= box->x0 && a->center.x <= box->x1 && a->center.y >= box->y0 && a->center.y <= box->y1) {
        if (out->count == 0)
            out->Add();
        out->items[0].SetPoint(CVector3D{a->center.x, a->center.y, 0.0f});
    } else {
        CVector2D mid = UnkVec((box->x0 + box->x1) * 0.5f, (box->y0 + box->y1) * 0.5f);
        if (!fn_710010f774(a, &mid))
            return out->count != 0;
        if (out->count == 0)
            out->Add();
        out->items[0].SetPoint(CVector3D{(box->x0 + box->x1) * 0.5f, (box->y0 + box->y1) * 0.5f, 0.0f});
    }
    return out->count != 0;
}

// A circle against two circles, against a segment (declared as src/game/UnkCollisionContacts.cpp
// does), and whether a circle lies inside a capsule (within `tolerance`).
bool fn_7100127720(const UnkCircle2D* a, const UnkCircle2D& b, UnkContactBuffer<2>* out, bool flag);
bool fn_71001271d0(const void* b, const UnkCircle2D* a, UnkContactBuffer<2>* out, bool flag);
bool fn_710010f838(const UnkCircle2D* circle, const UnkCapsule2D* capsule, f32 tolerance);

// A circle against a capsule, as fn_7100129ff0 (the list is not checked for four contacts after the
// upper circle). With no contact and `flag` set, a circle inside the capsule gives a contact at the
// capsule's centre, else a capsule holding the circle's centre one at that centre.
// NON_MATCHING(minor): as fn_7100129ff0: the same calls, tests and stores in the same order; the
// frame is 0x20 bytes smaller (the list, the circles and the segments sit at other offsets), and
// registers and the merge loops' blocks are allocated and laid out differently.
bool fn_710012a9a0(const UnkCircle2D* circle, const UnkShape8Geometry* shape, UnkContactBuffer<4>* buffer,
                   bool flag) {
    const UnkCapsule2D* a = reinterpret_cast<const UnkCapsule2D*>(shape);
    UnkContactList2D* out = reinterpret_cast<UnkContactList2D*>(buffer);
    bool hit = false;
    UnkContactArray2D<2> list;
    list.count = 0;
    {
        UnkCircle2D top(UnkVec2Result(0.0f + a->center.x, a->half_height + a->center.y), a->radius);
        if (fn_7100127720(circle, top, reinterpret_cast<UnkContactBuffer<2>*>(&list), false)) {
            hit = true;
            UnkMergeContacts(out, list, [](const UnkContact2D& c) { return c.normal_b.y > -FLT_MIN; });
        }
    }
    list.count = 0;
    {
        UnkCircle2D bottom(UnkVec2Result(a->center.x, a->center.y - a->half_height), a->radius);
        if (fn_7100127720(circle, bottom, reinterpret_cast<UnkContactBuffer<2>*>(&list), false)) {
            hit = true;
            UnkMergeContacts(out, list, [](const UnkContact2D& c) { return c.normal_b.y < FLT_MIN; });
        }
    }
    if (out->count == 4)
        return true;
    list.count = 0;
    {
        f32 x = a->center.x - a->radius;
        f32 y = a->center.y;
        UnkSegment2D side(UnkVec(x, y - a->half_height), UnkVec(0.0f + x, a->half_height + y));
        if (fn_71001271d0(&side, circle, reinterpret_cast<UnkContactBuffer<2>*>(&list), false)) {
            hit = true;
            UnkMergeAll(out, list);
        }
    }
    if (out->count == 4)
        return true;
    list.count = 0;
    {
        f32 x = a->radius + a->center.x;
        f32 y = 0.0f + a->center.y;
        UnkSegment2D side(UnkVec(0.0f + x, a->half_height + y), UnkVec(x, y - a->half_height));
        if (fn_71001271d0(&side, circle, reinterpret_cast<UnkContactBuffer<2>*>(&list), false)) {
            hit = true;
            UnkMergeAll(out, list);
        }
    }
    if (out->count != 0 || !flag)
        return hit;
    if (fn_710010f838(circle, a, 0.0f)) {
        if (out->count == 0)
            out->Add();
        out->items[0].SetPoint(CVector3D{a->center.x, a->center.y, 0.0f});
    } else {
        if (!fn_710010f774(a, reinterpret_cast<const CVector2D*>(&circle->center)))
            return out->count != 0;
        if (out->count == 0)
            out->Add();
        out->items[0].SetPoint(CVector3D{circle->center.x, circle->center.y, 0.0f});
    }
    return out->count != 0;
}

// The C library's allocation functions, which the game defines itself over its default allocator
// (fn_7100083990, flags 0xd): malloc, free, calloc, realloc and aligned_alloc. Named by address,
// because on the port malloc and free are the host's.
void* fn_7100132300(u64 size) {
    return fn_7100083990()->vf_0x40(size, 8, 0xd);
}

void fn_7100132340(void* ptr) {
    fn_7100083990()->vf_0x48(ptr);
}

void* fn_7100132370(u64 count, u64 size) {
    u64 total = size * count;
    void* p = fn_7100083990()->vf_0x40(total, 8, 0xd);
    if (p)
        memset(p, 0, total);
    return p;
}

// Grows by copying into a new block; never shrinks.
// NON_MATCHING(minor): the allocator getter fn_7100083990 is called once before the null test
// here and once on each path in the original (the inlined malloc is hoisted with the rest); the
// same calls with the same arguments on every path.
void* fn_71001323d0(void* ptr, u64 size) {
    if (!ptr)
        return fn_7100132300(size);  // malloc, inlined
    base::UnkAllocator* allocator = fn_7100083990();
    if (!size) {
        allocator->vf_0x48(ptr);
        return nullptr;
    }
    u64 old_size = allocator->vf_0x50(ptr);
    if (old_size >= size)
        return ptr;
    void* p = fn_7100083990()->vf_0x40(size, 8, 0xd);
    memcpy(p, ptr, old_size);
    fn_7100083990()->vf_0x48(ptr);
    return p;
}

void* fn_71001324b0(u64 align, u64 size) {
    return fn_7100083990()->vf_0x40(size, align, 0xd);
}
