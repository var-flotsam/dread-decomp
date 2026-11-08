// The movie runtime's WebM demuxer (webm.h).

#include "webm.h"

#include <cstring>
#include <utility>

namespace dread::webm {

namespace {

constexpr std::uint64_t kUnknownSize = ~0ull;

// Matroska element ids (with their length marker bits, as they appear in the file).
enum : std::uint32_t {
    kEbml = 0x1A45DFA3,
    kSegment = 0x18538067,
    kInfo = 0x1549A966,
    kTimecodeScale = 0x2AD7B1,
    kDuration = 0x4489,
    kTracks = 0x1654AE6B,
    kTrackEntry = 0xAE,
    kTrackNumber = 0xD7,
    kTrackType = 0x83,
    kCodecId = 0x86,
    kDefaultDuration = 0x23E383,
    kVideo = 0xE0,
    kPixelWidth = 0xB0,
    kPixelHeight = 0xBA,
    kCluster = 0x1F43B675,
    kTimecode = 0xE7,
    kSimpleBlock = 0xA3,
    kBlockGroup = 0xA0,
    kBlock = 0xA1,
};

class Reader {
public:
    Reader(const std::uint8_t* data, std::size_t size) : m_data(data), m_size(size) {}

    // An element id (marker bits kept) or a size (marker bit removed; all ones is "unknown").
    bool VarInt(std::size_t* pos, std::uint64_t* out, bool keep_marker) const {
        if (*pos >= m_size)
            return false;
        std::uint8_t first = m_data[*pos];
        if (first == 0)
            return false;
        int length = 1;
        std::uint8_t mask = 0x80;
        while (!(first & mask)) {
            mask >>= 1;
            ++length;
        }
        if (*pos + length > m_size)
            return false;
        std::uint64_t value = keep_marker ? first : (first & (mask - 1));
        bool all_ones = (first & (mask - 1)) == static_cast<std::uint8_t>(mask - 1);
        for (int i = 1; i < length; ++i) {
            std::uint8_t b = m_data[*pos + i];
            all_ones = all_ones && b == 0xFF;
            value = (value << 8) | b;
        }
        *pos += length;
        *out = !keep_marker && all_ones ? kUnknownSize : value;
        return true;
    }

    std::uint64_t Unsigned(std::size_t pos, std::uint64_t size) const {
        std::uint64_t v = 0;
        for (std::uint64_t i = 0; i < size && i < 8; ++i)
            v = (v << 8) | m_data[pos + i];
        return v;
    }

    double Float(std::size_t pos, std::uint64_t size) const {
        if (size == 4) {
            std::uint32_t bits = static_cast<std::uint32_t>(Unsigned(pos, 4));
            float f;
            std::memcpy(&f, &bits, 4);
            return f;
        }
        if (size == 8) {
            std::uint64_t bits = Unsigned(pos, 8);
            double d;
            std::memcpy(&d, &bits, 8);
            return d;
        }
        return 0;
    }

