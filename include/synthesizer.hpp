#pragma once

#include <vector>
#include <cmath>

#include "wav.hpp"
#include "number_traits.h"


// Реализация алгоритма Корплуса-Стронга
template <typename T, std::enable_if_t<is_number<T>::value, bool> = true>
static std::vector<int16_t> KorplusStrongAlgo(
    std::vector<T> noise, std::size_t samples_amount,
    double energy_decay
) {
    std::vector<int16_t> samples(samples_amount);
    std::size_t current = 0;
    T previous_sample = 0;

    for (std::size_t i = 0; i < samples_amount; ++i) {
        noise[current] = energy_decay * 0.5 * (noise[current] + previous_sample);
        // Сразу приводим ответ к int16
        samples[i] = static_cast<int16_t>(noise[current] * 32768);
        previous_sample = noise[current];
        current = (current + 1) % noise.size();
    }

    return samples;
}

auto SynthesizeWavHeader(
    int32_t sample_rate, int32_t duration,
    double energy_decay, int16_t num_channels,
    const std::vector<double>& freqs
) {
    std::vector<int16_t> samples;
    // Делим общую длительность на равные (по крайней мере, примерно равные!) части для каждой ноты
    double parted_duration = duration * 1.0 / freqs.size();
    std::size_t samples_amount = static_cast<std::size_t>(sample_rate * num_channels * parted_duration);

    // Для каждой ноты синтезируем свои сэмплы
    for (std::size_t i = 0; i < freqs.size(); ++i) {
        std::vector<double> noise(sample_rate / freqs[i]);
        for (std::size_t i = 0; i < noise.size(); ++i) {
            noise[i] = 2 * (rand() % 2) - 1;
        }

        // Синтезируем и добавляем в вектор всех сэмплов
        for (auto sample : KorplusStrongAlgo(noise, samples_amount, energy_decay)) {
            samples.push_back(sample);
        }
    }

    return wav::CreateWavHeader(std::move(samples), num_channels, sample_rate);
}
