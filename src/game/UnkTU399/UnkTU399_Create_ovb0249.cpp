// Unit 399 (tools/tu_map.py): creates the node a definition describes (an object of the class the
// definition's vf_0x18 gives, when that is an animtree::CAnimTreeControl and has a constructor at
// +0x28), lets the node read the definition (vf_0x38) and appends it to the vector at +0xa0.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
#include "game/UnkAnimationComponent.h"

struct UnkNode_b0249 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(base::reflection::UnkReflObjectB* def, void* owner);
};

struct UnkClassView_b0249 {
    u8 pad_0x0[0x18];
    u64 size;  // 0x18
    u8 pad_0x20[8];
    void (*construct)(void* node);  // 0x28
};

struct UnkNodeOwner_b0249 {
    u8 pad_0x0[0xa0];
    base::UnkVector<UnkNode_b0249*> nodes;  // 0xa0
};

UnkNode_b0249* fn_71002d5070(UnkNodeOwner_b0249* self, base::reflection::UnkReflObjectB* def) {
    UnkClassView_b0249* cls = reinterpret_cast<UnkClassView_b0249*>(const_cast<base::reflection::CClass*>(def->vf_0x18()));
    UnkNode_b0249* node = nullptr;
    if (fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(cls), fn_71002d5cf0()) &&
        cls->construct) {
        node = static_cast<UnkNode_b0249*>(fn_7100083990()->vf_0x40(cls->size, 8, 0x40));
        if (node)
            cls->construct(node);
    }
    node->vf_0x38(def, self);
    if (node)
        self->nodes.PushBack(node);
    return node;
}

// Unit 399 (tools/tu_map.py): for each component of the list at +0xc8 (count +0xd8) whose track (+0x70) is playing an
// action, hands the track the all-bits flag value (fn_7100714eb4, on its +0x38) and stores its progress (fn_710070d890)
// at +0xa8. Names and layouts are placeholders.

struct UnkFlags_b0249 {
    u32 bits;
    UnkFlags_b0249() : bits(0) {}
    UnkFlags_b0249(u32 b) : bits(b) {}
    UnkFlags_b0249(const UnkFlags_b0249& o) : bits(o.bits) {}
};

inline const UnkFlags_b0249& UnkFlagsNone_b0249() {
    static UnkFlags_b0249 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_b0249& UnkFlagsAll_b0249() {
    static UnkFlags_b0249 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_b0249().bits & 7);
    return s_all;
}

f32 fn_710070d890(UnkTrack* track);
void fn_7100714eb4(void* part, const u32* flags);

struct UnkTrackHolder_b0249 {
    u8 pad_0x0[0x70];
    UnkTrack* track;  // 0x70
};

struct UnkTrackList_b0249 {
    u8 pad_0x0[0xc8];
    UnkTrackHolder_b0249** items;  // 0xc8
    u8 pad_0xd0[8];
    u32 count;  // 0xd8
};

void fn_71002d596c(UnkTrackList_b0249* self) {
    UnkTrackHolder_b0249** end = self->items + self->count;
    for (UnkTrackHolder_b0249** p = self->items; p != end; ++p) {
        UnkTrack* track = (*p)->track;
        if (!track || !fn_710070d500(track))
            continue;
        void* part = reinterpret_cast<char*>(track) + 0x38;
        UnkFlags_b0249 flags = UnkFlagsAll_b0249();
        fn_7100714eb4(part, &flags.bits);
        *reinterpret_cast<f32*>(reinterpret_cast<char*>(track) + 0xa8) = fn_710070d890(track);
    }
}
