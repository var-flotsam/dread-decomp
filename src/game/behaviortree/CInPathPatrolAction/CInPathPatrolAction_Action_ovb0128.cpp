// behaviortree::CInPathPatrolAction's unit (tools/tu_map.py, unit 488): the node's destructor (fn_710038d180) and
// the pick of its wait (fn_710038cd5c). The destructor: its vtable
// (0x7101a26730), the listener at +0xa8 (vtable 0x7101a22d48) unlinked from the watched object behind the
// weak handle at +0xc0 and the handle released, the string ids at +0x90, +0x88, +0x80, +0x78, +0x70 and +0x68
// released, then the base's destructor fn_7100393ea0. Same shape as fn_710036be88. Names are placeholders.
#include "base/UnkDestructors.h"
#include "game/behaviortree/CBaseAction.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern "C" int rand();

extern void* g_unk_7101a26730[];  // the node's vtable
extern void* g_unk_7101a22d48[];  // the listener's vtable

struct UnkInPathPatrolDtor_ovb0128;
template <>
struct base::UnkDtorTraits<UnkInPathPatrolDtor_ovb0128> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a26730); }
    static void base(char* self) { fn_7100393ea0(self); }
};

template <>
struct base::UnkLinkTraits<UnkInPathPatrolDtor_ovb0128> {
    static char* link_vtable() { return reinterpret_cast<char*>(g_unk_7101a22d48); }
    static void unlink(char* list, char* link) { fn_710117b3f0(reinterpret_cast<UnkWeakTarget*>(list), link); }
};

struct UnkInPathPatrolLink_ovb0128 {
    static void Release(char* self) { base::refl_ReleaseLink<UnkInPathPatrolDtor_ovb0128, 0xa8>(self); }
};

__attribute__((flatten)) void fn_710038d180(char* self) {
    base::refl_Dtor<UnkInPathPatrolDtor_ovb0128, UnkInPathPatrolLink_ovb0128, base::UnkStrIdAt<0x90>,
                    base::UnkStrIdAt<0x88>, base::UnkStrIdAt<0x80>, base::UnkStrIdAt<0x78>, base::UnkStrIdAt<0x70>,
                    base::UnkStrIdAt<0x68>>(self);
}


struct UnkSelf;
void fn_7100394e2c(UnkSelf* self, void* record, f32* out);

struct UnkInPathPatrol_ovb0128 {
    u8 pad_0x0[0x78];
    u8 min_property[0x8];  // 0x78, a string id
    u8 max_property[0x8];  // 0x80, a string id
    u8 pad_0x88[0x14];
    f32 wait;              // 0x9c
    f32 timer;             // 0xa0
    bool dirty;            // 0xa4
};

// The node as fn_710038d520.cpp names it.
struct UnkHandler;

static inline bool IsAbout(f32 v, f32 target) {
    return v - 0.001f <= target && v + 0.001f >= target;
}

// When the timer runs (above -0.001) or the flag is set, picks the wait at random between the two
// blackboard values (both positive, or both -1); -1 otherwise. The timer and the flag are reset.
void fn_710038cd5c(UnkHandler* handler) {
    UnkInPathPatrol_ovb0128* self = reinterpret_cast<UnkInPathPatrol_ovb0128*>(handler);
    if (!(self->timer > -0.001f) && !self->dirty)
        return;
    f32 lo = -2.0f;
    fn_7100394e2c(reinterpret_cast<UnkSelf*>(self), self->min_property, &lo);
    f32 hi = -2.0f;
    fn_7100394e2c(reinterpret_cast<UnkSelf*>(self), self->max_property, &hi);
    f32 min = lo;
    f32 max = hi;
    f32 wait;
    if ((min > 0.0f && max > 0.0f) || (IsAbout(min, -1.0f) && IsAbout(max, -1.0f))) {
        f32 r = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
        wait = min + (max - min) * r;
    } else {
        wait = -1.0f;
    }
    self->wait = wait;
    self->dirty = false;
    self->timer = -1.0f;
}
