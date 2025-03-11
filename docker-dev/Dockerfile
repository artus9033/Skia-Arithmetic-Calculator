FROM ubuntu:24.04

RUN apt update
RUN apt install -y curl wget

# install build tools; python-is-python3 is for aliasing python to python3
RUN apt install -y make cmake gcc g++ build-essential vim git python-is-python3 ninja-build clang clang-format clang-tidy lldb

# install rust
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

# add .cargo/bin to PATH
ENV PATH="/root/.cargo/bin:${PATH}"

# install just
RUN cargo install just

# install fontconfig, Arial font & friends for runtime (running the app)
RUN echo "ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula select true" | debconf-set-selections
RUN apt install -y fontconfig fonts-dejavu fonts-freefont-ttf ttf-mscorefonts-installer

# install dependencies of Skia and Qt
RUN apt install -y doxygen xorg-dev libboost1.83-dev \
    graphviz libharfbuzz-dev libfreetype6-dev libxcb1-dev libxcb-util-dev \
    libglx-dev libgl1-mesa-dev libxcb-xkb-dev qt6-base-dev \
    libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev \
    libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libx11-xcb-dev libxcb-glx0-dev \
    libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync0-dev \
    libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-render-util0-dev \
    libxkbcommon-dev libxkbfile-dev libjpeg-dev libwebp-dev libtiff-dev libpng-dev

# rebuild fonts cache
RUN fc-cache -f
