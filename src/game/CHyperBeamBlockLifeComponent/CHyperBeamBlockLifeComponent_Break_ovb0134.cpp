// CHyperBeamBlockLifeComponent's unit (tools/tu_map.py unit 1378): fn_71008df930 starts the block's
// destruction. When the owner (+0x30) has a timeline (+0x2e8) it is sent the id g_unk_710164b158
// with the default mask (the guarded statics g_unk_7101d57cb8 and g_unk_7101d57cc8, as in
// CMagmaKraidPistonPlatformComponent_Link_ovb0126.cpp), and the component's listener (+0x3a0) is
// moved onto the timeline (fn_7100a2d4d0 detaches it, fn_710082efe8 hands out the timeline's link,
// fn_7100a2d4c8 attaches it) to call fn_71008dfe04. Then the owner's action switcher
// (fn_71002b3b84) gets the hash 0xa6a4b8f65b760be7. Returns 0. Names and layouts are placeholders.
#include "game/CNavMeshItemComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkTU372.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHyperStaticZero_ovb0134 {
    u32 value;
    UnkHyperStaticZero_ovb0134() : value(0) {}
};

inline const UnkHyperStaticZero_ovb0134& UnkHyperStaticZero() {
    static UnkHyperStaticZero_ovb0134 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkHyperDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkHyperStaticZero().value & 0xff;
    return s_mask;
}

extern u64 g_unk_710164b158;  // the timeline id
void fn_7100a2d4c8(UnkWeakTarget* target, void* listener);
void fn_710082efe8(UnkTimelineComponent* timeline, UnkWeakHandle** out);

struct UnkHyperBeamBlockLife_ovb0134;
void fn_71008dfe04(UnkHyperBeamBlockLife_ovb0134* self);

struct UnkHyperOwner_ovb0134 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

// A listener: the object and function it calls, and the counted link to what it listens to.
struct UnkHyperListener_ovb0134 {
    void** vtable;
    UnkHyperBeamBlockLife_ovb0134* context;               // 0x8
    void (*callback)(UnkHyperBeamBlockLife_ovb0134*);     // 0x10
    UnkWeakHandle* link;                                  // 0x18
};

struct UnkHyperBeamBlockLife_ovb0134 {
    u8 pad_0x0[0x30];
    UnkHyperOwner_ovb0134* owner;  // 0x30
    u8 pad_0x38[0x368];
    UnkHyperListener_ovb0134 listener;  // 0x3a0
};

u32 fn_71008df930(UnkHyperBeamBlockLife_ovb0134* self) {
    if (UnkTimelineComponent* timeline = self->owner->timeline) {
        alignas(8) u32 value = UnkHyperDefaultMask();
        fn_7100a2cb7c(timeline, &g_unk_710164b158, &value, false, -1.0f);
        UnkHyperListener_ovb0134* listener = &self->listener;
        listener->context = self;
        listener->callback = fn_71008dfe04;
        if (UnkWeakHandle* link = listener->link) {
            if (UnkWeakTarget* target = link->target)
                fn_7100a2d4d0(target, listener);
            if (UnkWeakHandle* p = listener->link) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                listener->link = nullptr;
            }
        }
        fn_710082efe8(timeline, reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<u8*>(self) + 0x3b8));
        fn_7100a2d4c8(listener->link ? listener->link->target : nullptr, listener);
    }
    if (void* switcher = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(switcher, 0xa6a4b8f65b760be7);
    return 0;
}
