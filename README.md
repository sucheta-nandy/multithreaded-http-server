# 🚀 Multithreaded C++ Web Server

A lightweight, high-performance **multithreaded HTTP server** built completely in **C++**, designed to handle multiple concurrent client requests efficiently.  

<img width="1190" height="744" alt="image" src="https://github.com/user-attachments/assets/aa2e1b25-946e-42a4-8cdb-db774ecbb7a4" />

## ⚙️ Features

✅ Serves multiple clients **concurrently** using `std::thread`  
✅ Handles **HTML, CSS, JS, JSON, PNG, JPG, and TXT** files  
✅ Implements **HTTP/1.1 response headers** and `Content-Type` detection  
✅ Fully functional **file I/O** and binary file handling  
✅ Minimal dependencies — runs on any Linux/Mac system with a C++ compiler  

## Core Concepts Demonstrated

- TCP/IP **Socket Programming**  
- **Multithreading** and concurrency control  
- **HTTP protocol fundamentals**  
- **Mutex synchronization** for thread-safe console output  
- **File handling** (text and binary) in C++  
- Cross-platform development principles

## 🏗️ Build & Run

### 1️⃣ Compile the server
Use the latest compiler supporting C++20 (or include the helper function if not):

In the Terminal:
g++ -std=c++20 server.cpp -o server -pthread
./server

Go To:
http://localhost:8080

When running:
<img width="419" height="190" alt="image" src="https://github.com/user-attachments/assets/8f624c29-2c19-47aa-8476-614e60c7f1e7" />


