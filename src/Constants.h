//#ifndef CONSTANTS_HH
//#define CONSTANTS_HH

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int BITS_PER_PIXEL = 32;
const sf::Time TIMESTEP = sf::seconds(1.0f / 60.0f); // timestep, 60 fps
const int velocityIterations = 8;
const int positionIterations = 3;
const float PIXELS_PER_METER = 30.0f;
const float RAD_TO_DEG = 57.275f;

//#endif