#include <random>

class RandomNumberGenerationService {
public:
    static int getRandomNumberInRange(const int& min, const int& max) {
        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());
        std::uniform_int_distribution<int> distribution(min, max);
    
        const int randomNumber = distribution(generator);
    
        return randomNumber;
    }
};