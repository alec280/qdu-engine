function(MakeExample TARGETNAME FILENAME)
    add_executable(${TARGETNAME} ${FILENAME})
    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD 20)
    set_property(TARGET ${TARGETNAME} PROPERTY FOLDER Examples)
    target_include_directories(${TARGETNAME} PRIVATE ${QDU_INCLUDE_DIRECTORY} ${THIRD_PARTY_INCLUDE_DIRECTORIES})
    target_link_libraries(${TARGETNAME} PRIVATE QDUEngine)
    add_custom_command(TARGET ${TARGETNAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:OpenAL> $<TARGET_FILE_DIR:${TARGETNAME}>)
endfunction(MakeExample)

MakeExample(Demo demo.cpp)
