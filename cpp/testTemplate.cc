#include <cstdio>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <string>



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

    tImage(const tImage& image) = delete;
    tImage& operator=(const tImage& image) = delete;

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
            throw std::out_of_range("out of range error! ");
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
    tImage<uint32_t> image4(8, 8, 8);
    tImage<uint16_t> image2(8, 8, 8);

    image4.setPixel(7, 7, 7, 123);
    auto p = image4.getPixel(7, 7, 7);

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
    {   int n = 512;
        char buff[n];
        snprintf(buff, n-1,  "runtime error inside %s() at %s:%d \n", __func__, __FILE__, __LINE__);
        throw std::runtime_error(buff); //str.c_str());
    }
    catch (std::exception& e)
    {
        printf("%s \n", e.what());
    }
    return 0;
}
