// The music volume overrides (sound::CMusicVolumeOverride::CTunableMusicVolumeOverride, the
// tunable at g_unk_71729f8150; the class name comes from reflection) looked up by a music
// stream's id: the switch is on the id's CRC-64. The strings of most stream paths aren't in the
// binary; the comments name the ones that are. Field names are placeholders.
#include "types.h"

// The string behind an id; its CRC-64 is compared signed.
struct UnkIdData_71010182fc {
    u8 unk_0x0[0x30];
    s64 hash;
};

struct UnkId_71010182fc {
    UnkIdData_71010182fc* data;
};

static constexpr s64 H(u64 hash) { return (s64)hash; }

struct UnkMusicVolumes_71010182fc {
    u8 unk_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    f32 field_0x80;
    f32 field_0x84;
    f32 field_0x88;
    f32 field_0x8c;
    f32 field_0x90;
    f32 field_0x94;
    f32 field_0x98;
    f32 field_0x9c;
    f32 field_0xa0;
    f32 field_0xa4;
    f32 field_0xa8;
    f32 field_0xac;
    f32 field_0xb0;
    f32 field_0xb4;
    f32 field_0xb8;
    f32 field_0xbc;
    f32 field_0xc0;
    f32 field_0xc4;
    f32 field_0xc8;
    f32 field_0xcc;
    f32 field_0xd0;
    f32 field_0xd4;
    f32 field_0xd8;
    f32 field_0xdc;
    f32 field_0xe0;
    f32 field_0xe4;
    f32 field_0xe8;
    f32 field_0xec;
    f32 field_0xf0;
    f32 field_0xf4;
    f32 field_0xf8;
    f32 field_0xfc;
    f32 field_0x100;
    f32 field_0x104;
    f32 field_0x108;
    f32 field_0x10c;
    f32 field_0x110;
    f32 field_0x114;
    f32 field_0x118;
    f32 field_0x11c;
    f32 field_0x120;
    f32 field_0x124;
    f32 field_0x128;
    f32 field_0x12c;
    f32 field_0x130;
    f32 field_0x134;
    f32 field_0x138;
    f32 field_0x13c;
    f32 field_0x140;
    f32 field_0x144;
};

extern UnkMusicVolumes_71010182fc g_unk_71729f8150;

