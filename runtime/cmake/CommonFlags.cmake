#
#//===----------------------------------------------------------------------===//
#//
#//                     The LLVM Compiler Infrastructure
#//
#// This file is dual licensed under the MIT and the University of Illinois Open
#// Source Licenses. See LICENSE.txt for details.
#//
#//===----------------------------------------------------------------------===//
#

# This file holds the common flags independent of compiler
# The flag types are: 
#   1) Assembly flags          (append_asm_flags_common)
#   2) C/C++ Compiler flags    (append_c_and_cxx_flags_common)
#   3) Fortran Compiler flags  (append_fort_flags_common)
#   4) Linker flags            (append_linker_flags_common)
#   5) Archiver flags          (append_archiver_flags_common)

# These append_* macros all append to the corresponding list variable holding the flags.
macro(append_c_flags new_flag)
    list(APPEND local_c_flags    "${new_flag}")
endmacro()

macro(append_cxx_flags new_flag)
    list(APPEND local_cxx_flags  "${new_flag}")
endmacro()

macro(append_c_and_cxx_flags new_flag)
    append_c_flags("${new_flag}")
    append_cxx_flags("${new_flag}")
endmacro()

macro(append_asm_flags new_flag)
    list(APPEND local_asm_flags  "${new_flag}")
endmacro()

macro(append_fort_flags new_flag)
    list(APPEND local_fort_flags "${new_flag}")
endmacro()

# The difference between linker_flags and linker_flags_libs is linker_flags_libs
# is put at the end of the linker command where linking libraries should be done.
macro(append_linker_flags new_flag)
    list(APPEND local_ld_flags   "${new_flag}")
endmacro()

macro(append_linker_flags_library new_flag)
    list(APPEND local_ld_flags_libs "${new_flag}")
endmacro()

macro(append_archiver_flags new_flag)
    list(APPEND local_ar_flags   "${new_flag}")
endmacro()

#########################################################
# Global Assembly flags
function(append_asm_flags_common input_asm_flags)
    set(local_asm_flags)
    set(${input_asm_flags} "${${input_asm_flags}}" "${local_asm_flags}" "${LIBOMP_ASMFLAGS}" PARENT_SCOPE)
endfunction()

#########################################################
# Global C/C++ Compiler flags
function(append_c_and_cxx_flags_common input_c_flags input_cxx_flags)
    set(local_c_flags)
    set(local_cxx_flags)
    set(${input_c_flags}   "${${input_c_flags}}"   "${local_c_flags}"   "${LIBOMP_CFLAGS}"   PARENT_SCOPE)
    set(${input_cxx_flags} "${${input_cxx_flags}}" "${local_cxx_flags}" "${LIBOMP_CXXFLAGS}" PARENT_SCOPE)
endfunction()

#########################################################
# Global Fortran Compiler flags (for creating .mod files)
function(append_fort_flags_common input_fort_flags)
    set(local_fort_flags)
    set(${input_fort_flags} "${${input_fort_flags}}" "${local_fort_flags}" "${LIBOMP_FFLAGS}" PARENT_SCOPE)
endfunction()

#########################################################
# Linker flags
function(append_linker_flags_common input_ld_flags input_ld_flags_libs)
    set(local_ld_flags)
    set(local_ld_flags_libs)

    if(${LIBOMP_USE_PREDEFINED_LINKER_FLAGS})

        #################################
        # Windows linker flags
        if(${WINDOWS}) 

        ##################
        # MAC linker flags
        elseif(${MAC})
            append_linker_flags("-single_module")
            append_linker_flags("-current_version ${LIBOMP_VERSION}.0")
            append_linker_flags("-compatibility_version ${LIBOMP_VERSION}.0")
        #####################################################################################
        # Intel(R) Many Integrated Core Architecture (Intel(R) MIC Architecture) linker flags
        elseif(${MIC})
            append_linker_flags("-Wl,-x")
            append_linker_flags("-Wl,--warn-shared-textrel") #  Warn if the linker adds a DT_TEXTREL to a shared object.
            append_linker_flags("-Wl,--as-needed")
            append_linker_flags("-Wl,--version-script=${src_dir}/exports_so.txt") # Use exports_so.txt as version script to create versioned symbols for ELF libraries
            if(NOT ${STUBS_LIBRARY})
                append_linker_flags_library("-pthread") # link in pthread library
            endif()
            if(${LIBOMP_STATS})
                append_linker_flags_library("-Wl,-lstdc++") # link in standard c++ library (stats-gathering needs it)
            endif()
        #########################
        # Unix based linker flags
        else()
            # For now, always include --version-script flag on Unix systems.
            append_linker_flags("-Wl,--version-script=${src_dir}/exports_so.txt") # Use exports_so.txt as version script to create versioned symbols for ELF libraries
            append_linker_flags("-Wl,-z,noexecstack") #  Marks the object as not requiring executable stack.
            append_linker_flags("-Wl,--as-needed")    #  Only adds library dependencies as they are needed. (if libomp actually uses a function from the library, then add it)
            if(NOT ${STUBS_LIBRARY})
                append_linker_flags("-Wl,--warn-shared-textrel") #  Warn if the linker adds a DT_TEXTREL to a shared object.
                append_linker_flags("-Wl,-fini=__kmp_internal_end_fini") # When creating an ELF executable or shared object, call NAME when the 
                                                                         # executable or shared object is unloaded, by setting DT_FINI to the 
                                                                         # address of the function.  By default, the linker uses "_fini" as the function to call.
                append_linker_flags_library("-pthread") # link pthread library
            endif()
        endif() # if(${OPERATING_SYSTEM}) ...

    endif() # LIBOMP_USE_PREDEFINED_LINKER_FLAGS

    set(${input_ld_flags}      "${${input_ld_flags}}"      "${local_ld_flags}"      "${LIBOMP_LDFLAGS}"     PARENT_SCOPE)
    set(${input_ld_flags_libs} "${${input_ld_flags_libs}}" "${local_ld_flags_libs}" "${LIBOMP_LIBFLAGS}" PARENT_SCOPE)
endfunction()

#########################################################
# Archiver Flags
function(append_archiver_flags_common input_ar_flags)
    set(local_ar_flags)
    set(${input_ar_flags} "${${input_ar_flags}}" "${local_ar_flags}" PARENT_SCOPE)
endfunction()

