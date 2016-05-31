
#include<cmath>
#include<cstdint>
#include<cstdlib>
#include<ctime>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int get_index(int a) {
    int index = 0;
    switch(a) {
    case sf::Keyboard::A:
        index = 0;
        break;
    case sf::Keyboard::S:
        index = 1;
        break;
    case sf::Keyboard::D:
        index = 2;
        break;
    case sf::Keyboard::F:
        index = 3;
        break;
    case sf::Keyboard::G:
        index = 4;
        break;
    case sf::Keyboard::H:
        index = 5;
        break;
    case sf::Keyboard::J:
        index = 6;
        break;
    case sf::Keyboard::K:
        index = 7;
        break;
    case sf::Keyboard::L:
        index = 8;
        break;
    case sf::Keyboard::SemiColon:
        index = 9;
        break;
    case sf::Keyboard::Z:
        index = 10;
        break;
    case sf::Keyboard::X:
        index = 11;
        break;
    case sf::Keyboard::C:
        index = 12;
        break;
    case sf::Keyboard::V:
        index = 13;
        break;
    case sf::Keyboard::B:
        index = 14;
        break;
    case sf::Keyboard::N:
        index = 15;
        break;
    case sf::Keyboard::M:
        index = 16;
        break;
    case sf::Keyboard::Comma:
        index = 17;
        break;
    case sf::Keyboard::Period:
        index = 18;
        break;
    case sf::Keyboard::Slash:
        index = 19;
        break;
    case sf::Keyboard::Q:
        index = 20;
        break;
    case sf::Keyboard::W:
        index = 21;
        break;
    case sf::Keyboard::E:
        index = 22;
        break;
    case sf::Keyboard::R:
        index = 23;
        break;
    case sf::Keyboard::T:
        index = 24;
        break;
    case sf::Keyboard::Y:
        index = 25;
        break;
    case sf::Keyboard::U:
        index = 26;
        break;
    case sf::Keyboard::I:
        index = 27;
        break;
    case sf::Keyboard::O:
        index = 28;
        break;
    case sf::Keyboard::P:
        index = 29;
        break;
    case sf::Keyboard::Num1:
    case sf::Keyboard::Numpad1:
        index = 30;
        break;
    case sf::Keyboard::Num2:
    case sf::Keyboard::Numpad2:
        index = 31;
        break;
    case sf::Keyboard::Num3:
    case sf::Keyboard::Numpad3:
        index = 32;
        break;
    case sf::Keyboard::Num4:
    case sf::Keyboard::Numpad4:
        index = 33;
        break;
    case sf::Keyboard::Num5:
    case sf::Keyboard::Numpad5:
        index = 34;
        break;
    case sf::Keyboard::Num6:
    case sf::Keyboard::Numpad6:
        index = 35;
        break;
    case sf::Keyboard::Num7:
    case sf::Keyboard::Numpad7:
        index = 36;
        break;
    case sf::Keyboard::Num8:
    case sf::Keyboard::Numpad8:
        index = 37;
        break;
    case sf::Keyboard::Num9:
    case sf::Keyboard::Numpad9:
        index = 38;
        break;
    case sf::Keyboard::Num0:
    case sf::Keyboard::Numpad0:
        index = 39;
        break;
    case sf::Keyboard::Space:
        index = rand() % 40;
        break;
        //cout<<index<<endl;
    default:
        index = a % 39;
    }
    return index;
}

