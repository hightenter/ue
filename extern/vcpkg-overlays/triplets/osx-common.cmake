include("${CMAKE_CURRENT_LIST_DIR}/common.cmake")

set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_OSX_DEPLOYMENT_TARGET 10.15)