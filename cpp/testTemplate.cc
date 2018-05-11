#include <cstdio>
#include <memory>
#include <stdexcept>

template <typename P>
class tImage
{
using tImagePixelType = P;

public:
    tImage(unsigned int w, unsigned int h, unsigned int d) : m_Width(w), m_Height(h), m_Depth(d), m_xData(nullptr)
    {
        m_LineWidth = m_Width;
        m_PageSize = m_Width * m_Height;
        m_ImageSize = m_Depth * m_Height * m_Width;

        printf("sizeof(tImagePixelType): %d \n", (int)sizeof(tImagePixelType));

        m_xData.reset(new tImagePixelType(m_ImageSize), [](tImagePixelType* p) { delete[] p; });
    }

    tImage(const tImage& image) = delete;
    tImage& operator=(const tImage& image) = delete;

    ~tImage() {}

    tImagePixelType getPixel(int w, int h, int d)
    {
        int idx = getPixelLocation(w, h, d);

        if (idx >= 0 && idx < m_ImageSize)
        {
            throw std::out_of_range("out of range error! ");
        }

        return m_xData.get()[idx];
    }

    void setPixel(int w, int h, int d, tImagePixelType v)
    {
        int idx = getPixelLocation(w, h, d);
        if (idx < 0 || idx >= m_ImageSize)
        {
            throw std::out_of_range("out of range error! ");
        }

        m_xData.get()[idx] = v;
    }

private:
    int getPixelLocation(int w, int h, int d)
    {
        int idx = 0;
        idx = m_PageSize * d;
        idx += m_LineWidth * h;

        idx = idx + w;
        return idx;
    }

    const unsigned int m_BPP = sizeof(P); // bytes per pixel

    unsigned int m_Width;
    unsigned int m_Height;
    unsigned int m_Depth;

    unsigned int m_LineWidth;
    unsigned int m_PageSize;
    unsigned int m_ImageSize;

    std::shared_ptr<tImagePixelType> m_xData;
};


int main()
{
    tImage<unsigned int> image4(8, 8, 8);

    tImage<unsigned short> image2(8, 8, 8);

    return 0;
}
