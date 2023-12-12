add_library(imgui)
target_sources(imgui
	PUBLIC
		${imgui_SOURCE_DIR}/imgui.h
		${imgui_SOURCE_DIR}/imstb_rectpack.h
		${imgui_SOURCE_DIR}/imstb_textedit.h
		${imgui_SOURCE_DIR}/imstb_truetype.h

	PRIVATE
		${imgui_SOURCE_DIR}/imgui.cpp
		${imgui_SOURCE_DIR}/imgui_draw.cpp
		${imgui_SOURCE_DIR}/imgui_internal.h
		${imgui_SOURCE_DIR}/imgui_tables.cpp
		${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

# Add backends in separate call to make it easier to swap them out later
target_sources(imgui
	PUBLIC
		${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
		${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h

	PRIVATE
		${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h

		${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui
	PUBLIC
		glfw
)

add_library(imgui_demo)
target_sources(imgui_demo
	PRIVATE
		${imgui_SOURCE_DIR}/imgui_demo.cpp
)
target_link_libraries(imgui_demo
	PUBLIC
		imgui
)