cmake_minimum_required (VERSION 3.17.0)

add_subdirectory ("core")
add_subdirectory ("game")
#add_subdirectory ("http")
#add_subdirectory ("sample")

#add_subdirectory ("flatbuffers")

set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT Sample)