// SFINAE трейты для фильтрации шаблонных параметров в библиотеке wav.hpp

#pragma once

template <typename T>
struct is_number {
    static constexpr bool value = false;
};

template <>
struct is_number<double> {
    static constexpr bool value = true;
};

template <>
struct is_number<float> {
    static constexpr bool value = true;
};

template <>
struct is_number<int8_t> {
    static constexpr bool value = true;
};

template <>
struct is_number<int16_t> {
    static constexpr bool value = true;
};

template <>
struct is_number<int32_t> {
    static constexpr bool value = true;
};

template <>
struct is_number<int64_t> {
    static constexpr bool value = true;
};
