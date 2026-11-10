// Unit 211 of tools/tu_map.py (no name hint): dispatches a request on its kind (0 to 10) to the player of the matching track at a fixed offset of the track block (fn_710019199c, fn_71001922b0 or fn_7100191d40); kind 4 also records its mode in the block. Names and layouts are placeholders.
#include "types.h"

struct UnkKeyPlayer_b0197;
struct UnkKeyRequest_b0197 {
    u32 kind_0x0;
    u32 mode_0x4;
};
struct UnkKeyTracks_b0197 {
    u8 pad_0x0[0x348];
    u32 state_0x348;
};

u64 fn_710019199c(UnkKeyPlayer_b0197* self, const UnkKeyRequest_b0197* request, void* track);
u64 fn_71001922b0(UnkKeyPlayer_b0197* self, const UnkKeyRequest_b0197* request, void* track, void* owner);
u64 fn_7100191d40(UnkKeyPlayer_b0197* self, const UnkKeyRequest_b0197* request, void* track);

static inline u8* At(UnkKeyTracks_b0197* tracks, u64 offset) { return reinterpret_cast<u8*>(tracks) + offset; }

u64 fn_7100190a8c(UnkKeyPlayer_b0197* self, const UnkKeyRequest_b0197* request, UnkKeyTracks_b0197* tracks) {
    switch (request->kind_0x0) {
    case 0:
        return fn_710019199c(self, request, At(tracks, 0x48));
    case 1:
        return fn_71001922b0(self, request, At(tracks, 0x68), nullptr);
    case 2:
        return fn_71001922b0(self, request, At(tracks, 0x90), nullptr);
    case 3:
        return fn_71001922b0(self, request, At(tracks, 0xb8), nullptr);
    case 4: {
        u64 r = fn_71001922b0(self, request, At(tracks, 0xe0), nullptr);
        tracks->state_0x348 = request->mode_0x4 == 2 ? 2 : 1;
        return r;
    }
    case 5:
        return fn_71001922b0(self, request, At(tracks, 0x108), tracks);
    case 6:
        return fn_71001922b0(self, request, At(tracks, 0x130), nullptr);
    case 7:
        return fn_710019199c(self, request, At(tracks, 0x158));
    case 8:
        return fn_7100191d40(self, request, At(tracks, 0x178));
    case 9:
        return fn_710019199c(self, request, At(tracks, 0x198));
    case 10:
        return fn_7100191d40(self, request, At(tracks, 0x1b8));
    default:
        return 0;
    }
}
