// CDoorGrapplePointComponent's unit: after the base step (fn_710082ccec), when grabbed, tells the
// owner's door lock (fn_71002af860 -> fn_71008d0ef4), adds one timeline id to the owner and sends the
// game manager an event with (-1, 0, 0); otherwise adds another id. Names are placeholders.
#include "game/CPullableGrapplePointComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1376.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkWeakTarget;
struct UnkDoorLock;
UnkDoorLock* fn_71002af860(UnkWeakTarget*);

// A 12-byte zeroed argument (as src/game_s2/fn_710035131c.cpp's).
struct UnkVec {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};
void fn_7100e038b0(UnkGameManager* dispatcher, void* key, UnkVec* vec, u64 arg);

extern const u64 g_unk_710164ac38;  // timeline id when grabbed
extern const u64 g_unk_710164ac30;  // timeline id otherwise
extern char g_unk_7172724710[] __attribute__((visibility("hidden")));  // the event's key

struct UnkDoorGrapplePoint_A5_b66 {
    u8 pad_0x0[0x30];
    union {
        UnkActor* owner;
        UnkWeakTarget* weak_owner;
    };
};

void fn_71008298e4(UnkDoorGrapplePoint_A5_b66* self, void* arg, bool grabbed) {
    fn_710082ccec(self, arg, grabbed);
    if (grabbed) {
        fn_71008d0ef4(fn_71002af860(self->weak_owner));
        fn_7100d7c828(self->owner, &g_unk_710164ac38, false);
        UnkGameManager* gm = g_unk_7101d47310;
        UnkVec vec;
        vec.field_0x0 = 0xbf800000;  // -1.0f
        vec.field_0x4 = 0;
        vec.field_0x8 = 0;
        fn_7100e038b0(gm, g_unk_7172724710, &vec, 0);
    } else {
        fn_7100d7c828(self->owner, &g_unk_710164ac30, false);
    }
}
