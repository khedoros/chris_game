#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <string>

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 900;
const float CLOCK_RADIUS = WINDOW_HEIGHT / 2.5f;

void updateDigitalClock(sf::Text& digitalClockText, int hours, int minutes) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    digitalClockText.setString(oss.str());
}

void updateAnalogClock(sf::Text& analogClockText, int hours, int minutes) {
    const std::array<std::string,19> numbers = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const std::array<std::string,6> tens = {"o' clock", "Ten", "Twenty", "Thirty", "Forty", "Fifty"};
    const std::string quarterTo = "Quarter to";
    const std::string quarterPast = "Quarter past";
    const std::string halfPast = "Half past";
    const std::string oh = "oh";

    std::string text = "";

    if (minutes == 0) {
        text = numbers[hours - 1] + " " + tens[0];
    } else if(minutes == 15) {
        text = quarterPast + " " + numbers[hours - 1];
    } else if(minutes == 30) {
        text = halfPast + " " + numbers[hours - 1];
    } else if(minutes == 45) {
        text = quarterTo + " " + numbers[(hours+12) % 12];
    } else {
        text = numbers[hours - 1] + " ";
        if(minutes < 10) {
            text += oh + " ";
        }

        if(minutes < 20) {
            text += numbers[minutes - 1];
        } else {
            int tenDigit = minutes / 10;
            int oneDigit = minutes % 10;
            text += tens[tenDigit];
            if(oneDigit > 0) {
                text += "-" + numbers[oneDigit - 1];
            }
        }
    }

    analogClockText.setString(text);
}

void drawClockTicks(sf::RenderWindow& window, sf::CircleShape& clockShape, int numTicks, float tickLength, sf::Color tickColor) {
    float angleStep = 2 * M_PI / static_cast<float>(numTicks);

    sf::Vector2f clockCenter = clockShape.getPosition() + sf::Vector2f(CLOCK_RADIUS, CLOCK_RADIUS);

    sf::RectangleShape tickShape(sf::Vector2f(tickLength / 4, tickLength));

    for (int i = 0; i < numTicks; ++i) {
        float currentAngle = angleStep * i;
        tickShape.setOrigin(tickShape.getSize().x / 2, CLOCK_RADIUS);
        tickShape.setPosition(clockCenter);
        tickShape.setFillColor(tickColor);
        tickShape.setRotation(currentAngle * 180.0 / M_PI);
        window.draw(tickShape);
    }
}