int main() {
    const static uint32_t xres = 1920;
    const static uint32_t yres = 1080;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(xres, yres), "SFML window", sf::Style::Default|sf::Style::Fullscreen);
    window.setKeyRepeatEnabled(false);

    srand(time(0));

    const static uint32_t sample_rate = 44100;
    const static size_t sample_space = 44100;
    const static float sample_mult = 16383.0; //out of 65536
    //float volume = 100.0;
    int playing = 0;
    //                               C4      D4     E4      F4      G4      A4      B4      C5
    const static float freqs[10] = {261.63 *2, 293.66 *2, 329.63 *2, 349.23 *2, 392.00 *2, 440.00 *2, 493.88 *2, 523.25 *2, 587.33 *2, 659.26 *2};
    uint16_t wavelengths[10];
    int16_t samples[40][sample_space];
    sf::SoundBuffer buffers[40];
    sf::Sound notes[40];
    
    uint16_t rand_samples[500] = {0};
    for(int i=0;i<500;++i) {
        rand_samples[i] = rand() % int16_t(sample_mult) - int16_t(sample_mult/2);
    }
    //Store wavelength of sine wave (in samples) and the raw samples for sine wave generation at the given frequency
    for(size_t i=0;i<40;++i) {
        size_t index = i;
        if(i >= 10) {
            index -= 10;
        }
        if(i >= 20) {
            index -= 10;
        }
        if(i >= 30) {
            index -= 10;
        }
        wavelengths[index] = uint16_t(float(sample_rate) / (freqs[index]));
        //cout<<"Freq: "<<freqs[index]<<"\tWavelength: "<<wavelengths[index]<<endl;
        size_t sample_size = sample_space - (sample_space % wavelengths[index]);
        for(size_t j = 0; j < sample_size; ++j) {
            //                rads in circle,  percent of the way through the circle
            if(i<10) { //sine waves
                samples[i][j] = int16_t(sample_mult * sin(2.0 * M_PI * (float(j) / float(wavelengths[index]))));
            }
            else if(i>=10 && i< 20) { //square waves
                samples[i][j] = int16_t((sample_mult/4) * (((float(j%wavelengths[index])/float(wavelengths[index])) > 0.5)?1:-1));
            }
            else if(i>=20 && i<30) { //abs value sine waves
                samples[i][j] = int16_t(abs(sample_mult * sin(2.0 * M_PI * (float(j) / float(wavelengths[index])))));
            }
            else if(i>=30 && i<40) {
                samples[i][j] = rand_samples[j % wavelengths[index]];
            }
        }
        buffers[i].loadFromSamples(&(samples[i][0]), sample_size, 1, sample_rate);
        notes[i].setBuffer(buffers[i]);
        notes[i].setLoop(true);
    }

    bool q = false;
    bool z = false;
    bool p = false;

    sf::Color colors[40];
    uint32_t positions[40][2];
    sf::CircleShape circles[40];
    
    bool shapes[40] = {false};

    int space = -1;

    float rotation = 0.0;

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if(event.type == sf::Event::KeyPressed) {
                int32_t index = get_index(event.key.code);
                switch(event.key.code) {
                case sf::Keyboard::Z:
                    z = true;
                    break;
                case sf::Keyboard::Q:
                    q = true;
                    break;
                case sf::Keyboard::P:
                    p = true;
                    break;
                case sf::Keyboard::Space:
                    space = index;
                    break;
                }
                if(index >= 0) {
                    notes[index].play();
                    positions[index][0] = rand() % (xres-150);
                    positions[index][1] = rand() % (yres-150);
                    colors[index].r = rand() % 256;
                    colors[index].g = rand() % 256;
                    colors[index].b = rand() % 256;
                    colors[index].a = 150;

                    if(index < 40) {
                        circles[index].setFillColor(colors[index]);
                        circles[index].setPosition(positions[index][0], positions[index][1]);
                        circles[index].setRadius(150);
                        shapes[index] = true;
                    }

                    if(index < 10) {
                        circles[index].setPointCount(32);
                    }
                    else if(index >= 10 && index < 20) {
                        circles[index].setPointCount(4);
                    }
                    else if(index >= 20 && index < 30) {
                        circles[index].setPointCount(3);
                    }
                    else if(index >= 30 && index < 40) {
                        circles[index].setPointCount(5);
                    }
                }
            }
            else if(event.type == sf::Event::KeyReleased) {
                int32_t index = get_index(event.key.code);
                switch(event.key.code) {
                case sf::Keyboard::Z:
                    z = false;
                    break;
                case sf::Keyboard::Q:
                    q = false;
                    break;
                case sf::Keyboard::P:
                    p = false;
                    break;
                case sf::Keyboard::Space:
                    index = space;
                    break;
                }
                if(index >= 0) {
                    notes[index].stop();
                    shapes[index] = false;
                }
            }
        }
        // Clear screen
        window.clear();
        // Draw the sprite
        //window.draw(sprite);
        // Draw the string
        //window.draw(text);
        for(int i=0;i<40;++i) {
            if(!shapes[i] && colors[i].a > 0) {
                colors[i].a -= 5;
                if(i < 40) {
                    circles[i].setFillColor(colors[i]);
                    circles[i].setRadius(circles[i].getRadius() - 5);
                    circles[i].rotate(10.0);
                }
            }
            if(shapes[i]&&i>=30&&i<40) {
                circles[i].rotate(-3.0);
            }
            if(i<40) {
                window.draw(circles[i]);
            }
        }

        // Update the window
        window.display();
        if(q&&p&&z) window.close();
        sf::sleep(sf::milliseconds(20));
    }
    return EXIT_SUCCESS;
}
