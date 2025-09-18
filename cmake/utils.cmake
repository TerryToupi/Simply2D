function(organize_target TARGET FOLDER)
    if (NOT TARGET ${TARGET})
        return()
    endif()

    # If it's an alias, resolve the actual target
    get_target_property(alias_real ${TARGET} ALIASED_TARGET)
    if (alias_real)
        set(real_target ${alias_real})
    else()
        set(real_target ${TARGET})
    endif()

    # Set folder property on the real target
    set_property(TARGET ${real_target} PROPERTY FOLDER "${FOLDER}")
endfunction()

# Get all directories below the specified root directory.
#   _result     : The variable in which to store the resulting directory list
#   _root       : The root directory, from which to start.
#
macro(get_directories _result _root)
    file(GLOB_RECURSE dirs RELATIVE ${_root} LIST_DIRECTORIES ON ${_root}/*)
    foreach(dir ${dirs})
        if(IS_DIRECTORY ${dir})
            list(APPEND ${_result} ${dir})
        endif()
    endforeach()
endmacro()

#
# Get all targets defined at the specified directory (level).
#   _result     : The variable in which to store the resulting list of targets.
#   _dir        : The directory to query for targets.
#
macro(get_targets_by_directory _result _dir)
    get_property(_target DIRECTORY ${_dir} PROPERTY BUILDSYSTEM_TARGETS)
    set(_result ${_target})
endmacro()

#
# Get all targets defined below the specified root directory.
#   _result     : The variable in which to store the resulting list of targets.
#   _root_dir   : The root project root directory
#
macro(get_all_targets _result _root_dir)
    get_directories(_all_directories ${_root_dir})
    foreach(_dir ${_all_directories})
        get_targets_by_directory(_target ${_dir})
        if(_target)
            list(APPEND ${_result} ${_target})
        endif()
    endforeach()
endmacro()