void drawClockNumbers(sf::RenderWindow& window, sf::CircleShape& clockShape, sf::Font& font, int fontSize, sf::Color numberColor) {
    const int NUM_HOURS = 12;
    float angleStep = 2 * M_PI / static_cast<float>(NUM_HOURS);

    sf::Vector2f clockCenter = clockShape.getPosition() + sf::Vector2f(CLOCK_RADIUS, CLOCK_RADIUS);

    for (int i = 1; i <= NUM_HOURS; ++i) {
        float currentAngle = angleStep * static_cast<float>(i) - M_PI / 2;
        sf::Text numberText(std::to_string(i), font, fontSize);
        numberText.setFillColor(numberColor);
        sf::FloatRect bounds = numberText.getLocalBounds();
        numberText.setOrigin(bounds.width / 2.0, bounds.height);
        float distance = 0.8 * CLOCK_RADIUS;
        sf::Vector2f numPosition = clockCenter + sf::Vector2f(distance * cos(currentAngle), distance * sin(currentAngle));
        numberText.setPosition(numPosition);
        window.draw(numberText);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Analog and Digital Clock");
    window.setFramerateLimit(60);

    int hours = 12;
    int minutes = 0;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    const int FONT_SIZE = 48;
    const sf::Color TEXT_COLOR(255, 255, 255);
    sf::Text digitalClockText("", font, FONT_SIZE);
    digitalClockText.setFillColor(TEXT_COLOR);
    digitalClockText.setPosition(10.0f, 10.0f);
    updateDigitalClock(digitalClockText, hours, minutes);

    sf::Text analogClockText("", font, FONT_SIZE);
    analogClockText.setFillColor(TEXT_COLOR);
    analogClockText.setPosition(200.0f, 10.0f); // TODO: Center text near bottom of screen
    updateAnalogClock(analogClockText, hours, minutes);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    hours++;
                    if (hours > 12) {
                        hours = 1;
                    }
                    updateDigitalClock(digitalClockText, hours, minutes);
                    updateAnalogClock(analogClockText, hours, minutes);
                } else if (event.key.code == sf::Keyboard::Down) {
                    hours--;
                    if (hours < 1) {
                        hours = 12;
                    }
                    updateDigitalClock(digitalClockText, hours, minutes);
                    updateAnalogClock(analogClockText, hours, minutes);
                } else if (event.key.code == sf::Keyboard::Right) {
                    minutes++;
                    if (minutes > 59) {
                        minutes = 0;
                        hours++;
                        if (hours > 12) {
                            hours = 1;
                        }
                    }
                    updateDigitalClock(digitalClockText, hours, minutes);
                    updateAnalogClock(analogClockText, hours, minutes);
                } else if (event.key.code == sf::Keyboard::Left) {
                    minutes--;
                    if (minutes < 0) {
                        minutes = 59;
                        hours--;
                        if (hours < 1) {
                            hours = 12;
                        }

                    }
                    updateDigitalClock(digitalClockText, hours, minutes);
                    updateAnalogClock(analogClockText, hours, minutes);
                }
            }
        }

        const sf::Color BACKGROUND_COLOR(30, 30, 30);
        window.clear(BACKGROUND_COLOR);

        const sf::Color CLOCK_COLOR(255, 255, 255);
        sf::CircleShape clockShape(CLOCK_RADIUS);
        clockShape.setFillColor(CLOCK_COLOR);
        clockShape.setPosition(WINDOW_WIDTH / 2 - CLOCK_RADIUS, WINDOW_HEIGHT / 2 - CLOCK_RADIUS);

        window.draw(clockShape);

        const int NUM_HOUR_TICKS = 12;
        const float HOUR_TICK_LENGTH = CLOCK_RADIUS / 10;
        const int NUM_MINUTE_TICKS = 60;
        const float MINUTE_TICK_LENGTH = CLOCK_RADIUS / 15;

        const sf::Color HOUR_TICK_COLOR(255, 0, 0);
        const sf::Color MINUTE_TICK_COLOR(255, 120, 120);
        const sf::Color NUMBER_COLOR(0,0,0);
        drawClockTicks(window, clockShape, NUM_MINUTE_TICKS, MINUTE_TICK_LENGTH, MINUTE_TICK_COLOR);
        drawClockTicks(window, clockShape, NUM_HOUR_TICKS, HOUR_TICK_LENGTH, HOUR_TICK_COLOR);
        drawClockNumbers(window, clockShape, font, FONT_SIZE, NUMBER_COLOR);

        const float HOUR_HAND_LENGTH = 0.6f * CLOCK_RADIUS;
        const float HOUR_HAND_WIDTH = CLOCK_RADIUS / 50.0;
        const sf::Color HAND_COLOR(0, 0, 0);
        sf::RectangleShape hourHandShape(sf::Vector2f(HOUR_HAND_WIDTH, -HOUR_HAND_LENGTH));
        hourHandShape.setFillColor(HAND_COLOR);
        hourHandShape.setOrigin(2.5f, 0);

        const float MINUTE_HAND_LENGTH = 0.8f * CLOCK_RADIUS;
        const float MINUTE_HAND_WIDTH = CLOCK_RADIUS / 75.0;
        sf::RectangleShape minuteHandShape(sf::Vector2f(MINUTE_HAND_WIDTH, -MINUTE_HAND_LENGTH));
        minuteHandShape.setFillColor(HAND_COLOR);
        minuteHandShape.setOrigin(1.5f, 0);


        float hourAngle = (hours % 12 + minutes / 60.0f) * (2 * M_PI / 12);
        hourHandShape.setRotation(hourAngle * 180 / M_PI);
        hourHandShape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        window.draw(hourHandShape);

        float minuteAngle = (minutes / 60.0f) * (2 * M_PI);
        minuteHandShape.setRotation(minuteAngle * 180 / M_PI);
        minuteHandShape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        window.draw(minuteHandShape);

        window.draw(digitalClockText);
        window.draw(analogClockText);

        window.display();
    }

    return 0;
}
