// Unit 356 (navmesh query state): asks the navmesh manager whether the query's start cell connects to its
// target cell, or to any of its target cells (each distinct cell asked once). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

bool fn_7100ec1644(UnkNavMeshManager* self, u16 a, u16 b, u32 flags, u32 extra);

struct UnkNavTargetEntry_l282e88 {
    u16 cell;
    u8 side;
    f32 x;
    f32 y;
};

struct UnkNavRequest_l282e88 {
    u32 flags;  // 0x0
    u8 pad_0x4[0x10];
    u16 start;  // 0x14
    u32 mode;  // 0x18: 1, one target cell; 2, a list of them
    bool enabled;  // 0x1c
    f32 x;  // 0x20
    f32 y;  // 0x24
    u16 target;  // 0x28
    u32 count;  // 0x2c
    UnkNavTargetEntry_l282e88 entries[23];  // 0x30
};
static_assert(__builtin_offsetof(UnkNavRequest_l282e88, entries) == 0x30, "request");

struct UnkNavQueryState_l282e88 {
    u8 pad_0x0[0x5dc84];
    UnkNavRequest_l282e88 request;  // 0x5dc84
};

// The cells asked about so far.
struct UnkNavCellSet_l282e88 {
    u32 count;
    u16 cells[18];

    const u16* End() const { return cells + count; }

    // The first slot holding `cell`, or End().
    const u16* Find(u16 cell) const {
        const u16* end = End();
        for (const u16* it = cells; it != end; it++) {
            if (*it == cell)
                return it;
        }
        return end;
    }
};

static inline UnkNavMeshManager* GetNavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 3));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

bool fn_7100282e88(UnkNavQueryState_l282e88* self) {
    UnkNavRequest_l282e88& request = self->request;
    if (!request.enabled)
        return false;

    switch (request.mode) {
    case 1:
        if (request.start == 0xffff || request.target == 0xffff)
            return false;
        return fn_7100ec1644(GetNavMeshManager(), request.start, request.target, request.flags, 0);
    case 2: {
        UnkNavCellSet_l282e88 asked;
        asked.count = 0;
        const UnkNavTargetEntry_l282e88* entries = request.entries;
        for (u32 i = 0; i < request.count; i++) {
            const UnkNavTargetEntry_l282e88& entry = entries[i];
            if (asked.Find(entry.cell) != asked.End())
                continue;
            asked.cells[asked.count++] = entry.cell;
            if (fn_7100ec1644(GetNavMeshManager(), request.start, entry.cell, request.flags, 0))
                return true;
        }
        break;
    }
    }
    return false;
}
