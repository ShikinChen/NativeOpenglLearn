#add_definitions(-DDEMO_OSX)

find_package(OpenGL REQUIRED)

add_executable(demo_osx
  MACOSX_BUNDLE
  platforms/osx/src/main.mm
  ${RESOURCES}
)

target_link_libraries(demo_osx
  PRIVATE
  demo
)

target_compile_options(demo_osx
  PRIVATE
  -std=c++1y
  -stdlib=libc++
  -fobjc-arc
  -Wall
  -Wreturn-type
  -Wsign-compare
  -Wignored-qualifiers
  -Wtype-limits
  -Wmissing-field-initializers
)

# add resource files and property list
set_target_properties(demo_osx PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/platforms/osx/Info.plist")
