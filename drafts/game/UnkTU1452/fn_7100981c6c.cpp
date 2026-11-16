// Unit 1452 (tools/tu_map.py): when the grapple point (fn_710097f1e0, its +0x10) is a
// CCharClassGrapplePointComponent, plays its animation on the owner's (fn_710097f1b8) animation (+0x220):
// the named one (+0x60) when it has a name (built by fn_71007121e0), else the one whose id hash is the
// singleton's part at +0x1258 as the CCharClassTweakGrappleDragActionTrack fills it in (which also sets the
// flag). Flags 0x10, bit 0 from the flag, are added. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1450.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::CType;

struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;

// fn_71007121e0's parameters, as src/game/CAlternativeActionPlayerComponent defines them.
struct UnkAltDesc_ovb0115c {
    u64 words[2];
};

struct __attribute__((packed, aligned(4))) UnkAltBlend_ovb0115c {
    u64 kind;
    f32 value;
};

struct alignas(8) UnkAltFlags_ovb0115 {
    u32 value;
    UnkAltFlags_ovb0115() { value = 0; }
};

struct UnkAltNamedRequest_ovb0115;
void fn_71007121e0(UnkAltNamedRequest_ovb0115* self, const CStrId& name, const UnkAltFlags_ovb0115& flags,
                   UnkAltDesc_ovb0115c desc, UnkAltBlend_ovb0115c blend);

struct UnkPartId_l981c6c {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAnimTime_l981c6c {
    u64 a;
    u64 b;
};

struct UnkAnimBlend_l981c6c {
    u32 a;
    u32 b;
    u32 c;
};

// What fn_7101176ea8 plays.
struct UnkAnimRequest_l981c6c {
    u64 id;
    UnkAnimTime_l981c6c time;
    UnkAnimBlend_l981c6c blend;
    u32 flags;
    u32 mask;

    UnkAnimRequest_l981c6c(u64 id_, UnkAnimTime_l981c6c time_, UnkAnimBlend_l981c6c blend_, u32 flags_, u32 mask_)
        : id(id_), time(time_), blend(blend_), flags(flags_), mask(mask_) {}
};

// The same request with the animation's name, as fn_71007121e0 builds it.
struct UnkNamedRequest_l981c6c {
    u64 id;
    UnkAnimTime_l981c6c time;
    UnkAnimBlend_l981c6c blend;
    u32 flags;
    u32 mask;
    UnkStringData* name;

    UnkNamedRequest_l981c6c(const CStrId& name_, const UnkAltFlags_ovb0115& flags_, UnkAltDesc_ovb0115c desc,
                            UnkAltBlend_ovb0115c blend_) {
        fn_71007121e0(reinterpret_cast<UnkAltNamedRequest_ovb0115*>(this), name_, flags_, desc, blend_);
    }
    ~UnkNamedRequest_l981c6c() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (name && name != &pool->empty) {
                --name->ref_count;
                if (name->ref_count == 0) {
                    fn_71000803a4(pool, name);
                    pool = g_unk_7101d47108;
                }
            }
            name = &pool->empty;
        }
    }
};

struct UnkDragTrack_l981c6c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkAnimRequest_l981c6c* request);
};

struct UnkGrapplePoint_l981c6c {
    virtual const CType* vf_0x0();
    u8 pad_0x8[0x58];
    CStrId name;  // 0x60
};

struct UnkGrappleSource_l981c6c {
    u8 pad_0x0[0x10];
    UnkGrapplePoint_l981c6c* point;
};

struct UnkAnimOwner_l981c6c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

static inline u64 PartHash_l981c6c(u64 offset) {
    return (*reinterpret_cast<UnkPartId_l981c6c**>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) +
                                                    offset))
        ->hash;
}

static inline UnkAnimationComponent* Animation_l981c6c(UnkUnit1452_b56* self) {
    return reinterpret_cast<UnkAnimOwner_l981c6c*>(fn_710097f1b8(self))->animation;
}

// Plays a copy of the request with flags 0x10 (bit 0 from `flag`) added.
template <class Request>
static inline void Play_l981c6c(UnkUnit1452_b56* self, const Request& request, bool flag) {
    UnkAnimationComponent* animation = Animation_l981c6c(self);
    UnkAnimRequest_l981c6c play(request.id, request.time, request.blend, request.flags | (flag ? 0x11u : 0x10u),
                                request.mask);
    fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                  reinterpret_cast<const UnkRequest_7101176ea8*>(&play), 0xf4061ff25a2163d6);
}

void fn_7100981c6c(UnkUnit1452_b56* self, bool flag) {
    UnkGrappleSource_l981c6c* source = reinterpret_cast<UnkGrappleSource_l981c6c*>(fn_710097f1e0(self));
    if (!source || !source->point)
        return;
    UnkGrapplePoint_l981c6c* point = source->point;
    if (!fn_710009d50c(point->vf_0x0(), fn_71008163f4()))
        return;
    if (CStrId(point->name).data == &g_unk_7101d47108->empty) {
        u64 id = PartHash_l981c6c(0x1258);
        UnkAnimTime_l981c6c time = reinterpret_cast<const UnkAnimTime_l981c6c&>(g_unk_710164b958);
        UnkAnimBlend_l981c6c blend = reinterpret_cast<const UnkAnimBlend_l981c6c&>(g_unk_7172769740);
        u32 mask = UnkAll_SpitSend().value;
        UnkAnimRequest_l981c6c request(id, time, blend, 0, mask);
        UnkTrackQuery query = {fn_7100c596dc(), nullptr, -1.0f};
        if (fn_710117b08c(Animation_l981c6c(self), &query)) {
            reinterpret_cast<UnkDragTrack_l981c6c*>(query.result)->vf_0x18(&request);
            flag = true;
        }
        Play_l981c6c(self, request, flag);
    } else {
        CStrId name = point->name;
        UnkNamedRequest_l981c6c request(name, UnkAltFlags_ovb0115(),
                                        reinterpret_cast<const UnkAltDesc_ovb0115c&>(g_unk_710164b958),
                                        reinterpret_cast<const UnkAltBlend_ovb0115c&>(g_unk_7172769740));
        Play_l981c6c(self, request, flag);
    }
}
