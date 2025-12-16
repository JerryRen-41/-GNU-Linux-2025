number-game: пример для задания по документированию

Сборка:
  autoreconf -fisv
  ./configure
  make

Запуск:
  ./src/number-game
  ./src/number-game -r
  ./src/number-game --help

Документация:
  make -C src doxygen-doc
  # открыть doxygen-doc/html/index.html в браузере

Man-страница:
  man ./man/number-game.1
