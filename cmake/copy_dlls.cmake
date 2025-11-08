# Function to copy DLLs from linked libraries
function(copy_dlls_from_linked_targets target)
    # Get all linked libraries
    get_target_property(linked_libs ${target} LINK_LIBRARIES)
    
    if(linked_libs)
        foreach(lib ${linked_libs})
            # Check if this is a target (not just a file path or system library)
            if(TARGET ${lib})
                # Check if it's a shared library
                get_target_property(lib_type ${lib} TYPE)
                if(lib_type STREQUAL "SHARED_LIBRARY")
                    add_custom_command(TARGET ${target} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            $<TARGET_FILE:${lib}>
                            $<TARGET_FILE_DIR:${target}>
                        COMMENT "Copying DLL: ${lib}"
                    )
                endif()
            endif()
        endforeach()
    endif()
endfunction()