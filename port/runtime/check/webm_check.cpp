// runtime_webm_check: the movie runtime's WebM demuxer (src/webm.cpp) on files this program builds
// in memory: no game data. Exits non-zero on the first failed check.
//   runtime_webm_check

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "../src/webm.h"

namespace {

using Bytes = std::vector<std::uint8_t>;
int g_failed = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++g_failed;
    }
}

void PutId(Bytes* b, std::uint32_t id) {
    int n = id > 0xFFFFFF ? 4 : id > 0xFFFF ? 3 : id > 0xFF ? 2 : 1;
    for (int i = n - 1; i >= 0; --i)
        b->push_back(static_cast<std::uint8_t>(id >> (8 * i)));
}

// An 8-byte size (0x01 marker), or the 1-byte "unknown" size.
void PutSize(Bytes* b, std::uint64_t size, bool unknown = false) {
    if (unknown) {
        b->push_back(0xFF);
        return;
    }
    b->push_back(0x01);
    for (int i = 6; i >= 0; --i)
        b->push_back(static_cast<std::uint8_t>(size >> (8 * i)));
}

Bytes Element(std::uint32_t id, const Bytes& body) {
    Bytes b;
    PutId(&b, id);
    PutSize(&b, body.size());
    b.insert(b.end(), body.begin(), body.end());
    return b;
}

Bytes Uint(std::uint32_t id, std::uint64_t v, int bytes) {
    Bytes body;
    for (int i = bytes - 1; i >= 0; --i)
        body.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
    return Element(id, body);
}

Bytes Str(std::uint32_t id, const char* s) {
    return Element(id, Bytes(s, s + std::strlen(s)));
}

Bytes Float64(std::uint32_t id, double d) {
    std::uint64_t bits;
    std::memcpy(&bits, &d, 8);
    return Uint(id, bits, 8);
}

Bytes Cat(std::initializer_list<Bytes> parts) {
    Bytes out;
    for (const Bytes& p : parts)
        out.insert(out.end(), p.begin(), p.end());
    return out;
}

// A block body: track 1 (vint 0x81), a relative time, flags, the frame.
Bytes BlockBody(std::int16_t rel, std::uint8_t flags, const Bytes& frame) {
    Bytes b{0x81, static_cast<std::uint8_t>(rel >> 8), static_cast<std::uint8_t>(rel), flags};
    b.insert(b.end(), frame.begin(), frame.end());
    return b;
}

Bytes Movie(bool unknown_segment_size) {
    Bytes ebml = Element(0x1A45DFA3, Cat({Str(0x4282, "webm")}));
    Bytes info = Element(0x1549A966, Cat({Uint(0x2AD7B1, 1000000, 3), Float64(0x4489, 2000.0)}));
    Bytes video = Element(0xE0, Cat({Uint(0xB0, 1280, 2), Uint(0xBA, 720, 2)}));
    Bytes track = Element(0xAE, Cat({Uint(0xD7, 1, 1), Uint(0x83, 1, 1), Str(0x86, "V_VP9"),
                                     Uint(0x23E383, 33333333, 4), video}));
    Bytes tracks = Element(0x1654AE6B, track);
    Bytes c1 = Element(0x1F43B675, Cat({Uint(0xE7, 0, 1), Element(0xA3, BlockBody(0, 0x80, {1, 2, 3})),
                                        Element(0xA3, BlockBody(33, 0x00, {4, 5}))}));
    // A BlockGroup with a ReferenceBlock: not a key frame.
    Bytes group = Element(0xA0, Cat({Element(0xA1, BlockBody(0, 0x00, {6})), Uint(0xFB, 1, 1)}));
    Bytes c2 = Element(0x1F43B675, Cat({Uint(0xE7, 1000, 2), group, Element(0xA3, BlockBody(500, 0x80, {7, 8}))}));
    Bytes seg_body = Cat({info, tracks, c1, c2});
    Bytes seg;
    PutId(&seg, 0x18538067);
    PutSize(&seg, seg_body.size(), unknown_segment_size);
    seg.insert(seg.end(), seg_body.begin(), seg_body.end());
    return Cat({ebml, seg});
}

void CheckMovie(bool unknown_size) {
    dread::webm::File f;
    std::string error;
    const bool ok = dread::webm::Parse(Movie(unknown_size), &f, &error);
    Check(ok, "a well-formed file parses");
    if (!ok)
        return;
    Check(f.timecode_scale_ns == 1000000, "timecode scale");
    Check(f.DurationUs() == 2000000, "duration 2 s");
    Check(f.tracks.size() == 1 && f.frames.size() == 1, "one track");
    const dread::webm::Track& t = f.tracks[0];
    Check(t.number == 1 && t.type == 1 && t.codec == "V_VP9", "the track's number, type and codec");
    Check(t.width == 1280 && t.height == 720, "the frame size");
    Check(t.default_duration_ns == 33333333, "the default frame duration");
    const auto& fr = f.frames[0];
    Check(fr.size() == 4, "four frames");
    if (fr.size() != 4)
        return;
    Check(fr[0].time_us == 0 && fr[1].time_us == 33000 && fr[2].time_us == 1000000 && fr[3].time_us == 1500000,
          "frame times from the cluster time and the block offsets");
    Check(fr[0].key && !fr[1].key && !fr[2].key && fr[3].key, "key frames: SimpleBlock flag, BlockGroup reference");
    Check(fr[0].size == 3 && f.bytes[fr[0].offset] == 1 && f.bytes[fr[0].offset + 2] == 3, "frame 0's bytes");
    Check(fr[2].size == 1 && f.bytes[fr[2].offset] == 6, "the grouped block's bytes");
}

}  // namespace

int main() {
    CheckMovie(false);
    CheckMovie(true);
    {
        dread::webm::File f;
        std::string error;
        Check(!dread::webm::Parse(Bytes{0x00, 0x01, 0x02}, &f, &error) && !error.empty(), "garbage is refused");
        Bytes cut = Movie(false);
        cut.resize(40);
        Check(!dread::webm::Parse(cut, &f, &error), "a file cut before its tracks is refused");
    }
    if (g_failed) {
        std::fprintf(stderr, "runtime_webm_check: %d failed\n", g_failed);
        return 1;
    }
    std::printf("runtime_webm_check: all passed\n");
    return 0;
}
