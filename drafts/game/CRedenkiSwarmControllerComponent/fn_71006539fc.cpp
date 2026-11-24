// CRedenkiSwarmControllerComponent's unit (977): assigns swarm members to target points by an auction.
// Every member bids for the target whose cost minus its current price is best; a displaced member is
// queued again. Members left over take their cheapest target. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkTU1161.h"
#include "types.h"

using base::global::CRntVector;

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkParent_71007455dc;

// The "no owner" index.
extern u32 g_unk_7101a5aee4;

struct UnkClone_fn_710065b7f4;
extern template void base::refl_ResizeSubVecs(
    base::global::CRntVector<base::UnkCloneSubVec<UnkClone_fn_710065b7f4>>*, u32);
extern template unsigned int& base::global::CRntVector<unsigned int>::GrowAt(u32);

// A target's current price and the member holding it.
struct UnkTargetClaim_l6539fc {
    f32 price = 0.0f;
    u32 owner = g_unk_7101a5aee4;
    ~UnkTargetClaim_l6539fc() {}
};

// A queue of member indices, nodes from an allocator.
struct UnkIndexQueue_l6539fc {
    struct Node {
        u32 value;
        Node* next;
    };

    base::IAllocator* allocator = GetDefaultAllocator();
    Node* head = nullptr;
    Node* tail = nullptr;
    u32 count = 0;

    ~UnkIndexQueue_l6539fc() {
        for (Node* n = head; n;) {
            Node* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }

    void PushBack(u32 value) {
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, 0x13));
        if (!n)
            return;
        n->value = value;
        n->next = nullptr;
        if (count == 0) {
            head = n;
            tail = n;
            count = 1;
        } else {
            tail->next = n;
            tail = n;
            n->next = nullptr;
            ++count;
        }
    }

    u32 PopFront() {
        u32 value = head->value;
        if (head) {
            Node* next = head->next;
            allocator->vf_0x48(head);
            if (--count == 0)
                tail = next;
            head = next;
        }
        return value;
    }
};

using UnkCostRow_l6539fc = CRntVector<f32>;
using UnkCostTable_l6539fc = CRntVector<UnkCostRow_l6539fc>;

// fn_710065b7f4: resizes the table to `n` rows.
static inline void ResizeRows(UnkCostTable_l6539fc& rows, u32 n) {
    base::refl_ResizeSubVecs(
        reinterpret_cast<base::global::CRntVector<base::UnkCloneSubVec<UnkClone_fn_710065b7f4>>*>(&rows), n);
}

// The row at `i`, growing the table to it.
static inline UnkCostRow_l6539fc& RowAt(UnkCostTable_l6539fc& rows, u32 i) {
    if (rows.field_0x14 && rows.count <= i) {
        if (rows.capacity <= i && rows.step != 0) {
            u32 grown = (i / rows.step + 1) * rows.step;
            rows.Reserve(grown);
        }
        ResizeRows(rows, i + 1);
    }
    return rows.data[i];
}

static inline UnkCostRow_l6539fc& LastRow(UnkCostTable_l6539fc& rows) {
    u32 n = rows.count;
    UnkCostRow_l6539fc* end = &rows.data[n];
    return n ? end[-1] : *end;
}

CRntVector<u32> fn_71006539fc(void* self, const CRntVector<UnkParent_71007455dc*>& members,
                              const CRntVector<base::math::CVector3D>& targets,
                              f32 (*cost)(const UnkVec3Result&, const base::math::CVector3D&)) {
    u32 targetCount = targets.count;
    UnkIndexQueue_l6539fc queue;
    UnkCostTable_l6539fc costs(0xf);

    // Queue every member and fill its row of costs.
    u32 memberCount = members.count;
    for (u32 i = 0; i < memberCount; ++i) {
        queue.PushBack(i);
        UnkVec3Result pos = fn_7100745e24(members.data[i]);
        ResizeRows(costs, costs.count + 1);
        UnkCostRow_l6539fc& row = LastRow(costs);
        for (u32 j = 0; j < targetCount; ++j)
            row.PushBack(cost(pos, targets.data[j]));
    }

    CRntVector<UnkTargetClaim_l6539fc> claims(0xf);
    if (targetCount != 0) {
        claims.Reserve(targetCount);
        for (u32 j = 0; j < targetCount; ++j)
            new (&claims.data[j]) UnkTargetClaim_l6539fc();
        claims.count = targetCount;
    }

    // Bid until only the members that cannot get a target are left.
    u32 spare = memberCount < targetCount ? 0 : memberCount - targetCount;
    if (queue.count > spare) {
        f32 increment = 1.0f / ((f32)targetCount + 1.0f);
        do {
            u32 member = queue.PopFront();
            UnkCostRow_l6539fc& row = RowAt(costs, member);
            f32 best = row.data[0] - claims.GrowAt(0).price;
            u32 bestTarget = 0;
            for (u32 k = 1; k < targetCount; ++k) {
                f32 value = row.data[k] - claims.GrowAt(k).price;
                if (value > best) {
                    best = value;
                    bestTarget = k;
                }
            }
            if (best >= 0.0f) {
                UnkTargetClaim_l6539fc& claim = claims.GrowAt(bestTarget);
                if (claim.owner != g_unk_7101a5aee4)
                    queue.PushBack(claim.owner);
                claim.owner = member;
                claim.price += increment;
            }
        } while (queue.count > spare);
    }

    CRntVector<u32> result(0xf);
    if (result.count < memberCount) {
        result.Reserve(memberCount);
        __builtin_memset(&result.data[result.count], 0, (u64)(memberCount - result.count) * sizeof(u32));
        result.count = memberCount;
    } else if (result.count > memberCount) {
        result.Truncate(memberCount);
    }
    for (u32 j = 0; j < targetCount; ++j)
        result.GrowAt(claims.GrowAt(j).owner) = j;

    // The members left over take the target that costs them most.
    for (u32 r = 0; r < spare; ++r) {
        if (queue.count == 0)
            continue;
        u32 member = queue.PopFront();
        UnkCostRow_l6539fc& row = RowAt(costs, member);
        u32 bestTarget = 0;
        if (targetCount >= 2) {
            f32 best = row.data[0];
            for (u32 k = 1; k < targetCount; ++k) {
                if (row.data[k] > best) {
                    best = row.data[k];
                    bestTarget = k;
                }
            }
        }
        result.GrowAt(member) = bestTarget;
    }

    claims.Free();
    if (costs.data) {
        for (u32 i = 0; i < costs.count; ++i)
            costs.data[i].Free();
        costs.Free();
    }
    return result;
}
