set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
define_property(
        TARGET
        PROPERTY FOLDER
        INHERITED
        BRIEF_DOCS "Set the folder name."
        FULL_DOCS  "Use to organize targets in an IDE."
)

function(add_subdirectory_with_folder _folder_name _folder)
    add_subdirectory(${_folder} ${ARGN})
    set_property(DIRECTORY "${_folder}" PROPERTY FOLDER "${_folder_name}")
endfunction()

add_subdirectory_with_folder("thirdParty" glad)
add_subdirectory_with_folder("thirdParty" glfw)
add_subdirectory_with_folder("thirdParty" imgui)
add_subdirectory_with_folder("thirdParty" stb)
add_subdirectory_with_folder("thirdParty" json)
add_subdirectory_with_folder("thirdParty/OpenAL" openal-soft)
add_subdirectory_with_folder("thirdParty" dr_wav)