// The override for the stream, or -2 when the stream has none.
// NON_MATCHING(minor): the original's unmatched ids branch to the return shared with the
// field_0x140 case (in the middle of the cases); ours to the one after the last case. Same
// behaviour.
f32 fn_71010182fc(const UnkId_71010182fc& id) {
    switch (id.data->hash) {
    case H(0x75f5fb3459786511):  // "streams/music/S_Boss_Commander_Outro.wav"
        return g_unk_71729f8150.field_0x128;
    case H(0x9e34aeaa2350be9e):
        return g_unk_71729f8150.field_0xfc;
    case H(0x3a71a7dc0e991e1f):
        return g_unk_71729f8150.field_0x44;
    case H(0xe36526bd337a31e6):
        return g_unk_71729f8150.field_0x8c;
    case H(0x691bd2a7b156f942):
        return g_unk_71729f8150.field_0x11c;
    case H(0xb134d51505a260d9):
        return g_unk_71729f8150.field_0xd0;
    case H(0x4b5f7595dbbb28fe):
        return g_unk_71729f8150.field_0x38;
    case H(0x1bbb0ce1a2b7a3b1):
        return g_unk_71729f8150.field_0x54;
    case H(0x7c3b93a7db9de9f5):
        return g_unk_71729f8150.field_0x84;
    case H(0x86150daa70277d3f):
        return g_unk_71729f8150.field_0x24;
    case H(0x23297a80946bbd18):
        return g_unk_71729f8150.field_0x64;
    case H(0xbba022ab4e20bb49):
        return g_unk_71729f8150.field_0xa0;
    case H(0x4dce4a256ba177e4):
        return g_unk_71729f8150.field_0xe8;
    case H(0x9f5cac4182aeecef):
        return g_unk_71729f8150.field_0xf4;
    case H(0x3ef1422d237584fc):
        return g_unk_71729f8150.field_0xd8;
    case H(0xf0d0791d7fe35abf):
        return g_unk_71729f8150.field_0x3c;
    case H(0x6fe1b57371afce91):
        return g_unk_71729f8150.field_0x74;
    case H(0x9043608088b49ed3):
        return g_unk_71729f8150.field_0xe4;
    case H(0x2e0954c07e7faac0):
        return g_unk_71729f8150.field_0x144;
    case H(0xd24fb2b889cbf040):
        return g_unk_71729f8150.field_0x34;
    case H(0x60dcc799cf7b3ae9):
        return g_unk_71729f8150.field_0x30;
    case H(0xa77b3a0198e83ed8):
        return g_unk_71729f8150.field_0xb0;
    case H(0x43faacd6df53f3ff):
        return g_unk_71729f8150.field_0xb8;
    case H(0x67184b61571cd16):
        return g_unk_71729f8150.field_0xa8;
    case H(0x7ab023c275e8ad20):
        return g_unk_71729f8150.field_0xa4;
    case H(0x88fe54149e3cedc9):
        return g_unk_71729f8150.field_0x130;
    case H(0x2af07fe462493254):
        return g_unk_71729f8150.field_0x94;
    case H(0xbf8b8c6cdd3f1c19):
        return g_unk_71729f8150.field_0x60;
    case H(0x5b57731ff560c4c5):
        return g_unk_71729f8150.field_0xc0;
    case H(0xa47694afa0033c7f):
        return g_unk_71729f8150.field_0x40;
    case H(0x3f396b1ce3751812):
        return g_unk_71729f8150.field_0x98;
    case H(0xf6a99c5cd5e463e7):
        return g_unk_71729f8150.field_0x134;
    case H(0x736b1379d6737307):
        return g_unk_71729f8150.field_0x120;
    case H(0x7434f920dcf8fe31):
        return g_unk_71729f8150.field_0xe0;
    case H(0x951ca6f8f6c6e82f):  // "streams/music/S_Boss_Commander_003.wav"
        return g_unk_71729f8150.field_0x108;
    case H(0x9e257955453d1f56):
        return g_unk_71729f8150.field_0x2c;
    case H(0x31da696d656e50df):
        return g_unk_71729f8150.field_0x140;
    case H(0x38826bc675809fce):
        return g_unk_71729f8150.field_0x90;
    case H(0xda86db8d7739e41e):
        return g_unk_71729f8150.field_0xd4;
    case H(0xdc58f266f7d63f31):
        return g_unk_71729f8150.field_0xf0;
    case H(0x623c6d4a792b6e9e):
        return g_unk_71729f8150.field_0x114;
    case H(0x68624c788859b7ed):
        return g_unk_71729f8150.field_0x118;
    case H(0xa78d35768b1f931d):
        return g_unk_71729f8150.field_0xb4;
    case H(0xacd6741ff195d65b):
        return g_unk_71729f8150.field_0x48;
    case H(0x44bf1239676b321d):
        return g_unk_71729f8150.field_0x138;
    case H(0x48a1edbfa5d9b6b9):
        return g_unk_71729f8150.field_0x4c;
    case H(0x148c9adf961d96e7):
        return g_unk_71729f8150.field_0x50;
    case H(0x15e41fda9a98a3e3):
        return g_unk_71729f8150.field_0xcc;
    case H(0x7abd3a64066717d1):
        return g_unk_71729f8150.field_0xdc;
    case H(0x7b2b35e1d6e28c0d):
        return g_unk_71729f8150.field_0xf8;
    case H(0x80e5c78135927538):
        return g_unk_71729f8150.field_0x78;
    case H(0x1edcd8dd64831d2d):
        return g_unk_71729f8150.field_0x80;
    case H(0xb29470d4d04f2156):
        return g_unk_71729f8150.field_0x6c;
    case H(0x4c912d140200e15e):
        return g_unk_71729f8150.field_0x13c;
    case H(0x9ea8f9c8cbc22e0c):
        return g_unk_71729f8150.field_0x88;
    case H(0x3e914e0106c37eba):
        return g_unk_71729f8150.field_0x9c;
    case H(0xe83b8f19656f571c):
        return g_unk_71729f8150.field_0xec;
    case H(0x69384f46d223aac6):
        return g_unk_71729f8150.field_0x7c;
    case H(0x8b962cd325e3ae73):
        return g_unk_71729f8150.field_0x70;
    case H(0x2d464838914ea893):
        return g_unk_71729f8150.field_0x100;
    case H(0xc931d198c502c871):  // "streams/music/S_Boss_Commander_002.wav"
        return g_unk_71729f8150.field_0x104;
    case H(0x5bfc15cc89737cfb):
        return g_unk_71729f8150.field_0x5c;
    case H(0xa761facd0ab836db):
        return g_unk_71729f8150.field_0x110;
    case H(0x4316636d5ef45639):
        return g_unk_71729f8150.field_0x10c;
    case H(0x5f7ac761177f97b):
        return g_unk_71729f8150.field_0x12c;
    case H(0x7712fcf1a3d46e49):
        return g_unk_71729f8150.field_0x58;
    case H(0x870cb743cdc2d933):
        return g_unk_71729f8150.field_0xac;
    case H(0x293a4e284df5e8da):
        return g_unk_71729f8150.field_0x68;
    case H(0xbf20eabfa12ca427):
        return g_unk_71729f8150.field_0xc4;
    case H(0x4ea5bc31340309ea):
        return g_unk_71729f8150.field_0xc8;
    case H(0xa0f61193bf79d853):
        return g_unk_71729f8150.field_0xbc;
    case H(0x3f0d4ec509e77f9f):
        return g_unk_71729f8150.field_0x28;
    case H(0xf185bdd90259e31e):
        return g_unk_71729f8150.field_0x124;
    }
    return -2.0f;
}

