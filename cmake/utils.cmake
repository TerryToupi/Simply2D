function(organize_target TARGET FOLDER)
    if (NOT TARGET ${TARGET})
        message(WARNING "Target ${TARGET} does not exist")
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