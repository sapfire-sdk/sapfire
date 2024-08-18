#pragma once
#include <Sapfire/c++stl/gdstdlib.hpp>

namespace sapfire::stl {
    struct StringImpl {
        StringData& data;

        // clear but assumes the existing data is uninit,
        // so basically a default ctor
        void setEmpty();

        // frees the existing string
        void free();

        char* getStorage();
        void setStorage(const std::string_view);

        size_t getSize();
        void setSize(size_t);

        size_t getCapacity();
        void setCapacity(size_t);
    };
}