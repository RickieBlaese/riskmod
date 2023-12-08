#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <random>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

template <typename T>
T get_random_int(const T &a, const T &b) {
    static std::random_device device{};
    static std::default_random_engine engine(device());
    return std::uniform_int_distribution<T>(a, b)(engine);
}


std::tuple<std::vector<std::int64_t>, std::int64_t> run() {
    const std::int64_t trials = 100000;
    std::vector<std::int64_t> xdata(19);
    std::int64_t acount = 0;

    for (std::int64_t i = 0; i < trials; i++) {
        std::int32_t aarm = 10, darm = 8;
        while (aarm > 0 && darm > 0) {
            std::int32_t adice = std::min(4, aarm);
            std::int32_t ddice = std::min(3, darm);
            std::int32_t dice = std::min(adice, ddice);
            std::vector<std::int32_t> ad(adice), dd(ddice);
            for (std::uint32_t d = 0; d < adice; d++) {
                ad[d] = get_random_int(1, 6);
            }
            for (std::uint32_t d = 0; d < ddice; d++) {
                dd[d] = get_random_int(1, 6);
            }
            std::sort(ad.begin(), ad.end(), [](std::uint32_t a, std::uint32_t b) { return a > b; });
            std::sort(dd.begin(), dd.end(), [](std::uint32_t a, std::uint32_t b) { return a > b; });
            for (std::uint32_t d = 0; d < dice; d++) {
                std::uint32_t a = ad[d];
                std::uint32_t b = dd[d];
                if (a > b) {
                    darm--;
                } else {
                    aarm--;
                }
                if (aarm <= 0 || darm <= 0) { break; }
            }
        }
        xdata[aarm - darm + 8]++;
        if (aarm > 0) {
            acount++;
        }
    }
    return std::make_tuple(xdata, acount);
}

PYBIND11_MODULE(riskmod, m) {
    m.doc() = "simulates modified version of risk";

    m.def("run", &run, "X data, attacker win count");
}
