#include <cmath>

template <typename T>
class Vector3 {
public:
    T x = 0;
    T y = 0;
    T z = 0;

    Vector3() = default;

    Vector3(const T& x, const T& y, const T& z):
    x(x),
    y(y),
    z(z) {}

    Vector3 operator+(const Vector3& vec) const {
        return Vector3(x + vec.x, y + vec.y, z + vec.z);
    }

    T operator*(const Vector3& vec) const {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    Vector3 operator-(const Vector3& vec) const {
        return Vector3(x - vec.x, y - vec.y, z - vec.z);
    }

    Vector3& operator-() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    double len() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    double operator^(const Vector3& vec) const {
        return std::acos((*this * vec) / (len() * vec.len()));
    }

    double cos(const Vector3& vec) const {
        return (*this * vec) / (len() * vec.len());
    }

    Vector3& operator=(const Vector3& color) {
        x = color.x;
        y = color.y;
        z = color.z;

        return *this;
    }
};

class Color: public Vector3<unsigned char> {
public:
    Color(): Vector3<unsigned char>() {}

    Color(unsigned char r, unsigned char g, unsigned char b):
    Vector3<unsigned char>(r, g, b) {}

    Color(const Vector3<unsigned char>& vec):
    Vector3<unsigned char>(vec) {}

    Color operator*(const Color& color) {
        return Color(x * color.x / 255, y * color.y / 255, z * color.z / 255);
    }

    Color operator+(const Color& color) {
        Color newColor;
        newColor.x = std::min(255, x + color.x);
        newColor.y = std::min(255, y + color.y);
        newColor.z = std::min(255, z + color.z);

        return newColor;
    }

//    Color& operator=(const Color& color) {
//        x = color.x;
//        y = color.y;
//        z = color.z;
//
//        return *this;
//    }

    sf::Color operator*() const {
        return sf::Color(x, y, z);
    }

    Color operator*(const double intensity) {
        return Color(x * intensity, y * intensity, z * intensity);
    }
};

Color operator*(double intensity, const Color& color) {
    return Color(color.x * intensity, color.y * intensity, color.z * intensity);
}


template <typename T>
class CoordinateSystem3 {
private:
    Vector3<T> zeroPoint;

public:
    CoordinateSystem3(T x, T y, T z):
    zeroPoint(x, y, z) {}

    CoordinateSystem3(const CoordinateSystem3& coordSys):
    zeroPoint(coordSys.zeroPoint.x, coordSys.zeroPoint.y, coordSys.zeroPoint.z) {}

    explicit CoordinateSystem3(const Vector3<T>& vec):
    zeroPoint(vec) {}

    Vector3<T> makeVec(T x, T y, T z) const {
        return Vector3(x - zeroPoint.x, zeroPoint.y - y, z - zeroPoint.z);
    }

    Vector3<T> makeVec(const Vector3<T>& vec) const {
        return Vector3(vec.x - zeroPoint.x, zeroPoint.y - vec.y, vec.z - zeroPoint.z);
    }

    Vector3<T> gridPos(const Vector3<T>& vec) const {
        return Vector3(vec.x + zeroPoint.x, zeroPoint.y - vec.y, vec.z + zeroPoint.z);
    }
};