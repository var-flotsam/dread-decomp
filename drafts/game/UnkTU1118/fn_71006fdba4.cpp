// Unit 1118: runs and clears a pending callback, then resets an embedded state block by assigning a
// freshly constructed one over it. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CFlyingFollowPathAction.h"
#include "types.h"

template <class T>
struct UnkCopiedVector;
struct UnkThreeWords;
UnkCopiedVector<UnkThreeWords>* fn_71000cbcf0(UnkCopiedVector<UnkThreeWords>* self,
                                              const UnkCopiedVector<UnkThreeWords>* other);

struct UnkTU1118StateEntry {
    u8 flag;
    u8 pad_0x1[7];
};

struct UnkTU1118StateFlagged {
    u8 flag;
    u64 field_0x8;
    u64 field_0x10;
};

// An 8-byte value left unset on construction, with an assignment of its own (copied apart from its
// neighbours).
struct UnkTU1118StateHandle {
    u64 value;

    UnkTU1118StateHandle() {}
    UnkTU1118StateHandle& operator=(const UnkTU1118StateHandle& other) {
        value = other.value;
        return *this;
    }
};

// The copyable part of the state, offsets as in UnkTU1118State.
struct UnkTU1118StateData {
    UnkVec3f field_0x8;                    // 0x8
    u8 field_0x14;                         // 0x14
    u8 pad_0x15[7];
    u8 field_0x1c;                         // 0x1c
    u64 field_0x20;                        // 0x20
    u64 field_0x28;                        // 0x28
    u8 field_0x30;                         // 0x30
    u8 pad_0x31[3];
    u32 field_0x34;                        // 0x34
    UnkTU1118StateHandle field_0x38;       // 0x38
    UnkTU1118StateEntry field_0x40[7];     // 0x40
    u8 field_0x78;                         // 0x78
    UnkVec3f field_0x7c;                   // 0x7c
    u8 field_0x88;                         // 0x88
    u8 pad_0x89[7];
    u8 field_0x90;                         // 0x90
    u8 pad_0x91;
    u8 field_0x92;                         // 0x92
    u8 pad_0x93;
    u8 field_0x94;                         // 0x94
    u8 pad_0x95[7];
    u8 field_0x9c;                         // 0x9c
    u8 pad_0x9d;
    u8 field_0x9e;                         // 0x9e
    UnkTU1118StateFlagged field_0xa0[2];   // 0xa0
    u8 field_0xd0;                         // 0xd0

    UnkTU1118StateData()
        : field_0x8(0.0f, 0.0f, 0.0f), field_0x14(0), field_0x1c(0), field_0x20(0), field_0x28(0),
          field_0x30(0), field_0x34(0), field_0x78(0), field_0x7c(0.0f, 0.0f, 0.0f), field_0x88(0),
          field_0x90(0), field_0x92(0), field_0x94(0), field_0x9c(0), field_0x9e(0), field_0xd0(0) {
        for (u32 i = 0; i < 7; i++) {
            field_0x40[i].flag = 0;
        }
        for (u32 i = 0; i < 2; i++) {
            field_0xa0[i].flag = 0;
            field_0xa0[i].field_0x8 = 0;
            field_0xa0[i].field_0x10 = 0;
        }
    }
};

struct UnkTU1118StateTail {
    u8 field_0x0;
    u8 pad_0x1;
    u8 field_0x2;
    u8 pad_0x3;
    u8 field_0x4;
    u8 pad_0x5;
    u8 field_0x6;

    UnkTU1118StateTail() : field_0x0(0), field_0x2(0), field_0x4(0), field_0x6(0) {}
};

struct UnkTU1118State {
    void* vtable;                                    // 0x0
    UnkTU1118StateData data;                         // 0x8
    base::global::CRntVector<UnkVec3f> points;       // 0xd8
    UnkTU1118StateTail tail;                         // 0x100

    UnkTU1118State() : vtable(g_unk_7101a264d0 + 0x10), points(15) { data.field_0x34 = 2; }
    ~UnkTU1118State() {
        vtable = g_unk_7101a264d0 + 0x10;
        points.Free();
    }

    // Copies everything but the vtable; the points go through their own copy-assignment.
    UnkTU1118State& operator=(const UnkTU1118State& other) {
        data = other.data;
        fn_71000cbcf0(reinterpret_cast<UnkCopiedVector<UnkThreeWords>*>(&points),
                      reinterpret_cast<const UnkCopiedVector<UnkThreeWords>*>(&other.points));
        tail = other.tail;
        return *this;
    }
};
static_assert(__builtin_offsetof(UnkTU1118State, data.field_0x7c) == 0x7c, "");
static_assert(__builtin_offsetof(UnkTU1118State, data.field_0xd0) == 0xd0, "");
static_assert(__builtin_offsetof(UnkTU1118State, points) == 0xd8, "");
static_assert(sizeof(UnkTU1118State) == 0x108, "");

// A pending call: run once with an argument, then forgotten.
struct UnkTU1118Callback {
    void* context;
    void (*function)(void* context, void* arg);

    void Run(void* arg) {
        if (function) {
            function(context, arg);
            context = nullptr;
            function = nullptr;
        }
    }
};

struct UnkTU1118Resettable {
    u8 pad_0x0[0x30];
    void* field_0x30;                                // 0x30
    u8 pad_0x38[0x20];
    UnkTU1118Callback callback;                      // 0x58
    u8 pad_0x68[0x98];
    UnkTU1118State state;                            // 0x100
};

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_71006fdba4(UnkTU1118Resettable* self) {
    self->callback.Run(self->field_0x30);
    self->state = UnkTU1118State();
}
