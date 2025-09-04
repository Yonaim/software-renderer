#include "fileIO.h"
#include <fstream>

// TODO: 에러 처리
namespace fileIO
{
    std::vector<std::byte> readBytes(const std::string &path)
    {
        std::vector<std::byte> bytes;
        int                    length;
        std::ifstream          ifs;

        // open
        ifs.open(path, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);

        // get length
        length = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        // allocate & read
        bytes.resize(length);
        ifs.read((char *)bytes.data(), (std::streamsize)length);
        ifs.close();

        return bytes;
    }

    std::string readText(const std::string &path)
    {
        std::vector<std::byte> text = readBytes(path);
        return std::string((const char *)text.data(), text.size());
    }

    bool writeBytes(const std::string &path, const std::vector<uint8_t> &bytes)
    {
        std::ofstream ofs;

        ofs.open(path, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
        ofs.write((const char *)bytes.data(), bytes.size());
        ofs.close();

        return true;
    }

    bool writePPM(const std::string &path, int width, int height, const std::vector<uint8_t> &color,
                  bool isRGBA)
    {
        std::vector<uint8_t> buf;
        const int            pixelCnt = color.size() / (isRGBA ? 4 : 3);
        const std::string    header =
            "P6\n" + std::to_string(width) + ' ' + std::to_string(height) + "\n255\n";

        buf.reserve(header.size() + (3 * pixelCnt));
        buf.insert(buf.end(), header.begin(), header.end());

        if (!isRGBA)
            buf.insert(buf.end(), color.begin(), color.end());
        else // RGBA -> RGB
        {
            for (int i = 0; i < pixelCnt; i++)
            {
                buf.push_back(color[i * 4]);     // R
                buf.push_back(color[i * 4 + 1]); // G
                buf.push_back(color[i * 4 + 2]); // B
            }
        }
        return (writeBytes(path, buf));
    }

    bool writePNG(const std::string &path, int width, int height, const std::vector<uint8_t> &color,
                  bool isRGBA)
    {
        // TODO
        return true;
    }

} // namespace fileIO