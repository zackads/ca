# code

## Setup

```sh
brew install clang-format llvm
git config core.hooksPath .githooks
```

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Shortcuts

Run from the project root (the root `Makefile` forwards to `build/`, configuring it if needed):

```sh
make                # build
make format         # format all sources in place
make format-check   # fail if anything is unformatted
make lint           # run clang-tidy
make clean          # delete build/
```

From another directory, use `make -C <project root> <target>`. Wrap hand-aligned code in `// clang-format off` / `// clang-format on` to keep the formatter away from it.

## Format

```sh
clang-format -i *.c *.h                          # format in place
clang-format --dry-run --Werror *.c *.h          # check only
```

## Lint

```sh
cmake -S . -B build -DENABLE_CLANG_TIDY=ON && cmake --build build   # lint during build
/opt/homebrew/opt/llvm/bin/clang-tidy -p build \
  --extra-arg=-isysroot"$(xcrun --show-sdk-path)" *.c              # lint directly
```
