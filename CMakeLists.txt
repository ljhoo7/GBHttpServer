cmake_minimum_required (VERSION 3.17.0)

project(GBHttpServerFramework)

add_subdirectory ("core")
#add_subdirectory ("game")
#add_subdirectory ("http")
#add_subdirectory ("sample")

set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT Sample)