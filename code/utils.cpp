#include <random>
#include <time.h>

namespace ng
{
    union Distribution
    {
        std::uniform_real_distribution<float> realD;
        std::uniform_int_distribution<uint32> intD;
    };

    struct Randomizer
    {
        std::default_random_engine generator;
        Distribution distribution;
    };

    // distribution(gen) to get the random value
    Randomizer
    getRandomInt(uint32 low, uint32 high)
    {
        Randomizer randomizer = {};
        std::default_random_engine generator(time(0));
        std::uniform_int_distribution<uint32> distribution(low, high);
        randomizer.generator = generator;
        randomizer.distribution.intD = distribution;
        return randomizer;
    }

    Randomizer
    getRandomReal(float low, float high)
    {
        Randomizer randomizer = {};
        std::default_random_engine generator(time(0));
        std::uniform_real_distribution<float> distribution(low, high);
        randomizer.generator = generator;
        randomizer.distribution.realD = distribution;
        return randomizer;
    } 

    void concatStrings(char *dest, char *str1, char *str2){
        strcat(dest, str1);
        strcat(dest, str2);
    }
};