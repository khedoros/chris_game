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
const sf::Color HOUR_TICK_COLOR(0, 0, 0);
const sf::Color MINUTE_TICK_COLOR(120, 120, 120);
const sf::Color HAND_COLOR(0, 0, 0);
const int FONT_SIZE = 48;

void updateDigitalClock(sf::Text& digitalClockText, int hours, int minutes) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    digitalClockText.setString(oss.str());
}

void drawClockTicks(sf::RenderWindow& window, sf::CircleShape& clockShape, int numTicks, float tickLength, sf::Color tickColor) {
    float angleStep = 2 * PI / numTicks;
    float currentAngle = -PI / 2;

    sf::Vector2f clockCenter = clockShape.getPosition() + sf::Vector2f(CLOCK_RADIUS, CLOCK_RADIUS);

    for (int i = 0; i < numTicks; ++i) {
        float startX = clockCenter.x + std::cos(currentAngle) * (CLOCK_RADIUS - tickLength);
        float startY = clockCenter.y + std::sin(currentAngle) * (CLOCK_RADIUS - tickLength);
        float endX = clockCenter.x + std::cos(currentAngle) * CLOCK_RADIUS;
        float endY = clockCenter.y + std::sin(currentAngle) * CLOCK_RADIUS;

        sf::Vertex tick[] = {
            sf::Vertex(sf::Vector2f(startX, startY), tickColor),
            sf::Vertex(sf::Vector2f(endX, endY), tickColor)
        };

        window.draw(tick, 2, sf::Lines);

        currentAngle += angleStep;
    }
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

    const int NUM_HOUR_TICKS = 12;
    const float HOUR_TICK_LENGTH = 10.0f;
    const int NUM_MINUTE_TICKS = 60;
    const float MINUTE_TICK_LENGTH = 5.0f;

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

        drawClockTicks(window, clockShape, NUM_HOUR_TICKS, HOUR_TICK_LENGTH, HOUR_TICK_COLOR);
        drawClockTicks(window, clockShape, NUM_MINUTE_TICKS, MINUTE_TICK_LENGTH, MINUTE_TICK_COLOR);

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
