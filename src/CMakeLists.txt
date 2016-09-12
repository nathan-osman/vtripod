configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h")

set(SRC
    main.cpp
    mainwindow.h
    mainwindow.cpp
)

add_executable(vtripod WIN32 ${SRC})

set_target_properties(vtripod PROPERTIES
    CXX_STANDARD 11
)

target_link_libraries(vtripod Qt5::Widgets Qt5::Multimedia)

install(TARGETS vtripod
    RUNTIME DESTINATION BINARY_DIR
)
