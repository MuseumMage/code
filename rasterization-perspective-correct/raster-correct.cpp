// compile with:
// c++ -o raster3d raster3d.cpp
// for naive vertex attribute interpolation and:
// c++ -o raster3d raster3d.cpp -D PERSP_CORRECT
// for perspective correct interpolation

// (c) www.scratchapixel.com

#include <fstream>
#include <cstdint>
#include <cstring>

#define PERSP_CORRECT

typedef float Vec2[2];
typedef float Vec3[3];
typedef unsigned char Rgb[3];

inline
float edgeFunction(const Vec3 &a, const Vec3 &b, const Vec3 &c)
{ return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]); }

int main(int argc, char **argv)
{
    Vec3 v2 = { -48, -10,  82};
    Vec3 v1 = {  29, -15,  44};
    Vec3 v0 = {  13,  34, 114};
    Vec3 c2 = {1, 0, 0};
    Vec3 c1 = {0, 1, 0};
    Vec3 c0 = {0, 0, 1};

    const uint32_t w = 512;
    const uint32_t h = 512;

    // project triangle onto the screen
    v0[0] /= v0[2], v0[1] /= v0[2];
    v1[0] /= v1[2], v1[1] /= v1[2];
    v2[0] /= v2[2], v2[1] /= v2[2];
    // convert from screen space to NDC then raster (in one go)
    v0[0] = (1 + v0[0]) * 0.5f * w, v0[1] = (1 + v0[1]) * 0.5f * h;
    v1[0] = (1 + v1[0]) * 0.5f * w, v1[1] = (1 + v1[1]) * 0.5f * h;
    v2[0] = (1 + v2[0]) * 0.5f * w, v2[1] = (1 + v2[1]) * 0.5f * h;

#ifdef PERSP_CORRECT
    // divide vertex-attribute by the vertex z-coordinate
    c0[0] /= v0[2], c0[1] /= v0[2], c0[2] /= v0[2];
    c1[0] /= v1[2], c1[1] /= v1[2], c1[2] /= v1[2];
    c2[0] /= v2[2], c2[1] /= v2[2], c2[2] /= v2[2];
    // pre-compute 1 over z
    v0[2] = 1 / v0[2], v1[2] = 1 / v1[2], v2[2] = 1 / v2[2];
#endif

    Rgb *framebuffer = new Rgb[w * h];
    memset(framebuffer, 0x0, w * h * 3);

    float area = edgeFunction(v0, v1, v2);

    for (uint32_t j = 0; j < h; ++j) {
        for (uint32_t i = 0; i < w; ++i) {
            Vec3 p = {i + 0.5f, h - j + 0.5f, 0};
            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                float lambda_0 = w0 / area;
                float lambda_1 = w1 / area;
                float lambda_2 = w2 / area;
                float r = lambda_0 * c0[0] + lambda_1 * c1[0] + lambda_2 * c2[0];
                float g = lambda_0 * c0[1] + lambda_1 * c1[1] + lambda_2 * c2[1];
                float b = lambda_0 * c0[2] + lambda_1 * c1[2] + lambda_2 * c2[2];
#ifdef PERSP_CORRECT
                float z = 1 / (lambda_0 * v0[2] + lambda_1 * v1[2] + lambda_2 * v2[2]);
                // if we use perspective-correct interpolation we need to
                // multiply the result of this interpolation by z, the depth
                // of the point on the 3D triangle that the pixel overlaps.
                r *= z, g *= z, b *= z;
#endif
                framebuffer[j * w + i][0] = (unsigned char)(r * 255);
                framebuffer[j * w + i][1] = (unsigned char)(g * 255);
                framebuffer[j * w + i][2] = (unsigned char)(b * 255);
            }
        }
    }

    std::ofstream ofs;
    ofs.open("./raster-correct.ppm");
    ofs << "P6\n" << w << " " << h << "\n255\n";
    ofs.write((char*)framebuffer, w * h * 3);
    ofs.close();

    delete [] framebuffer;

    return 0;
}