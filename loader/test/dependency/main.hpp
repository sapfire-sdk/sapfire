#pragma once

#include <Sapfire/loader/Event.hpp>
#include <Sapfire/loader/Dispatch.hpp>
#include <Sapfire/binding/GJGarageLayer.hpp>

using namespace sapfire::prelude;

#ifdef SAPFIRE_IS_WINDOWS
    #ifdef EXPORTING_MOD
        #define SAPFIRE_TESTDEP_DLL __declspec(dllexport)
    #else
        #define SAPFIRE_TESTDEP_DLL __declspec(dllimport)
    #endif
#else
    #define SAPFIRE_TESTDEP_DLL
#endif

class SAPFIRE_TESTDEP_DLL TestEvent : public Event {
protected:
    std::string data;

public:
    std::string getData() const;
    TestEvent(std::string const& data);
};

class SAPFIRE_TESTDEP_DLL TestEventFilter : public EventFilter<TestEvent> {
public:
    using Callback = void(TestEvent*);

    ListenerResult handle(utils::MiniFunction<Callback> fn, TestEvent* event);
    TestEventFilter();
    TestEventFilter(TestEventFilter const&) = default;
};

using MyDispatchEvent = sapfire::DispatchEvent<GJGarageLayer*>;
using MyDispatchFilter = sapfire::DispatchFilter<GJGarageLayer*>;