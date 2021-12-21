cmake_minimum_required(VERSION 3.14)

project(
    croquis
    VERSION 0.1
    DESCRIPTION "HPC multi-stream sketching library"
    LANGUAGES CXX
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CROQUIS_MAIN_PROJECT ON)
endif ()

# Controls whether to set up install boilerplate for project and depencencies.
# Expects CMAKE_INSTALL_PREFIX to be set to a suitable directory.
option(CROQUIS_INSTALL "Generate the install target" ${CROQUIS_MAIN_PROJECT})

if (CROQUIS_MAIN_PROJECT)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app NOTE: this needs to be done
    # in the main CMakeLists because it calls enable_testing, which must be in
    # the main CMakeLists
    include(CTest)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if (Doxygen_FOUND)
        # add_subdirectory(docs)
        message(STATUS "Doxygen found, skipping docs")
    else ()
        message(STATUS "Doxygen not found, not building docs")
    endif ()

endif ()

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
    message(
        FATAL_ERROR "Cannot build in a directory with a CMakeLists.txt file. "
                    "Please make a build subdirectory."
    )
endif ()

# Prepare to invoke FetchContent
include(FetchContent)

option(CROQUIS_USE_BOOST "Compile using boost::container::flat_map" ON)
set(croquis_boost_VERSION 1.48)
if (CROQUIS_USE_BOOST)
    # find_package(Boost 1.75 COMPONENTS container)
    find_package(Boost ${croquis_boost_VERSION} QUIET)
    if (Boost_FOUND)
        message(STATUS "croquis found boost include dirs: "
                       ${Boost_INCLUDE_DIRS}
        )
    else ()
        message(STATUS "croquis could not find boost. skipping...")
    endif ()
else ()
    message(STATUS "croquis building without boost")
endif ()

option(CROQUIS_USE_YGM "Compile using YGM distributed memory features" ON)
if (CROQUIS_USE_YGM)
    # Load cereal
    set(CROQUIS_CEREAL_VERSION 1.3.0)
    # Will do something like this upon new release and fix to spack
    # distribution. find_package(cereal ${CROQUIS_CEREAL_VERSION} CONFIG QUIET)
    set(CROQUIS_CEREAL_TARGET "cereal::cereal")
    find_package(cereal CONFIG QUIET)
    if (NOT cereal_FOUND)
        # Currently cereal version 1.3.0 has an outdated CMakeLists.txt, so we
        # need to use this commit for now.
        FetchContent_Declare(
            cereal
            GIT_REPOSITORY https://github.com/USCiLab/cereal
            GIT_TAG af0700efb25e7dc7af637b9e6f970dbb94813bff
        )
        FetchContent_GetProperties(cereal)
        if (cereal_POPULATED)
            message(STATUS "croquis found already populated cereal dependency: "
                           ${cereal_SOURCE_DIR}
            )
        else ()
            # Do not compile any cereal tests
            set(JUST_INSTALL_CEREAL ON)
            # Install cereal at ${CMAKE_INSTALL_PREFIX}
            set(CEREAL_INSTALL ${CROQUIS_INSTALL})
            # Populate cereal
            FetchContent_Populate(cereal)
            # Include cereal root cmake boilerplate
            add_subdirectory(${cereal_SOURCE_DIR})
            message(STATUS "croquis cloned cereal dependency: "
                           ${cereal_SOURCE_DIR}
            )
        endif ()
    else ()
        # cereal installed with spack creates library target "cereal", whereas
        # installing from source creates target "cereal::cereal". This is the
        # only simple way I could figure out how to differentiate the two, but
        # this will cause problems if a spack instance installs headers to a
        # path that does not include the substring "spack".
        message(STATUS "croquis found installed cereal dependency: "
                       ${cereal_DIR}
        )
        if (${cereal_DIR} MATCHES ".*spack.*")
            set(CROQUIS_CEREAL_TARGET "cereal")
        else ()
            # target should still be cereal::cereal
        endif ()
    endif ()

    # Load YGM
    set(CROQUIS_YGM_VERSION 0.3)
    find_package(ygm ${CROQUIS_YGM_VERSION} CONFIG QUIET)
    if (NOT ygm_FOUND)
        FetchContent_Declare(
            ygm
            GIT_REPOSITORY https://github.com/LLNL/ygm
            GIT_TAG v${CROQUIS_YGM_VERSION}
        )
        FetchContent_GetProperties(ygm)
        if (ygm_POPULATED)
            message(STATUS "croquis found already populated ygm dependency: "
                           ${ygm_SOURCE_DIR}
            )
        else ()
            # Do not compile any ygm tests
            set(JUST_INSTALL_YGM ON)
            # Install ygm at ${CMAKE_INSTALL_PREFIX}
            set(YGM_INSTALL ${CROQUIS_INSTALL})
            # Populate ygm
            FetchContent_Populate(ygm)
            # Include ygm cmake boilerplate
            add_subdirectory(${ygm_SOURCE_DIR})
            message(STATUS "croquis cloned ygm dependency " ${ygm_SOURCE_DIR})
        endif ()
    else ()
        message(STATUS "croquis found ygm dependency " ${ygm_DIR})
    endif ()
else ()
    message(STATUS "croquis building without ygm")
endif ()

add_library(croquis INTERFACE)
add_library(croquis::croquis ALIAS croquis)
target_include_directories(
    croquis INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                      $<INSTALL_INTERFACE:include>
)
target_compile_features(croquis INTERFACE cxx_std_17)
target_link_libraries(croquis INTERFACE ${CROQUIS_CEREAL_TARGET} ygm::ygm)
if (Boost_FOUND)
    target_include_directories(croquis INTERFACE ${Boost_INCLUDE_DIRS})
endif ()

option(TEST_WITH_SLURM "Run tests with Slurm" OFF)

if (CROQUIS_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    set(CROQUIS_EXPORT_TARGETS croquis)

    install(
        TARGETS ${CROQUIS_EXPORT_TARGETS}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(
        EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
    )

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/croquis
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # create version file
    write_basic_package_version_file(
        "${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY ExactVersion
    )

    # create config file
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
    )

    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
    )

endif ()

option(JUST_INSTALL_CROQUIS "Skip executable compilation" OFF)
if (JUST_INSTALL_CROQUIS)
    return()
endif ()

if (CROQUIS_MAIN_PROJECT)
    add_subdirectory(test)
endif ()
