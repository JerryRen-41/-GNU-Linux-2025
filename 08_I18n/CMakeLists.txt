cmake_minimum_required(VERSION 3.10)
project(GuessI18N C)

set(CMAKE_C_STANDARD 11)

find_package(Gettext REQUIRED)
find_package(Intl REQUIRED)

add_executable(game src/game.c)
target_link_libraries(game PRIVATE ${Intl_LIBRARIES})
target_include_directories(game PRIVATE ${Intl_INCLUDE_DIRS})

set(LOCALE_BASE "${CMAKE_CURRENT_BINARY_DIR}/locale")
target_compile_definitions(game PRIVATE LOCALE_INSTALL_DIR=\"${LOCALE_BASE}\")

set(DOMAIN "gamenumber")
set(PO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/po")
set(POT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${DOMAIN}.pot")

add_custom_command(
    OUTPUT ${POT_FILE}
    COMMAND xgettext --keyword=T_ --from-code=UTF-8
            -o ${POT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/src/game.c
    COMMENT "Producing template POT file"
)

add_custom_target(update_pot ALL DEPENDS ${POT_FILE})

file(READ "${PO_DIR}/LINGUAS" LING_LIST)
string(STRIP "${LING_LIST}" LANGUAGES)

foreach(L ${LANGUAGES})
    set(PO_FILE "${PO_DIR}/${L}.po")
    set(MO_PATH "${LOCALE_BASE}/${L}/LC_MESSAGES")
    set(MO_FILE "${MO_PATH}/${DOMAIN}.mo")

    file(MAKE_DIRECTORY ${MO_PATH})

    add_custom_command(
        OUTPUT ${PO_FILE}.merged
        COMMAND msgmerge --update ${PO_FILE} ${POT_FILE}
        DEPENDS ${PO_FILE} ${POT_FILE}
        COMMENT "Merging updates into ${L}.po"
    )

    add_custom_command(
        OUTPUT ${MO_FILE}
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${MO_FILE} ${PO_FILE}
        DEPENDS ${PO_FILE}.merged
        COMMENT "Compiling translation for ${L}"
    )

    add_custom_target(locale_${L} ALL DEPENDS ${MO_FILE})
    add_dependencies(game locale_${L})
endforeach()

install(DIRECTORY ${LOCALE_BASE}/ DESTINATION share/locale)

