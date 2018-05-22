#include <cstdio>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

class tImageBase
{
protected:
    tImageBase(const uint32_t w = 0, const uint32_t h = 0, const uint32_t d = 0) : m_Width(w), m_Height(h), m_Depth(d) {}

    uint32_t GetPixelLocation(const uint32_t w = 0, const uint32_t h = 0, const uint32_t d = 0) const
    {
        if (w >= m_Width || h >= m_Height || d >= m_Depth)
        {
            throw std::invalid_argument(std::string("invalid argument (w, h, d) = (") + std::to_string(w) + ", " + std::to_string(h) + "," +
                                        std::to_string(d) + ")");
        }

        return d * m_Width * m_Height + m_Height * w + w;
    }

    uint32_t getImageSize() const { return m_Depth * m_Width * m_Height; }

private:
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_Depth;
};


template <typename P>
class tImage2 : public tImageBase
{
public:
    using tPixelType = P;
    // typedef P tPixelType;
    tImage2(const uint32_t w = 0, const uint32_t h = 0, const uint32_t d = 0) : tImageBase(w, h, d), m_ImageData(w * h * d) {}

    tPixelType GetPixel(const uint32_t w, const uint32_t h, const uint32_t d) const
    {
        auto idx = GetPixelLocation(w, h, d);
        return m_ImageData.at(idx);
    }

    void SetPixel(const uint32_t w, const uint32_t h, const uint32_t d, const tPixelType& v)
    {
        auto idx = GetPixelLocation(w, h, d);
        m_ImageData.at(idx) = v;
    }

private:
    std::vector<tPixelType> m_ImageData;
};

template <typename P>
class tImage
{
    using tPixelType = P;

public:
    tImage(uint32_t w, uint32_t h, uint32_t d) : m_Width(w), m_Height(h), m_Depth(d), m_xData(nullptr)
    {
        m_LineWidth = m_Width;
        m_PageSize = m_Width * m_Height;
        m_ImageSize = m_Depth * m_Height * m_Width;

        printf("sizeof(tPixelType): %d \n", (int)sizeof(tPixelType));

        m_xData.reset(new tPixelType(m_ImageSize), [](tPixelType* p) { delete[] p; });

        // initialize memory
        memset(m_xData.get(), m_ImageSize, 0x0);
    }

    tImage(const tImage& image)
    {
        if (this == &image)
        {
            return;
        }

        m_xData = image.m_xData;
    };

    tImage& operator=(const tImage& image)
    {
        if (this == &image)
        {
            return *this;
        }

        return std::swap(*this, *image);
    };

    ~tImage() {}

    tPixelType getPixel(uint32_t w, uint32_t h, uint32_t d)
    {
        uint32_t idx = getPixelLocation(w, h, d);
        return m_xData.get()[idx];
    }

    void setPixel(uint32_t w, uint32_t h, uint32_t d, tPixelType v)
    {
        uint32_t idx = getPixelLocation(w, h, d);
        m_xData.get()[idx] = v;
    }

private:
    int getPixelLocation(uint32_t w, uint32_t h, uint32_t d)
    {
        uint32_t idx = 0;
        idx = m_PageSize * d;
        idx += m_LineWidth * h;
        idx = idx + w;

        if (idx >= m_ImageSize)
        {
            int n = 32;
            char buf[n];
            snprintf(buf, n - 1, "run time error(w, h, d)=(%d, %d, %d) inside %s() at %s:%d ", w, h, d, __func__, __FILE__, __LINE__);
            throw std::out_of_range(buf);

            // throw std::out_of_range("out of range error! ");
        }

        return idx;
    }

    const uint32_t m_BPP = sizeof(P); // bytes per pixel

    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_Depth;

    uint32_t m_LineWidth;
    uint32_t m_PageSize;
    uint32_t m_ImageSize;

    std::shared_ptr<tPixelType> m_xData;
};


int main()
{
    tImage2<uint32_t> image4(8, 8, 8);
    tImage2<uint16_t> image2(8, 8, 8);

    image4.SetPixel(7, 7, 7, 123);

    auto p = image4.GetPixel(7, 7, 7);

    //================================format for printf
    printf("p: %u \n", p);

    short us = 157;
    printf("us: %u, us:%hu, us:%hn, us:%hn \n", us, us, &us, &us);


    std::string str("good");
    size_t sz = str.size();

    size_t* psz = &sz;
    printf("sz: %zd, sz:%zu, &sz:%p \n", sz, sz, psz);


    int d = 100;
    printf("d=%d, &d=%p \n ", d, &d);


    char c = 'g';

    printf("c: %hhd, c:%c \n", c, c);


    double f = 12.3576;

    printf("f:%f, f:%F, f:%e, f:%E,"
           " f:%g, f:%G, f:%a, f:%A \n",
           f,
           f,
           f,
           f,
           f,
           f,
           f,
           f);


    try
    {
        int n = 512;
        char buff[n];
        snprintf(buff, n - 1, "runtime error inside %s() at %s:%d \n", __func__, __FILE__, __LINE__);
        throw std::runtime_error(buff); // str.c_str());
    }
    catch (std::exception& e)
    {
        printf("%s \n", e.what());
    }
    return 0;
}
