// CEmmyProtoAIComponent's unit (tools/tu_map.py unit 791):
// - fn_71004fc7ec: what the component does when its E.M.M.I. is destroyed: the "defeated" message is
//   sent to the game manager's callback set at +0x2128 (as the other E.M.M.I. components do,
//   src/game/CEmmyCaveAIComponent/UnkEmmyDefeatedMessage_b42.cpp), then the base component's
//   handler (fn_7100554740) runs.
// - fn_71004fabc8, fn_71004fac90: the component's destructor and deleting destructor.
// - fn_71004fae68: runs fn_7100541974, then detaches the callback record at +0x2a6f8 (built by
//   fn_71004faad4.cpp) from the set it is registered in.
// Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;

// The message, as UnkEmmyDefeatedMessage_b42.cpp lays it out.
struct UnkMlogText_b42a1 : base::global::UnkStrIdBase {
    __attribute__((always_inline)) UnkMlogText_b42a1(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    __attribute__((always_inline)) ~UnkMlogText_b42a1() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMlogMessage_b42a1 {
    u32 kind;
    UnkMlogText_b42a1 text;
    base::UnkVector<base::UnkStrIdMember> args;

    __attribute__((always_inline)) UnkMlogMessage_b42a1(const CStrId& t) : kind(0), text(t), args(0xc) {}
    __attribute__((always_inline)) ~UnkMlogMessage_b42a1() {
        args.Free();
    }
};

void fn_71004fc7ec(void* self, void* arg) {
    UnkMlogMessage_b42a1 message(CStrId("#MLOG_DEFEAT_PROTOEMMY_CAVE", true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128, &message);
    fn_7100554740(self, arg);
}

void fn_71004fd510(base::UnkDeferredSet<void>* set, void* item);

// The callback record: an id, a state word, and the registration in a set (the set, the
// component and the function).
struct UnkProtoCallback_ovb0003 {
    u64 field_0x0;
    base::UnkStrIdMember id;  // 0x8
    u32 field_0x10;
    base::UnkDeferredSet<void>* set;  // 0x18
    void* obj;                        // 0x20
    void (*fn)();                     // 0x28
    u32 field_0x30;

    void Detach() {
        if (set) {
            fn_71004fd510(set, &set);
            set = nullptr;
        }
    }
    ~UnkProtoCallback_ovb0003() {
        Detach();
        fn = nullptr;
        obj = nullptr;
    }
};

struct UnkComponent {
    void** vtable;
    u8 pad_0x8[0x2a6f0];
};

struct UnkProtoBase_ovb0003 : UnkComponent {
    ~UnkProtoBase_ovb0003() { fn_710053f510(this); }
};

struct UnkProtoComponent_ovb0003 : UnkProtoBase_ovb0003 {
    UnkProtoCallback_ovb0003 callback;  // 0x2a6f8

    ~UnkProtoComponent_ovb0003() { vtable = g_unk_7101a45710 + 2; }
};
static_assert(__builtin_offsetof(UnkProtoComponent_ovb0003, callback) == 0x2a6f8, "UnkProtoComponent_ovb0003 layout");

void fn_71004fabc8(UnkProtoComponent_ovb0003* self) { self->~UnkProtoComponent_ovb0003(); }

void fn_71004fac90(UnkProtoComponent_ovb0003* self) {
    self->~UnkProtoComponent_ovb0003();
    fn_7100083c00(self);
}

void fn_7100541974(UnkProtoComponent_ovb0003* self);

void fn_71004fae68(UnkProtoComponent_ovb0003* self) {
    fn_7100541974(self);
    self->callback.field_0x10 = 0;
    self->callback.Detach();
}
