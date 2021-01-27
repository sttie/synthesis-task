// Этот файл реализует мини-библиотеку для синтезирования 
// частот и создания соответствующего заголовка для создания .wav файла

#pragma once

#include <QTextStream>
#include <vector>
#include <type_traits>
#include <fstream>
#include <iostream>

#include "number_traits.h"


namespace wav {

    // Для подробной информации см. формат .wav файла
    template <typename T, std::enable_if_t<is_number<T>::value, bool> = true>
    struct WavHeader {
        int32_t chunk_size;
        int32_t subchunk_size;
        int16_t audio_format;
        int16_t num_channels;
        int32_t sample_rate;
        int32_t byte_rate;
        int16_t block_align;
        int16_t bits_per_sample;
        int32_t subchunk2_size;
        std::vector<T> data;
    };

    // Для подробной информации см. формат .wav файла
    template <typename T>
    inline std::ostream& operator<<(std::ostream& out, const WavHeader<T>& header) {
        out << "RIFF";
        out.write(reinterpret_cast<const char*>(&header.chunk_size), sizeof(header.chunk_size));
        out << "WAVEfmt ";
        out.write(reinterpret_cast<const char*>(&header.subchunk_size), sizeof(header.subchunk_size));
        out.write(reinterpret_cast<const char*>(&header.audio_format), sizeof(header.audio_format));
        out.write(reinterpret_cast<const char*>(&header.num_channels), sizeof(header.num_channels));
        out.write(reinterpret_cast<const char*>(&header.sample_rate), sizeof(header.sample_rate));
        out.write(reinterpret_cast<const char*>(&header.byte_rate), sizeof(header.byte_rate));
        out.write(reinterpret_cast<const char*>(&header.block_align), sizeof(header.block_align));
        out.write(reinterpret_cast<const char*>(&header.bits_per_sample), sizeof(header.bits_per_sample));
        out << "data";
        out.write(reinterpret_cast<const char*>(&header.subchunk2_size), sizeof(header.subchunk2_size));

        for (T sample : header.data) {
            out.write(reinterpret_cast<const char*>(&sample), sizeof(T));
        }

        return out;
    }


    // Функция создает и возвращает wav-заголовок по заданным параметрам
    template <typename T, std::enable_if_t<is_number<T>::value, bool> = true>
    inline WavHeader<T> CreateWavHeader(
        std::vector<T> data, int16_t num_channels,
        int32_t sample_rate
    ) {
        WavHeader<T> header{};

        int32_t data_size = data.size() * sizeof(T);
        header.chunk_size = data_size + 44 - 8; // 44 - header size, 8 - first two records' size
        header.subchunk_size = 16;
        header.audio_format = 1;
        header.num_channels = num_channels;
        header.sample_rate = sample_rate;

        int16_t bytes_per_sample = sizeof(T);
        header.byte_rate = num_channels * sample_rate * bytes_per_sample;
        header.block_align = num_channels * bytes_per_sample;
        header.bits_per_sample = bytes_per_sample * 8;
        header.subchunk2_size = data_size;

        header.data = std::move(data);

        return header;
    }

}