    std::size_t size() const { return m_size; }
    const std::uint8_t* data() const { return m_data; }

private:
    const std::uint8_t* m_data;
    std::size_t m_size;
};

struct Element {
    std::uint32_t id;
    std::size_t body;  // first byte of the body
    std::size_t end;   // one past the body (the parent's end for an unknown size)
};

bool Next(const Reader& r, std::size_t* pos, std::size_t parent_end, Element* out) {
    std::uint64_t id, size;
    std::size_t p = *pos;
    if (p >= parent_end || !r.VarInt(&p, &id, true) || !r.VarInt(&p, &size, false))
        return false;
    out->id = static_cast<std::uint32_t>(id);
    out->body = p;
    out->end = size == kUnknownSize || p + size > parent_end ? parent_end : p + static_cast<std::size_t>(size);
    *pos = out->end;
    return true;
}

void ParseTrack(const Reader& r, const Element& entry, Track* t) {
    std::size_t pos = entry.body;
    Element e;
    while (Next(r, &pos, entry.end, &e)) {
        std::uint64_t n = e.end - e.body;
        switch (e.id) {
        case kTrackNumber: t->number = r.Unsigned(e.body, n); break;
        case kTrackType: t->type = static_cast<std::uint32_t>(r.Unsigned(e.body, n)); break;
        case kCodecId: t->codec.assign(reinterpret_cast<const char*>(r.data() + e.body), n); break;
        case kDefaultDuration: t->default_duration_ns = r.Unsigned(e.body, n); break;
        case kVideo: {
            std::size_t vp = e.body;
            Element v;
            while (Next(r, &vp, e.end, &v)) {
                if (v.id == kPixelWidth)
                    t->width = static_cast<std::uint32_t>(r.Unsigned(v.body, v.end - v.body));
                else if (v.id == kPixelHeight)
                    t->height = static_cast<std::uint32_t>(r.Unsigned(v.body, v.end - v.body));
            }
            break;
        }
        default: break;
        }
    }
    // The codec id keeps no trailing NULs.
    while (!t->codec.empty() && t->codec.back() == '\0')
        t->codec.pop_back();
}

// A Block or SimpleBlock body: track number, a 16-bit signed time relative to the cluster, flags,
// then the frame (no lacing).
void AddBlock(const Reader& r, File* f, std::size_t body, std::size_t end, std::int64_t cluster_time,
              bool simple, bool key_from_group) {
    std::size_t p = body;
    std::uint64_t track;
    if (!r.VarInt(&p, &track, false) || p + 3 > end)
        return;
    std::int16_t rel = static_cast<std::int16_t>((r.data()[p] << 8) | r.data()[p + 1]);
    std::uint8_t flags = r.data()[p + 2];
    p += 3;
    if (flags & 0x06)  // laced: not used by VP9 video, and not read here
        return;
    for (std::size_t i = 0; i < f->tracks.size(); ++i) {
        if (f->tracks[i].number != track)
            continue;
        Frame fr;
        fr.offset = p;
        fr.size = end - p;
        double ns = static_cast<double>(cluster_time + rel) * static_cast<double>(f->timecode_scale_ns);
        fr.time_us = static_cast<std::int64_t>(ns / 1000.0);
        fr.key = simple ? (flags & 0x80) != 0 : key_from_group;
        f->frames[i].push_back(fr);
        return;
    }
}

}  // namespace

bool Parse(std::vector<std::uint8_t> bytes, File* out, std::string* error) {
    *out = File{};
    out->bytes = std::move(bytes);
    Reader r(out->bytes.data(), out->bytes.size());
    std::size_t pos = 0;
    Element top;
    if (!Next(r, &pos, r.size(), &top) || top.id != kEbml) {
        *error = "not an EBML file";
        return false;
    }
    Element segment;
    if (!Next(r, &pos, r.size(), &segment) || segment.id != kSegment) {
        *error = "no Segment after the EBML header";
        return false;
    }
    std::size_t sp = segment.body;
    Element e;
    while (Next(r, &sp, segment.end, &e)) {
        if (e.id == kInfo) {
            std::size_t ip = e.body;
            Element i;
            while (Next(r, &ip, e.end, &i)) {
                if (i.id == kTimecodeScale)
                    out->timecode_scale_ns = r.Unsigned(i.body, i.end - i.body);
                else if (i.id == kDuration)
                    out->duration_ticks = r.Float(i.body, i.end - i.body);
            }
        } else if (e.id == kTracks) {
            std::size_t tp = e.body;
            Element t;
            while (Next(r, &tp, e.end, &t)) {
                if (t.id != kTrackEntry)
                    continue;
                out->tracks.emplace_back();
                ParseTrack(r, t, &out->tracks.back());
            }
            out->frames.resize(out->tracks.size());
        } else if (e.id == kCluster) {
            std::int64_t cluster_time = 0;
            std::size_t cp = e.body;
            Element c;
            while (Next(r, &cp, e.end, &c)) {
                if (c.id == kTimecode) {
                    cluster_time = static_cast<std::int64_t>(r.Unsigned(c.body, c.end - c.body));
                } else if (c.id == kSimpleBlock) {
                    AddBlock(r, out, c.body, c.end, cluster_time, true, false);
                } else if (c.id == kBlockGroup) {
                    // A Block in a group is a key frame when the group has no ReferenceBlock (0xFB).
                    std::size_t gp = c.body;
                    Element g;
                    std::size_t block_body = 0, block_end = 0;
                    bool key = true;
                    while (Next(r, &gp, c.end, &g)) {
                        if (g.id == kBlock) {
                            block_body = g.body;
                            block_end = g.end;
                        } else if (g.id == 0xFB) {
                            key = false;
                        }
                    }
                    if (block_end)
                        AddBlock(r, out, block_body, block_end, cluster_time, false, key);
                }
            }
        }
    }
    if (out->tracks.empty()) {
        *error = "no tracks";
        return false;
    }
    return true;
}

}  // namespace dread::webm
