#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

const float PI = 3.14159f;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float CLOCK_RADIUS = 200.0f;
const float HOUR_HAND_LENGTH = 0.6f * CLOCK_RADIUS;
const float MINUTE_HAND_LENGTH = 0.8f * CLOCK_RADIUS;
const sf::Color BACKGROUND_COLOR(30, 30, 30);
const sf::Color CLOCK_COLOR(255, 255, 255);
const sf::Color HAND_COLOR(0, 0, 0);
const int FONT_SIZE = 48;

void updateDigitalClock(sf::Text& digitalClockText, int hours, int minutes) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    digitalClockText.setString(oss.str());
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Analog and Digital Clock");

    sf::CircleShape clockShape(CLOCK_RADIUS);
    clockShape.setFillColor(CLOCK_COLOR);
    clockShape.setPosition(WINDOW_WIDTH / 2 - CLOCK_RADIUS, WINDOW_HEIGHT / 2 - CLOCK_RADIUS);

    sf::RectangleShape hourHandShape(sf::Vector2f(5.0f, -HOUR_HAND_LENGTH));
    hourHandShape.setFillColor(HAND_COLOR);
    hourHandShape.setOrigin(2.5f, HOUR_HAND_LENGTH);

    sf::RectangleShape minuteHandShape(sf::Vector2f(3.0f, -MINUTE_HAND_LENGTH));
    minuteHandShape.setFillColor(HAND_COLOR);
    minuteHandShape.setOrigin(1.5f, MINUTE_HAND_LENGTH);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    sf::Text digitalClockText("", font, FONT_SIZE);
    digitalClockText.setFillColor(CLOCK_COLOR);
    digitalClockText.setPosition(10.0f, 10.0f);

    int hours = 0;
    int minutes = 0;
    updateDigitalClock(digitalClockText, hours, minutes);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    hours++;
                    if (hours > 23) {
                        hours = 0;
                    }
                    updateDigitalClock(digitalClockText, hours, minutes);
                } else if (event.key.code == sf::Keyboard::Down) {
                    hours--;
                    if (hours < 0) {
                        hours = 23;
                    }
                    updateDigitalClock(digitalClockText, hours, minutes);
                } else if (event.key.code == sf::Keyboard::Right) {
                    minutes++;
                    if (minutes > 59) {
                        minutes = 0;
                    }
                    updateDigitalClock(digitalClockText, hours, minutes);
                } else if (event.key.code == sf::Keyboard::Left) {
                    minutes--;
                    if (minutes < 0) {
                        minutes = 59;
                    }
                    updateDigitalClock(digitalClockText, hours, minutes);
                }
            }
        }

        window.clear(BACKGROUND_COLOR);

        window.draw(clockShape);

        float hourAngle = (hours % 12 + minutes / 60.0f) * (2 * PI / 12) - PI / 2;
        hourHandShape.setRotation(hourAngle * 180 / PI);
        hourHandShape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        window.draw(hourHandShape);

        float minuteAngle = (minutes / 60.0f) * (2 * PI) - PI / 2;
        minuteHandShape.setRotation(minuteAngle * 180 / PI);
        minuteHandShape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        window.draw(minuteHandShape);

        window.draw(digitalClockText);

        window.display();
    }

    return 0;
}
