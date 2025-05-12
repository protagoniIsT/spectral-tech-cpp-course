# Environment

Fork this repository and work in this fork. Sometimes you'll have to get some updates from the main repository (to receive new tasks). After cloning the fork to your pc locally execute

```bash
git remote add main_repo git@gitlab.spectral.tech:study/effective-cpp-2025.git
# or git remote add main_repo https://gitlab.spectral.tech/study/effective-cpp-2025.git
```

After that you can use `git pull main_repo main` to receive updates from the main repo.

Execute `git submodule update --init --recursive` inside `effective-cpp-2025` directory after cloning.

All the tasks are designed in a way that you can do them locally on your pc.
However you need Linux or Mac OS in order to do that. Also make sure the architecture is x86-64 (check with `uname -a`), otherwise you won't be able
to complete some of the tasks. ~~Rest in peace if you have Apple M1.~~

<details>
<summary>For Apple M1 users</summary>

In order to test some tasks, that require x64 (mostly layout and gdb weeks), you can use docker like this by also passing the current directory to `/effective-cpp`:

```bash
docker run -it --rm --platform linux/amd64 \
    --name "effective-cpp" \
    --volume "$(pwd):/effective-cpp:rw" \
    ubuntu:24.04
```

And then run tests inside this container (you might want to install `build-essential` and `cmake` there). You don't need this
for the majority of tasks though.

</details>

Only a few number of tasks require the usage of C++20, otherwise C++17 is enough. In case you want a newer compiler you may install `clang-18` like this (on Ubuntu):

1. First, install dependencies

```bash
apt update \
  && apt install sudo lsb-release wget software-properties-common gnupg -y
```

2. Add [LLVM repo](https://apt.llvm.org/)

```bash
cd /tmp \
  && wget https://apt.llvm.org/llvm.sh \
  && sudo chmod +x ./llvm.sh
```

3. Install LLVM with preferred version:

```bash
# clang-18 or any version you prefer
sudo ./llvm.sh 18 all

```

And then use `clang++-18` (or whichever version you got) for compilation. You may also want to have a newer version of your standard library in case you want to use some c++20
features (although it's not required). Upgrading it is a complicated process and you can easily break your os if you don't know what are you doing exactly. So if you really want a newer version, I'd suggest using a docker container with a newer version of OS installed (Ubuntu 24.04 for a example):

```bash
docker run -it --rm \
    --name "effective-cpp" \
    --volume "$(pwd):/effective-cpp:rw" \
    ubuntu:24.04
```

You can check if you have a recent version by `#include <concepts>`.

Every task is located in its own directory (check <https://gitlab.spectral.tech/study/effective-cpp-2025/-/blob/main/tasks/multiplication> for a example) and all the tasks belong to a single cmake project.

You only need to push the files you've been asked to modify in the task. You may also push anything else for your convinience, but only the mentioned files will be used in testing.

When you finish a task make sure the tests are passed in both `Release` and `ASAN` targets (check `multiplication` for the details). There are a few
exceptions to this rule, and they are always mentioned in a task's description.
