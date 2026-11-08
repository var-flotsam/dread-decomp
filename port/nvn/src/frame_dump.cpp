// The frame dump: the window has no Vulkan surface yet (window.cpp), so to see what the game draws,
// every N-th presented texture is read back and written as a PNG.
//
//   DREAD_NVN_FRAME_DUMP=<dir>    turns it on; frames go to <dir>/frame_<present number>.png
//   DREAD_NVN_FRAME_EVERY=<n>     every n-th present (default 60, one a second at 60 Hz)
//   DREAD_NVN_FRAME_MAX=<n>       at most n files (default 100)
//   DREAD_NVN_DUMP_TARGETS=1      also, beside each frame, the draws since the previous one by program
//                                 and target (frame_<n>.txt) and their colour targets (_target<k>.png)
//
// The PNG is RGB, 8 bits per channel (the presented alpha is dropped: the game does not keep it
// meaningful), deflated with stored blocks only, so nothing beyond the C++ library is needed. The
// read-back waits for everything submitted (ReadTexels), so a dump slows the frames it takes. The
// images are the game's output, derived from the user's dump: they stay under build/ or wherever
// the user points the directory, never in the repository.

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "nvn_shim/texture.h"
#include "objects.h"

namespace dread::nvn {

namespace {

struct DumpConfig {
    bool on = false;
    std::string dir;
    std::uint64_t every = 60;
    std::uint64_t max = 100;
};

const DumpConfig& Config() {
    static const DumpConfig config = [] {
        DumpConfig c;
        if (const char* d = std::getenv("DREAD_NVN_FRAME_DUMP"); d && *d) {
            c.on = true;
            c.dir = d;
        }
        if (const char* e = std::getenv("DREAD_NVN_FRAME_EVERY"))
            c.every = std::max<std::uint64_t>(1, std::strtoull(e, nullptr, 10));
        if (const char* m = std::getenv("DREAD_NVN_FRAME_MAX"))
            c.max = std::strtoull(m, nullptr, 10);
        return c;
    }();
    return config;
}

std::uint32_t Crc32(const std::uint8_t* p, std::size_t n, std::uint32_t crc = 0) {
    static const std::array<std::uint32_t, 256> table = [] {
        std::array<std::uint32_t, 256> t{};
        for (std::uint32_t i = 0; i < 256; ++i) {
            std::uint32_t c = i;
            for (int k = 0; k < 8; ++k)
                c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            t[i] = c;
        }
        return t;
    }();
    crc = ~crc;
    for (std::size_t i = 0; i < n; ++i)
        crc = table[(crc ^ p[i]) & 0xff] ^ (crc >> 8);
    return ~crc;
}

void PutU32(std::vector<std::uint8_t>* out, std::uint32_t v) {
    for (int s = 24; s >= 0; s -= 8)
        out->push_back(static_cast<std::uint8_t>(v >> s));
}

void Chunk(std::vector<std::uint8_t>* png, const char* type, const std::vector<std::uint8_t>& data) {
    PutU32(png, static_cast<std::uint32_t>(data.size()));
    const std::size_t start = png->size();
    png->insert(png->end(), type, type + 4);
    png->insert(png->end(), data.begin(), data.end());
    PutU32(png, Crc32(png->data() + start, png->size() - start));
}

// A zlib stream of stored (uncompressed) deflate blocks.
std::vector<std::uint8_t> ZlibStored(const std::vector<std::uint8_t>& raw) {
    std::vector<std::uint8_t> z = {0x78, 0x01};
    std::size_t pos = 0;
    do {
        const std::size_t n = std::min<std::size_t>(65535, raw.size() - pos);
        const bool last = pos + n == raw.size();
        z.push_back(last ? 1 : 0);
        z.push_back(static_cast<std::uint8_t>(n));
        z.push_back(static_cast<std::uint8_t>(n >> 8));
        z.push_back(static_cast<std::uint8_t>(~n));
        z.push_back(static_cast<std::uint8_t>(~n >> 8));
        z.insert(z.end(), raw.begin() + pos, raw.begin() + pos + n);
        pos += n;
    } while (pos < raw.size());
    std::uint32_t a = 1, b = 0;
    for (std::uint8_t c : raw) {
        a = (a + c) % 65521;
        b = (b + a) % 65521;
    }
    PutU32(&z, (b << 16) | a);
    return z;
}

// IEEE 754 binary16 to float (NaN as 0: a dump shows it black).
float HalfToFloat(std::uint16_t h) {
    const int exp = (h >> 10) & 0x1f;
    const int mant = h & 0x3ff;
    float v;
    if (exp == 0)
        v = std::ldexp(static_cast<float>(mant), -24);
    else if (exp == 31)
        v = mant ? 0.0f : INFINITY;
    else
        v = std::ldexp(static_cast<float>(mant | 0x400), exp - 25);
    return (h & 0x8000) ? -v : v;
}

}  // namespace

std::vector<std::uint8_t> detail::EncodePngRgb(const std::uint8_t* rgb, std::uint32_t width, std::uint32_t height) {
    std::vector<std::uint8_t> raw;
    raw.reserve(static_cast<std::size_t>(height) * (width * 3 + 1));
    for (std::uint32_t y = 0; y < height; ++y) {
        raw.push_back(0);  // filter: none
        const std::uint8_t* row = rgb + static_cast<std::size_t>(y) * width * 3;
        raw.insert(raw.end(), row, row + static_cast<std::size_t>(width) * 3);
    }
    std::vector<std::uint8_t> png = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'};
    std::vector<std::uint8_t> ihdr;
    PutU32(&ihdr, width);
    PutU32(&ihdr, height);
    ihdr.insert(ihdr.end(), {8, 2, 0, 0, 0});  // 8 bits, RGB, deflate, no filter method, no interlace
    Chunk(&png, "IHDR", ihdr);
    Chunk(&png, "IDAT", ZlibStored(raw));
    Chunk(&png, "IEND", {});
    return png;
}

bool detail::DumpTexturePng(const void* texture, const std::string& path) {
    const TextureInfo info = GetTextureInfo(texture);
    bool bgr = false, half = false;
    switch (info.format) {
    case VK_FORMAT_R8G8B8A8_UNORM:
    case VK_FORMAT_R8G8B8A8_SRGB:
        break;
    case VK_FORMAT_B8G8R8A8_UNORM:
    case VK_FORMAT_B8G8R8A8_SRGB:
        bgr = true;
        break;
    case VK_FORMAT_R16G16B16A16_SFLOAT:  // written clamped to 0..1, as stored (no tone mapping)
        half = true;
        break;
    default:
        std::fprintf(stderr, "nvn shim: frame dump: %s: the texture's format (Vulkan %d) is not 8-bit RGBA or RGBA16F\n",
                     path.c_str(), static_cast<int>(info.format));
        return false;
    }
    const std::size_t n = static_cast<std::size_t>(info.width) * info.height;
    std::vector<std::uint8_t> texels;
    if (!ReadTexels(texture, &texels) || texels.size() < n * (half ? 8 : 4))
        return false;
    std::vector<std::uint8_t> rgb(n * 3);
    for (std::size_t i = 0; i < n; ++i) {
        for (int ch = 0; ch < 3; ++ch) {
            if (half) {
                std::uint16_t h;
                std::memcpy(&h, &texels[i * 8 + ch * 2], 2);
                const float v = std::clamp(HalfToFloat(h), 0.0f, 1.0f);
                rgb[i * 3 + ch] = static_cast<std::uint8_t>(v * 255.0f + 0.5f);
            } else {
                rgb[i * 3 + ch] = texels[i * 4 + (bgr ? 2 - ch : ch)];
            }
        }
    }
    const std::vector<std::uint8_t> png = EncodePngRgb(rgb.data(), info.width, info.height);
    std::FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) {
        std::fprintf(stderr, "nvn shim: frame dump: cannot write %s\n", path.c_str());
        return false;
    }
    std::fwrite(png.data(), 1, png.size(), f);
    std::fclose(f);
    return true;
}

