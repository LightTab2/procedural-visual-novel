# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(ms-gsl_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(ms-gsl_FRAMEWORKS_FOUND_RELEASE "${ms-gsl_FRAMEWORKS_RELEASE}" "${ms-gsl_FRAMEWORK_DIRS_RELEASE}")

set(ms-gsl_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET ms-gsl_DEPS_TARGET)
    add_library(ms-gsl_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET ms-gsl_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${ms-gsl_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${ms-gsl_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### ms-gsl_DEPS_TARGET to all of them
conan_package_library_targets("${ms-gsl_LIBS_RELEASE}"    # libraries
                              "${ms-gsl_LIB_DIRS_RELEASE}" # package_libdir
                              "${ms-gsl_BIN_DIRS_RELEASE}" # package_bindir
                              "${ms-gsl_LIBRARY_TYPE_RELEASE}"
                              "${ms-gsl_IS_HOST_WINDOWS_RELEASE}"
                              ms-gsl_DEPS_TARGET
                              ms-gsl_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "ms-gsl"    # package_name
                              "${ms-gsl_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${ms-gsl_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Release ########################################

    ########## COMPONENT Microsoft.GSL::GSL #############

        set(ms-gsl_Microsoft.GSL_GSL_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(ms-gsl_Microsoft.GSL_GSL_FRAMEWORKS_FOUND_RELEASE "${ms-gsl_Microsoft.GSL_GSL_FRAMEWORKS_RELEASE}" "${ms-gsl_Microsoft.GSL_GSL_FRAMEWORK_DIRS_RELEASE}")

        set(ms-gsl_Microsoft.GSL_GSL_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET ms-gsl_Microsoft.GSL_GSL_DEPS_TARGET)
            add_library(ms-gsl_Microsoft.GSL_GSL_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET ms-gsl_Microsoft.GSL_GSL_DEPS_TARGET
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_DEPENDENCIES_RELEASE}>
                     APPEND)

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'ms-gsl_Microsoft.GSL_GSL_DEPS_TARGET' to all of them
        conan_package_library_targets("${ms-gsl_Microsoft.GSL_GSL_LIBS_RELEASE}"
                              "${ms-gsl_Microsoft.GSL_GSL_LIB_DIRS_RELEASE}"
                              "${ms-gsl_Microsoft.GSL_GSL_BIN_DIRS_RELEASE}" # package_bindir
                              "${ms-gsl_Microsoft.GSL_GSL_LIBRARY_TYPE_RELEASE}"
                              "${ms-gsl_Microsoft.GSL_GSL_IS_HOST_WINDOWS_RELEASE}"
                              ms-gsl_Microsoft.GSL_GSL_DEPS_TARGET
                              ms-gsl_Microsoft.GSL_GSL_LIBRARIES_TARGETS
                              "_RELEASE"
                              "ms-gsl_Microsoft.GSL_GSL"
                              "${ms-gsl_Microsoft.GSL_GSL_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET Microsoft.GSL::GSL
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_LIBRARIES_TARGETS}>
                     APPEND)

        if("${ms-gsl_Microsoft.GSL_GSL_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET Microsoft.GSL::GSL
                         PROPERTY INTERFACE_LINK_LIBRARIES
                         ms-gsl_Microsoft.GSL_GSL_DEPS_TARGET
                         APPEND)
        endif()

        set_property(TARGET Microsoft.GSL::GSL PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_LINKER_FLAGS_RELEASE}> APPEND)
        set_property(TARGET Microsoft.GSL::GSL PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_INCLUDE_DIRS_RELEASE}> APPEND)
        set_property(TARGET Microsoft.GSL::GSL PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_LIB_DIRS_RELEASE}> APPEND)
        set_property(TARGET Microsoft.GSL::GSL PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_COMPILE_DEFINITIONS_RELEASE}> APPEND)
        set_property(TARGET Microsoft.GSL::GSL PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${ms-gsl_Microsoft.GSL_GSL_COMPILE_OPTIONS_RELEASE}> APPEND)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET Microsoft.GSL::GSL PROPERTY INTERFACE_LINK_LIBRARIES Microsoft.GSL::GSL APPEND)

########## For the modules (FindXXX)
set(ms-gsl_LIBRARIES_RELEASE Microsoft.GSL::GSL)