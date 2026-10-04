# pct

potemkin package manager — простой CLI для pull-пакетов из
`github.com/potemkin-storage`.

Ставит пакеты в корень диска, ведёт глобальный реестр в `~/.pctc`
(Linux) / `C:\.pctc` (Windows), умеет обновлять и удалять.

---

## установка

**Linux:**

```bash
curl -fsSL https://raw.githubusercontent.com/potemkin-storage/pct/main/build.sh | bash
```

**Windows (PowerShell):**

```powershell
irm https://raw.githubusercontent.com/potemkin-storage/pct/main/build.ps1 | iex
```

Скрипты клонируют репу во временную папку, собирают бинарник, кладут
его в `PATH` и убирают за собой.

Куда именно ставится:

- Linux: `/usr/local/bin`, если доступно на запись, иначе `~/.local/bin`
  (и добавляется в `.bashrc` / `.zshrc`).
- Windows: `%USERPROFILE%\.pct\bin` (и добавляется в `PATH` пользователя).

Переопределить можно через `PCT_INSTALL_DIR`.

---

## быстрый старт

```bash
pct pull helloworld       # скачать пакет
pct update helloworld     # обновить один пакет
pct update                 # обновить всё, что установлено
pct remove helloworld     # снести пакет
```

---

## команды

### `pct pull <packet> [-o <dir>]`

Клонирует `https://github.com/potemkin-storage/<packet>` в корень диска
(`/` на Linux, `C:\` на Windows) или в `<dir>`, если указан `-o`.

После клона ищет в корне репы файл `*.pcmetadata`, читает его и выводит
информацию о пакете. Заодно регистрирует пакет в глобальном конфиге.

Если папка уже существует:

- и это git-репа → предложит `pct update <packet>`;
- и это что-то другое → предложит `pct pull <packet> -o <dir>`.

### `pct update [packet] [-o <dir>]`

Делает `git pull` в папке пакета. Без аргумента — проходит по всем
пакетам из `.pctc` и обновляет каждый.

### `pct remove <packet> [-o <dir>]`

Удаляет папку пакета и вычёркивает его из `.pctc`. Перед удалением
спрашивает подтверждение — потому что корень диска, мало ли.

### `pct help` / `pct version`

Справка и версия. То же самое, что `-h` / `--help` и `-v` / `--version`.

---

## формат метаданных

В корне каждой репы с пакетом лежит файл `<packet>.pcmetadata`:

```
name=helloworld
version=1.1
author=me
type=file
```

Поля:

| поле          | что значит                                              |
| ------------- | ------------------------------------------------------- |
| `name`        | человекочитаемое имя                                    |
| `version`     | версия, попадает в `.pctc`                              |
| `author`      | автор                                                   |
| `type`        | `file` — статичный пакет, `exc` — с install-командой    |

Для `type=exc` добавляются команды установки:

```
name=calculator
version=1.1
author=me
type=exc
install_cmd_linux=g++ -o calc main.cpp
install_cmd_wn=g++ main.cpp -o calc.exe
```

`pct` не запускает эти команды сам — только показывает, что нужно
выполнить, и где. Решение остаётся за тобой.

---

## конфиг

Глобальный файл: `~/.pctc` (Linux), `C:\.pctc` (Windows).

```
[packets]
helloworld=1.1
calculator=1.1
#комментарий

[config]
# ключи для будущих настроек
```

Секция `[packets]` — реестр установленного. Секция `[config]` — под
будущие настройки самого `pct`.

---

## сборка из исходников

Нужен `g++` с поддержкой C++17.

```bash
git clone https://github.com/potemkin-storage/pct
cd pct
make
```

Полезные цели:

```bash
make            # собрать pct
make clean      # снести .o и бинарник
make rebuild    # clean + build
make run        # собрать и показать pct help
make install    # поставить в /usr/local/bin (нужен sudo)
make help       # список целей
```

Переопределение:

```bash
CXX=clang++ make
CXXFLAGS="-std=c++17 -O0 -g" make
```

---

## структура проекта

```
pct/
├── Makefile
├── README.md
├── LICENSE
├── main.cpp
├── parser.h / parser.cpp
├── build.sh / build.ps1
└── src/
    ├── utils/
    │   ├── include/
    │   │   ├── colors.h
    │   │   ├── fs.h
    │   │   └── utils.h
    │   ├── fs.cpp
    │   └── utils.cpp
    └── commands/
        ├── include/
        │   ├── pull.h
        │   ├── update.h
        │   └── remove.h
        ├── pull.cpp
        ├── update.cpp
        └── remove.cpp
```

---

## лицензия

см. `LICENSE`.