// The scene dump: since the previous dumped frame, how many draws each program recorded, into which
// targets, and how many were skipped and why. Written as frame_<n>.txt, with each colour target as
// frame_<n>_target<k>.png (8-bit RGBA and RGBA16F; others are listed only).
namespace {

struct SceneDump {
    std::map<std::string, std::uint64_t> programs;          // "<program> -> c0 <k>, depth <k>": draws
    std::map<std::string, std::uint64_t> skipped;           // reason: draws
    std::map<const void*, std::uint64_t> colors, depths;    // target: draws
    std::map<const void*, std::size_t> index;               // target: its number in this dump
};

SceneDump& Scene() {
    static SceneDump scene;
    return scene;
}

std::size_t TargetIndex(const void* target) {
    auto& index = Scene().index;
    return index.emplace(target, index.size()).first->second;
}

void WriteScene(const DumpConfig& c, std::uint64_t present) {
    SceneDump& s = Scene();
    char base[48];
    std::snprintf(base, sizeof base, "/frame_%06llu", static_cast<unsigned long long>(present));
    std::FILE* f = std::fopen((c.dir + base + ".txt").c_str(), "w");
    if (!f)
        return;
    std::fprintf(f, "draws by program (since the previous dumped frame):\n");
    for (const auto& [name, n] : s.programs)
        std::fprintf(f, "  %8llu  %s\n", static_cast<unsigned long long>(n), name.c_str());
    std::fprintf(f, "skipped:\n");
    for (const auto& [why, n] : s.skipped)
        std::fprintf(f, "  %8llu  %s\n", static_cast<unsigned long long>(n), why.c_str());
    std::fprintf(f, "targets:\n");
    for (const auto& [target, k] : s.index) {
        const TextureInfo info = GetTextureInfo(target);
        const auto colour = s.colors.find(target);
        const auto depth = s.depths.find(target);
        std::fprintf(f, "  target%zu %p %ux%u Vulkan format %d: %llu colour draws, %llu depth draws\n", k, target,
                     info.width, info.height, static_cast<int>(info.format),
                     static_cast<unsigned long long>(colour == s.colors.end() ? 0 : colour->second),
                     static_cast<unsigned long long>(depth == s.depths.end() ? 0 : depth->second));
        const bool readable = info.format == VK_FORMAT_R8G8B8A8_UNORM || info.format == VK_FORMAT_R8G8B8A8_SRGB ||
                              info.format == VK_FORMAT_B8G8R8A8_UNORM || info.format == VK_FORMAT_B8G8R8A8_SRGB ||
                              info.format == VK_FORMAT_R16G16B16A16_SFLOAT;
        if (colour != s.colors.end() && readable)
            detail::DumpTexturePng(target, c.dir + base + "_target" + std::to_string(k) + ".png");
    }
    std::fclose(f);
    s = SceneDump{};
}

bool TargetsOn() {
    static const bool on = [] {
        const char* e = std::getenv("DREAD_NVN_DUMP_TARGETS");
        return e && *e && *e != '0';
    }();
    return on;
}

}  // namespace