// Sets the stream's override unless it is already set (not negative).
void fn_7101019158(const UnkId_71010182fc& id, f32 volume) {
    switch (id.data->hash) {
    case H(0x75f5fb3459786511):  // "streams/music/S_Boss_Commander_Outro.wav"
        if (g_unk_71729f8150.field_0x128 < 0.0f)
            g_unk_71729f8150.field_0x128 = volume;
        break;
    case H(0x9e34aeaa2350be9e):
        if (g_unk_71729f8150.field_0xfc < 0.0f)
            g_unk_71729f8150.field_0xfc = volume;
        break;
    case H(0x3a71a7dc0e991e1f):
        if (g_unk_71729f8150.field_0x44 < 0.0f)
            g_unk_71729f8150.field_0x44 = volume;
        break;
    case H(0xe36526bd337a31e6):
        if (g_unk_71729f8150.field_0x8c < 0.0f)
            g_unk_71729f8150.field_0x8c = volume;
        break;
    case H(0x691bd2a7b156f942):
        if (g_unk_71729f8150.field_0x11c < 0.0f)
            g_unk_71729f8150.field_0x11c = volume;
        break;
    case H(0xb134d51505a260d9):
        if (g_unk_71729f8150.field_0xd0 < 0.0f)
            g_unk_71729f8150.field_0xd0 = volume;
        break;
    case H(0x4b5f7595dbbb28fe):
        if (g_unk_71729f8150.field_0x38 < 0.0f)
            g_unk_71729f8150.field_0x38 = volume;
        break;
    case H(0x1bbb0ce1a2b7a3b1):
        if (g_unk_71729f8150.field_0x54 < 0.0f)
            g_unk_71729f8150.field_0x54 = volume;
        break;
    case H(0x7c3b93a7db9de9f5):
        if (g_unk_71729f8150.field_0x84 < 0.0f)
            g_unk_71729f8150.field_0x84 = volume;
        break;
    case H(0x86150daa70277d3f):
        if (g_unk_71729f8150.field_0x24 < 0.0f)
            g_unk_71729f8150.field_0x24 = volume;
        break;
    case H(0x23297a80946bbd18):
        if (g_unk_71729f8150.field_0x64 < 0.0f)
            g_unk_71729f8150.field_0x64 = volume;
        break;
    case H(0xbba022ab4e20bb49):
        if (g_unk_71729f8150.field_0xa0 < 0.0f)
            g_unk_71729f8150.field_0xa0 = volume;
        break;
    case H(0x4dce4a256ba177e4):
        if (g_unk_71729f8150.field_0xe8 < 0.0f)
            g_unk_71729f8150.field_0xe8 = volume;
        break;
    case H(0x9f5cac4182aeecef):
        if (g_unk_71729f8150.field_0xf4 < 0.0f)
            g_unk_71729f8150.field_0xf4 = volume;
        break;
    case H(0x3ef1422d237584fc):
        if (g_unk_71729f8150.field_0xd8 < 0.0f)
            g_unk_71729f8150.field_0xd8 = volume;
        break;
    case H(0xf0d0791d7fe35abf):
        if (g_unk_71729f8150.field_0x3c < 0.0f)
            g_unk_71729f8150.field_0x3c = volume;
        break;
    case H(0x6fe1b57371afce91):
        if (g_unk_71729f8150.field_0x74 < 0.0f)
            g_unk_71729f8150.field_0x74 = volume;
        break;
    case H(0x9043608088b49ed3):
        if (g_unk_71729f8150.field_0xe4 < 0.0f)
            g_unk_71729f8150.field_0xe4 = volume;
        break;
    case H(0x2e0954c07e7faac0):
        if (g_unk_71729f8150.field_0x144 < 0.0f)
            g_unk_71729f8150.field_0x144 = volume;
        break;
    case H(0xd24fb2b889cbf040):
        if (g_unk_71729f8150.field_0x34 < 0.0f)
            g_unk_71729f8150.field_0x34 = volume;
        break;
    case H(0x60dcc799cf7b3ae9):
        if (g_unk_71729f8150.field_0x30 < 0.0f)
            g_unk_71729f8150.field_0x30 = volume;
        break;
    case H(0xa77b3a0198e83ed8):
        if (g_unk_71729f8150.field_0xb0 < 0.0f)
            g_unk_71729f8150.field_0xb0 = volume;
        break;
    case H(0x43faacd6df53f3ff):
        if (g_unk_71729f8150.field_0xb8 < 0.0f)
            g_unk_71729f8150.field_0xb8 = volume;
        break;
    case H(0x67184b61571cd16):
        if (g_unk_71729f8150.field_0xa8 < 0.0f)
            g_unk_71729f8150.field_0xa8 = volume;
        break;
    case H(0x7ab023c275e8ad20):
        if (g_unk_71729f8150.field_0xa4 < 0.0f)
            g_unk_71729f8150.field_0xa4 = volume;
        break;
    case H(0x88fe54149e3cedc9):
        if (g_unk_71729f8150.field_0x130 < 0.0f)
            g_unk_71729f8150.field_0x130 = volume;
        break;
    case H(0x2af07fe462493254):
        if (g_unk_71729f8150.field_0x94 < 0.0f)
            g_unk_71729f8150.field_0x94 = volume;
        break;
    case H(0xbf8b8c6cdd3f1c19):
        if (g_unk_71729f8150.field_0x60 < 0.0f)
            g_unk_71729f8150.field_0x60 = volume;
        break;
    case H(0x5b57731ff560c4c5):
        if (g_unk_71729f8150.field_0xc0 < 0.0f)
            g_unk_71729f8150.field_0xc0 = volume;
        break;
    case H(0xa47694afa0033c7f):
        if (g_unk_71729f8150.field_0x40 < 0.0f)
            g_unk_71729f8150.field_0x40 = volume;
        break;
    case H(0x3f396b1ce3751812):
        if (g_unk_71729f8150.field_0x98 < 0.0f)
            g_unk_71729f8150.field_0x98 = volume;
        break;
    case H(0xf6a99c5cd5e463e7):
        if (g_unk_71729f8150.field_0x134 < 0.0f)
            g_unk_71729f8150.field_0x134 = volume;
        break;
    case H(0x736b1379d6737307):
        if (g_unk_71729f8150.field_0x120 < 0.0f)
            g_unk_71729f8150.field_0x120 = volume;
        break;
    case H(0x7434f920dcf8fe31):
        if (g_unk_71729f8150.field_0xe0 < 0.0f)
            g_unk_71729f8150.field_0xe0 = volume;
        break;
    case H(0x951ca6f8f6c6e82f):  // "streams/music/S_Boss_Commander_003.wav"
        if (g_unk_71729f8150.field_0x108 < 0.0f)
            g_unk_71729f8150.field_0x108 = volume;
        break;
    case H(0x9e257955453d1f56):
        if (g_unk_71729f8150.field_0x2c < 0.0f)
            g_unk_71729f8150.field_0x2c = volume;
        break;
    case H(0x31da696d656e50df):
        if (g_unk_71729f8150.field_0x140 < 0.0f)
            g_unk_71729f8150.field_0x140 = volume;
        break;
    case H(0x38826bc675809fce):
        if (g_unk_71729f8150.field_0x90 < 0.0f)
            g_unk_71729f8150.field_0x90 = volume;
        break;
    case H(0xda86db8d7739e41e):
        if (g_unk_71729f8150.field_0xd4 < 0.0f)
            g_unk_71729f8150.field_0xd4 = volume;
        break;
    case H(0xdc58f266f7d63f31):
        if (g_unk_71729f8150.field_0xf0 < 0.0f)
            g_unk_71729f8150.field_0xf0 = volume;
        break;
    case H(0x623c6d4a792b6e9e):
        if (g_unk_71729f8150.field_0x114 < 0.0f)
            g_unk_71729f8150.field_0x114 = volume;
        break;
    case H(0x68624c788859b7ed):
        if (g_unk_71729f8150.field_0x118 < 0.0f)
            g_unk_71729f8150.field_0x118 = volume;
        break;
    case H(0xa78d35768b1f931d):
        if (g_unk_71729f8150.field_0xb4 < 0.0f)
            g_unk_71729f8150.field_0xb4 = volume;
        break;
    case H(0xacd6741ff195d65b):
        if (g_unk_71729f8150.field_0x48 < 0.0f)
            g_unk_71729f8150.field_0x48 = volume;
        break;
    case H(0x44bf1239676b321d):
        if (g_unk_71729f8150.field_0x138 < 0.0f)
            g_unk_71729f8150.field_0x138 = volume;
        break;
    case H(0x48a1edbfa5d9b6b9):
        if (g_unk_71729f8150.field_0x4c < 0.0f)
            g_unk_71729f8150.field_0x4c = volume;
        break;
    case H(0x148c9adf961d96e7):
        if (g_unk_71729f8150.field_0x50 < 0.0f)
            g_unk_71729f8150.field_0x50 = volume;
        break;
    case H(0x15e41fda9a98a3e3):
        if (g_unk_71729f8150.field_0xcc < 0.0f)
            g_unk_71729f8150.field_0xcc = volume;
        break;
    case H(0x7abd3a64066717d1):
        if (g_unk_71729f8150.field_0xdc < 0.0f)
            g_unk_71729f8150.field_0xdc = volume;
        break;
    case H(0x7b2b35e1d6e28c0d):
        if (g_unk_71729f8150.field_0xf8 < 0.0f)
            g_unk_71729f8150.field_0xf8 = volume;
        break;
    case H(0x80e5c78135927538):
        if (g_unk_71729f8150.field_0x78 < 0.0f)
            g_unk_71729f8150.field_0x78 = volume;
        break;
    case H(0x1edcd8dd64831d2d):
        if (g_unk_71729f8150.field_0x80 < 0.0f)
            g_unk_71729f8150.field_0x80 = volume;
        break;
    case H(0xb29470d4d04f2156):
        if (g_unk_71729f8150.field_0x6c < 0.0f)
            g_unk_71729f8150.field_0x6c = volume;
        break;
    case H(0x4c912d140200e15e):
        if (g_unk_71729f8150.field_0x13c < 0.0f)
            g_unk_71729f8150.field_0x13c = volume;
        break;
    case H(0x9ea8f9c8cbc22e0c):
        if (g_unk_71729f8150.field_0x88 < 0.0f)
            g_unk_71729f8150.field_0x88 = volume;
        break;
    case H(0x3e914e0106c37eba):
        if (g_unk_71729f8150.field_0x9c < 0.0f)
            g_unk_71729f8150.field_0x9c = volume;
        break;
    case H(0xe83b8f19656f571c):
        if (g_unk_71729f8150.field_0xec < 0.0f)
            g_unk_71729f8150.field_0xec = volume;
        break;
    case H(0x69384f46d223aac6):
        if (g_unk_71729f8150.field_0x7c < 0.0f)
            g_unk_71729f8150.field_0x7c = volume;
        break;
    case H(0x8b962cd325e3ae73):
        if (g_unk_71729f8150.field_0x70 < 0.0f)
            g_unk_71729f8150.field_0x70 = volume;
        break;
    case H(0x2d464838914ea893):
        if (g_unk_71729f8150.field_0x100 < 0.0f)
            g_unk_71729f8150.field_0x100 = volume;
        break;
    case H(0xc931d198c502c871):  // "streams/music/S_Boss_Commander_002.wav"
        if (g_unk_71729f8150.field_0x104 < 0.0f)
            g_unk_71729f8150.field_0x104 = volume;
        break;
    case H(0x5bfc15cc89737cfb):
        if (g_unk_71729f8150.field_0x5c < 0.0f)
            g_unk_71729f8150.field_0x5c = volume;
        break;
    case H(0xa761facd0ab836db):
        if (g_unk_71729f8150.field_0x110 < 0.0f)
            g_unk_71729f8150.field_0x110 = volume;
        break;
    case H(0x4316636d5ef45639):
        if (g_unk_71729f8150.field_0x10c < 0.0f)
            g_unk_71729f8150.field_0x10c = volume;
        break;
    case H(0x5f7ac761177f97b):
        if (g_unk_71729f8150.field_0x12c < 0.0f)
            g_unk_71729f8150.field_0x12c = volume;
        break;
    case H(0x7712fcf1a3d46e49):
        if (g_unk_71729f8150.field_0x58 < 0.0f)
            g_unk_71729f8150.field_0x58 = volume;
        break;
    case H(0x870cb743cdc2d933):
        if (g_unk_71729f8150.field_0xac < 0.0f)
            g_unk_71729f8150.field_0xac = volume;
        break;
    case H(0x293a4e284df5e8da):
        if (g_unk_71729f8150.field_0x68 < 0.0f)
            g_unk_71729f8150.field_0x68 = volume;
        break;
    case H(0xbf20eabfa12ca427):
        if (g_unk_71729f8150.field_0xc4 < 0.0f)
            g_unk_71729f8150.field_0xc4 = volume;
        break;
    case H(0x4ea5bc31340309ea):
        if (g_unk_71729f8150.field_0xc8 < 0.0f)
            g_unk_71729f8150.field_0xc8 = volume;
        break;
    case H(0xa0f61193bf79d853):
        if (g_unk_71729f8150.field_0xbc < 0.0f)
            g_unk_71729f8150.field_0xbc = volume;
        break;
    case H(0x3f0d4ec509e77f9f):
        if (g_unk_71729f8150.field_0x28 < 0.0f)
            g_unk_71729f8150.field_0x28 = volume;
        break;
    case H(0xf185bdd90259e31e):
        if (g_unk_71729f8150.field_0x124 < 0.0f)
            g_unk_71729f8150.field_0x124 = volume;
        break;
    }
}
