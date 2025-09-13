#include <sndfile.h>
#include <vector>
#include <iostream>
#include <cmath>

int main() {
    const char* inputFile = "./files/original_test1.wav";
    const char* outputFile = "./files/processed_test1.wav";

    SF_INFO sfinfo;
    SNDFILE* infile = sf_open(inputFile, SFM_READ, &sfinfo);
    if (!infile) {
        std::cerr << "Error in opening file: " << sf_strerror(nullptr) << "\n";
        return 1;
    }

    // 1. Читаем сэмплы в float [-1,1]
    std::vector<float> samples(sfinfo.frames * sfinfo.channels);
    sf_readf_float(infile, samples.data(), sfinfo.frames);
    sf_close(infile);

    std::cout << "Прочитано " << sfinfo.frames << " фреймов, "
              << sfinfo.channels << " канал(ов), "
              << "частота: " << sfinfo.samplerate << "\n";

    // Здесь можно работать с samples в формате [-1,1]

    // 2. Преобразуем обратно в int16 для сохранения
    std::vector<short> intSamples(samples.size());
    for (size_t i = 0; i < samples.size(); ++i) {
        // Клиппинг, чтобы не выйти за пределы [-1,1]
        float x = std::fmax(-1.0f, std::fmin(1.0f, samples[i]));
        intSamples[i] = static_cast<short>(std::round(x * 32767.0f));
    }

    // 3. Сохраняем новый WAV
    SF_INFO outInfo = sfinfo; // копируем метаданные
    SNDFILE* outfile = sf_open(outputFile, SFM_WRITE, &outInfo);
    if (!outfile) {
        std::cerr << "Error in creating file: " << sf_strerror(nullptr) << "\n";
        return 1;
    }

    sf_writef_short(outfile, intSamples.data(), sfinfo.frames);
    sf_close(outfile);

    std::cout << "File successfully saved: " << outputFile << "\n";

    return 0;
}



//g++ src/main.cpp -o src/main.exe -lfftw3 -lsndfile -lm && ./src/main.exe