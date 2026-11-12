// CCooldownXBossLavaDropsAttack's unit (tools/tu_map.py unit 748): advances the attack's timer (+0x2ec) by
// `dt`; while it is under the tunables' (+0x118) limit (+0x110), builds a shape (fn_7100d6169c) from the
// attack's +0x2f0 with the tunables' size (+0x114), and when the owner's (+0x8, fn_7100d7c990) movement
// cache (+0x200, fn_71010d7a6c) object meets it (fn_7100d57ca0) sends the owner's component at +0x258 a
// damage request (fn_71008c0c74, virtual 0x190) with the tunable's value (+0x1a4 of g_unk_7172716160) and
// the unit's static string id at +0xb8. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkActor.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726d7cc8 __attribute__((visibility("hidden")));

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

struct UnkLavaDropsShape_A2_b71;
void fn_7100d6169c(UnkLavaDropsShape_A2_b71* self, const void* source, f32 size);
void fn_7100d61750(UnkLavaDropsShape_A2_b71* self);
bool fn_7100d57ca0(UnkLavaDropsShape_A2_b71* self, void* body, void* other, void* out, u32 flags);

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

struct UnkLavaDropsRequest_A2_b71 {
    u8 raw[0x100];

    UnkLavaDropsRequest_A2_b71() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkLavaDropsRequest_A2_b71() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
    CStrId& Id() { return *reinterpret_cast<CStrId*>(raw + 0xb8); }
};

struct UnkLavaDropsReceiver_A2_b71 {
    void** vtable;
    void vf_0x190(UnkActor* actor, UnkLavaDropsRequest_A2_b71* request, void* a, void* b) {
        reinterpret_cast<void (*)(UnkLavaDropsReceiver_A2_b71*, UnkActor*, UnkLavaDropsRequest_A2_b71*, void*,
                                  void*)>(vtable[0x190 / 8])(this, actor, request, a, b);
    }
};

struct UnkLavaDropsTunables_A2_b71 {
    u8 pad_0x0[0x110];
    f32 limit;  // 0x110
    f32 size;   // 0x114
};

struct UnkLavaDropsAttack_A2_b71 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x108];
    UnkLavaDropsTunables_A2_b71* tunables;  // 0x118
    u8 pad_0x120[0x1cc];
    f32 timer;  // 0x2ec
    u8 shape_source[0x4];  // 0x2f0
};

void fn_71004c8428(UnkLavaDropsAttack_A2_b71* self, f32 dt) {
    self->timer += dt;
    if (self->timer >= self->tunables->limit)
        return;
    UnkActor* actor = fn_7100d7c990(self->owner);
    if (!actor)
        return;
    void* movement = *reinterpret_cast<void**>(reinterpret_cast<char*>(actor) + 0x200);
    if (!movement)
        return;
    void* body = fn_71010d7a6c(static_cast<UnkPlayerMovementCache*>(movement));
    if (!body)
        return;
    UnkLavaDropsShape_A2_b71 shape(self->shape_source, self->tunables->size);
    if (fn_7100d57ca0(&shape, shape.body, body, nullptr, 0)) {
        UnkLavaDropsRequest_A2_b71 request;
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request),
                      *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0x1a4));
        request.Id() = g_unk_71726d7cc8;
        auto* receiver =
            *reinterpret_cast<UnkLavaDropsReceiver_A2_b71**>(reinterpret_cast<char*>(self->owner) + 0x258);
        receiver->vf_0x190(actor, &request, nullptr, nullptr);
    }
}
