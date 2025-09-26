// Класс для проведения тестов
// Имеет перегруженный оператор () для выполнения тестов
// Хранит два поля - количество повторений и единицы измерения времени
// Время выполнения тестов можно задать в миллисекундах или секундах ( по умолчанию в мс )
// При выполнении тестов результаты первых трех и последнего выводятся в консоль
// В результате выводит среднее время выполнения теста
// Содержит статическую функцию для сохранения вектора в файл

#pragma once
#include <chrono>
#include <functional>
#include <iostream>
#include <fstream>

class Tester{
public:

    enum class TimeUnit { MILLISECONDS, SECONDS };

    Tester() : repeat(1), unit(TimeUnit::MILLISECONDS) {}

    Tester(TimeUnit unit) : repeat(1), unit(unit) {}

    Tester(int r = 1, TimeUnit unit = TimeUnit::MILLISECONDS): repeat(r > 0 ? r : 1), unit(unit)
    {
        if (r <= 0) {
            std::cerr << "Warning: invalid repeat value. The default value is set to 1.\n";
        }
    }

    template<typename Func, typename... Args>
    double operator()(Func func, Args&&... args) const {
        double total_time = 0.0;

        for (int i = 0; i < repeat; ++i) {
            auto start = std::chrono::high_resolution_clock::now();

            auto result = func(std::forward<Args>(args)...);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;

            double time = (unit == TimeUnit::MILLISECONDS) ? duration.count() * 1000 : duration.count();
            const char* suffix = (unit == TimeUnit::MILLISECONDS) ? " ms" : " s";

            if (repeat <= 4 || i < 3)
                std::cout << "Run " << i + 1 << ": " << time << suffix << "\n";
            if (i > 3 && i + 1 == repeat)
                std::cout << "...\nRun " << i + 1 << ": " << time << suffix << "\n";

            total_time += time;
        }

        if (repeat > 1)
            std::cout << "Average time: " << total_time / repeat << ((unit == TimeUnit::MILLISECONDS) ? " ms" : " s") << "\n";
        
        return total_time;
    }

    void setRepeat(int new_repeat){
        if (new_repeat > 0)
            repeat = new_repeat;
        else 
            std::cerr << "Warning: invalid repeat value. Current number of repetitions is " << repeat << "\n";
    }

    void setTimeUnit(TimeUnit new_unit) { unit = new_unit; }

    template<typename T>
    static void saveVectorToFile(const std::vector<T>& vec, const std::string& filename = "test_file.txt") {
        std::ofstream out(filename);
        if (!out.is_open())
            throw std::runtime_error("Cannot open file " + filename);

        for (size_t i = 0; i < vec.size(); ++i) {
            out << static_cast<int>(vec[i]);
            if (i + 1 != vec.size())
                out << " ";
        }
        out << std::endl;
        out.close();
        std::cout << "Vector saved to " << filename << "\n";
    }

private: 
    int repeat; 
    TimeUnit unit;
};