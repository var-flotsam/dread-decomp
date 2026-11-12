// CShockWavePoolComponent's unit: the actors it tracks, in the node list at 0x268 (allocated from its
// allocator with the list's flags). fn_71009f7da4 adds an actor, unless the actor is the player
// (fn_7100defcd4) or the game manager's second player (+0x29a0, its +0x10) and one of the two is already
// in the list. fn_71009f7ec4 answers whether the actor is hit: a tracked actor only while active (+0x244)
// with the timer (+0x320) at 0 or when forced; then always without a segment of points (+0x2a8), else
// when the actor's box (fn_7100d7bc70), projected to 2D, touches the segment of the first two points
// (fn_710012b2c0). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkOwner;

struct UnkPoolNode_ovb0069 {
    UnkOwner* value;
    UnkPoolNode_ovb0069* next;
};

struct UnkPoolPlayerRef_ovb0069 {
    u8 pad_0x0[0x10];
    UnkOwner* actor;  // 0x10
};

struct UnkPoolList_ovb0069 {
    void** vtable;
    u32 count;
    UnkPoolNode_ovb0069* head;
    UnkPoolNode_ovb0069* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    s32 IndexOf(UnkOwner* value) const {
        u32 i = 0;
        for (UnkPoolNode_ovb0069* n = head; n; n = n->next, ++i)
            if (n->value == value)
                return static_cast<s32>(i);
        return -1;
    }

    void PushBack(UnkOwner* value) {
        auto* node = static_cast<UnkPoolNode_ovb0069*>(allocator->vf_0x40(0x10, 8, flags));
        if (!node)
            return;
        node->value = value;
        node->next = nullptr;
        if (count == 0) {
            tail = node;
            head = node;
            count = 1;
            return;
        }
        tail->next = node;
        // An untyped store: the original reloads the count after it.
        __builtin_memset(&node->next, 0, sizeof(node->next));
        u32 n = count + 1;
        tail = node;
        count = n;
    }
};

struct UnkEntityBox {
    base::math::CVector3D min;
    base::math::CVector3D max;
};
struct UnkEntityBounds;

struct UnkBox2D {
    f32 x0, y0;
    f32 x1, y1;
};
struct UnkContactPoint_ovb0069 {
    u8 data[0x2c];
};
template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPoint_ovb0069 points[N];
};
bool fn_710012b2c0(const void* b, const UnkBox2D* a, UnkContactBuffer<2>* out, bool flag);

struct UnkShockWavePoolAdd_ovb0069 {
    u8 pad_0x0[0x244];
    bool active;  // 0x244
    u8 pad_0x245[0x23];
    UnkPoolList_ovb0069 list;  // 0x268
    u8 pad_0x298[0x10];
    base::math::CVector3D* points;  // 0x2a8
    u32 point_count;                // 0x2b0
    u8 pad_0x2b4[0x20];
    bool field_0x2d4;
    u8 pad_0x2d5[0x4b];
    f32 timer;  // 0x320
};
static_assert(__builtin_offsetof(UnkShockWavePoolAdd_ovb0069, timer) == 0x320, "layout");

static inline UnkOwner* SecondPlayer(UnkGameManager* gm) {
    auto* ref = *reinterpret_cast<UnkPoolPlayerRef_ovb0069**>(reinterpret_cast<char*>(gm) + 0x29a0);
    return ref ? ref->actor : nullptr;
}

static inline bool IsPlayer(UnkOwner* actor) {
    return fn_7100defcd4(g_unk_7101d47310) == actor || SecondPlayer(g_unk_7101d47310) == actor;
}

void fn_71009f7da4(UnkShockWavePoolAdd_ovb0069* self, UnkOwner* actor) {
    if (IsPlayer(actor)) {
        if (self->list.IndexOf(fn_7100defcd4(g_unk_7101d47310)) >= 0)
            return;
        if (self->list.IndexOf(SecondPlayer(g_unk_7101d47310)) >= 0)
            return;
    }
    self->list.PushBack(actor);
}

bool fn_71009f7ec4(UnkShockWavePoolAdd_ovb0069* self, UnkOwner* actor, void* unused, bool force) {
    bool listed;
    if (IsPlayer(actor))
        listed = self->list.IndexOf(fn_7100defcd4(g_unk_7101d47310)) >= 0 ||
                 self->list.IndexOf(SecondPlayer(g_unk_7101d47310)) >= 0;
    else
        listed = self->list.IndexOf(actor) >= 0;
    if (listed && !(self->active && (self->timer == 0.0f || force)))
        return false;
    // Fewer points than a segment needs: a single point counts only when +0x2d4 is clear.
    u32 n = self->point_count;
    bool single = !self->field_0x2d4;
    if (n == static_cast<u32>(n != 0 && single))
        return true;
    UnkEntityBox box = fn_7100d7bc70(reinterpret_cast<const UnkEntityBounds*>(actor));
    UnkContactBuffer<2> contacts;
    contacts.count = 0;
    const base::math::CVector3D* p = self->points;
    UnkSegment2D segment(*reinterpret_cast<const base::math::CVector2D*>(p),
                         *reinterpret_cast<const base::math::CVector2D*>(self->point_count == 1 ? p : p + 1));
    base::math::CVector2D corners[2];
    fn_710007b430(&corners[0], &box.min);
    fn_710007b430(&corners[1], &box.max);
    return fn_710012b2c0(&segment, reinterpret_cast<const UnkBox2D*>(corners), &contacts, true);
}
