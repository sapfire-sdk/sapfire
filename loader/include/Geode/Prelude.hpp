#pragma once

// Because C++ doesn't like using
// namespaces that don't exist
namespace sapfire {
    namespace addresser {}
    namespace cast {}
    namespace cocos {}
    namespace utils {}
    namespace helper {}
    namespace op {}
    namespace stream {}
    namespace view {}
}

namespace cocos2d {
    namespace extension {}
}

namespace sapfire {
    namespace prelude {
        using namespace ::sapfire;
        using namespace ::sapfire::addresser;
        using namespace ::sapfire::cast;
        using namespace ::sapfire::cocos;
        using namespace ::sapfire::helper;
        using namespace ::sapfire::utils;
        using namespace ::sapfire::op;
        using namespace ::sapfire::stream;
        using namespace ::sapfire::view;
        using namespace ::cocos2d;
        using namespace ::cocos2d::extension;
    }
}
