# C++ HTTP Server Library (WinSock)

A lightweight **HTTP server library written in modern C++**, built **from scratch on top of TCP using WinSock (Windows)**.

This project is designed as a **library**, not a monolithic application.  
Its goal is to provide a clean, extensible, and well-structured HTTP framework while focusing on **architecture, multithreading, and correctness**.

---

## Features

- Native TCP server using **WinSock**
- HTTP/1.1 basic support
- Clean routing system (`GET` endpoints)
- Thread pool with configurable limits
- Clear separation of concerns
- Simple public API
- Designed to be extended internally (POST, middleware, keep-alive, etc.)

---

## Architecture Overview

```
User Code
   ↓
HttpServer (Public API)
   ↓
Router
   ↓
ServerCore (TCP + ThreadPool)
   ↓
Socket (WinSock)
```

---

## Getting Started

### Example Usage

```cpp
#include "http/HttpServer.h"

int main() {
    HttpServer server(8080);

    server.get("/ping", [](const HttpRequest&) {
        HttpResponse res;
        res.body = R"({"pong": true})";
        return res;
    });

    server.start();
}
```

### Test with curl

```bash
curl http://localhost:8080/ping
```

---

## Thread Pool

The server uses an internal **thread pool** with configurable minimum and maximum thread limits.
This allows handling multiple concurrent connections safely and efficiently.

---

## Source Code Policy

This project is **source-available**, but **not open-source**.

You are allowed to:
- Use the library
- Compile and link it in your own projects
- Study the implementation for learning purposes

You are **NOT allowed** to:
- Modify the source code
- Redistribute modified versions
- Create derivative works
- Submit pull requests or forks with changes

This policy may change in future versions.

---

## Current Limitations

- No HTTP keep-alive
- Only basic GET routing
- No HTTPS
- No request body parsing (POST)

---

## Platform

- Windows only (WinSock)
- C++17 or newer
- Visual Studio recommended

---

## Project Purpose

This project was created to:
- Understand how HTTP servers work internally
- Practice multithreading and concurrency
- Design a clean C++ library architecture
- Avoid black-box frameworks

---

## License

All Rights Reserved.

Copyright (c) 2025 MrOnionOri

---

## Author

Developed by **MrOnionOri**