bool detail::SceneDumpOn() {
    return Config().on && TargetsOn();
}

void detail::SceneDumpNoteDraw(const std::string& program, std::uint32_t color_count, const void* const* colors,
                               const void* depth, const char* skipped) {
    SceneDump& s = Scene();
    if (skipped) {
        ++s.skipped[skipped];
        return;
    }
    std::string key = program + " ->";
    for (std::uint32_t i = 0; i < color_count; ++i) {
        if (!colors[i])
            continue;
        ++s.colors[colors[i]];
        key += " c" + std::to_string(i) + " target" + std::to_string(TargetIndex(colors[i]));
    }
    if (depth) {
        ++s.depths[depth];
        key += " depth target" + std::to_string(TargetIndex(depth));
    }
    ++s.programs[key];
}

void detail::FrameDumpPresented(const void* texture, std::uint64_t present) {
    const DumpConfig& c = Config();
    static std::uint64_t written = 0;
    if (!c.on || !texture || present % c.every != 0 || written >= c.max)
        return;
    char name[32];
    std::snprintf(name, sizeof name, "/frame_%06llu.png", static_cast<unsigned long long>(present));
    if (DumpTexturePng(texture, c.dir + name) && ++written == 1) {
        const TextureInfo info = GetTextureInfo(texture);
        std::fprintf(stderr, "nvn shim: frame dump: %ux%u frames to %s\n", info.width, info.height, c.dir.c_str());
    }
    if (TargetsOn())
        WriteScene(c, present);
}

}  // namespace dread::nvn
