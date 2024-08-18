cmake_minimum_required(VERSION 3.3.0)


add_library(SapfireBootstrapper SHARED Bootstrapper.cpp)
target_compile_features(SapfireBootstrapper PUBLIC cxx_std_17)
set_target_properties(SapfireBootstrapper PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "SapfireBootstrapper"
	LIBRARY_OUTPUT_DIRECTORY "${SAPFIRE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${SAPFIRE_BIN_PATH}/nightly"
)

target_link_libraries(SapfireBootstrapper PRIVATE)

add_library(FakeSapfire SHARED FakeSapfire.cpp)
target_compile_features(FakeSapfire PUBLIC cxx_std_17)
set_target_properties(FakeSapfire PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "Sapfire"
)

set_target_properties(SapfireBootstrapper PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(SapfireBootstrapper PROPERTIES INSTALL_RPATH "./")

set_target_properties(FakeSapfire PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(FakeSapfire PROPERTIES INSTALL_RPATH "./")

target_link_libraries(SapfireBootstrapper PRIVATE FakeSapfire)
