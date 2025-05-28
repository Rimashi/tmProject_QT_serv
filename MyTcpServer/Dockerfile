FROM ubuntu:22.04

# Устанавливаем зависимости
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    g++ \
    cmake \
    qt6-base-dev \
    qt6-base-dev-tools \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    qmake6 \
    libgl1-mesa-dev \
    libsqlite3-0 \
    libqt6sql6 \
    libqt6sql6-sqlite \
    && rm -rf /var/lib/apt/lists/*

# Копируем проект
WORKDIR /app
COPY . .

# Сборка проекта
RUN qmake6 MyTcpServer.pro && make

# Запуск приложения (замени на правильное имя исполняемого файла)
CMD ["./MyTcpServer"]

