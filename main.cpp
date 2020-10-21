#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>

#include "vector3.h"


const Color redColor = Color(255, 0, 0);
const Color blackColor = Color(0, 0, 0);
const Color violet = Color(102, 0, 153);
const Color pink = Color(252, 146, 168);
const Color white = Color(255, 255, 255);

sf::Color hexToRgb(uint32_t hex) {
    uint8_t red = (hex >> 16) & 0xff;
    uint8_t green = (hex >> 8) & 0xff;
    uint8_t blue = hex & 0xff;

    return sf::Color(red, green, blue);
}

template <typename T>
class Scene: public sf::Drawable {
private:
    CoordinateSystem3<T> coordSys;
    int sceneSize;
    Vector3<T> lightPos;
    Color lightColor;
    Vector3<int> eyePosition;
    int smooth = 20;
    Color ballColor;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        sf::Image image;
        image.create(sceneSize, sceneSize, sf::Color::Black);
        for (int y = 0; y < sceneSize; ++y) {
            for (int x = 0; x < sceneSize; ++x) {
                image.setPixel(x, y, *array[x][y]);
            }
        }

        sf::Texture texture;
        texture.loadFromImage(image);
        sf::Sprite sprite(texture);
        target.draw(sprite);
//        return sprite;
    }

public:
    std::vector<std::vector<Color>> array;

    Scene(int canvasSize, const CoordinateSystem3<T>& coordSys, const Vector3<T>& lightPosition, const Color& lightColor, const Vector3<T>& eyePos):
    sceneSize(canvasSize),
    array(canvasSize, std::vector<Color>(canvasSize, blackColor)),
    coordSys(coordSys),
    lightColor(lightColor) {
        lightPos = coordSys.makeVec(lightPosition);
        eyePosition = coordSys.makeVec(lightPos);
    }

    double getZCoord(int radius, int x, int y) {
        Vector3 vec = coordSys.makeVec(x, y, 0);
        return std::sqrt(radius * radius - vec.len() * vec.len());
    }

    void makeBall(int radius, const Color& color) {
        ballColor = color;

        for (int y = 0; y < array.size(); ++y) {
            for (int x = 0; x < array[0].size(); ++x) {
                Vector3 vecToPoint = coordSys.makeVec(x, y, getZCoord(radius, x, y));

                if (vecToPoint.len() <= radius) {
                    Vector3 pointToLight = lightPos - vecToPoint;
                    double diffuseIntensity = 0.50 * std::max(0.0, vecToPoint.cos(pointToLight));
                    double ambientIntensity = 0.15;

                    Vector3 pointToEye = eyePosition - vecToPoint;
                    double specularIntensity = 0.35 * std::pow(vecToPoint.cos(pointToEye + pointToLight), smooth);

                    array[x][y] = ballColor * lightColor * diffuseIntensity +
                                  lightColor * specularIntensity +
                                  ballColor * lightColor * ambientIntensity;
                }
            }
        }
    }

    void setLightPosition(const Vector3<T>& pos) {
        lightPos = coordSys.makeVec(pos);
    }

    Vector3<T> getLightPosition() {
        return coordSys.gridPos(lightPos);
    }
};

class SceneObject {
    virtual ~SceneObject() = default;
};

class Ball {
private:
    int radius = 0;
    Color color;
    int smooth;

public:
    Ball(int radius, const Color& color, int smooth):
    radius(radius),
    color(color),
    smooth(smooth) {};
};

int main() {
    int canvasSize = 800;
    CoordinateSystem3 coordSys(canvasSize / 2, canvasSize / 2, 0);
    Scene scene (
            canvasSize,
            coordSys,
            Vector3(0, 0, 1000),
            pink,
            Vector3(canvasSize / 2, canvasSize / 2, 600));

//    Ball ball(300, redColor, 20);
    scene.makeBall(300, violet);

    sf::RenderWindow window(sf::VideoMode(canvasSize, canvasSize), "Ball window");
//    window.setFramerateLimit(30);

    auto lastTime = std::chrono::system_clock::now();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        auto nowTime = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = nowTime - lastTime;
        if (diff.count() > 0.000005) {

            Vector3 lightPos = scene.getLightPosition();
            scene.setLightPosition(Vector3(lightPos.x + 15, lightPos.y + 15, lightPos.z));
            scene.makeBall(300, violet);

            lastTime = std::chrono::system_clock::now();
        }
        window.draw(scene);
        window.display();
    }

    return 0;
}
