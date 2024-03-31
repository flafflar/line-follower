include(ExternalProject)

ExternalProject_Add(picotool_project
	GIT_REPOSITORY https://github.com/raspberrypi/picotool.git
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
)

function(picotool_flash_target TARGET)
	get_target_property(target_name ${TARGET} OUTPUT_NAME)
	if(NOT ${target_name})
		get_target_property(target_name ${TARGET} NAME)
	endif()
	install(CODE "execute_process(COMMAND ${CMAKE_BINARY_DIR}/bin/picotool load -f ${target_name}.uf2)")
endfunction()
