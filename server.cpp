#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <unordered_map>

#define PORT 8080
#define BUFFER_SIZE 4096

std::mutex cout_mutex;

// Detect file type and return appropriate MIME type
std::string get_content_type(const std::string &path) {
    if (path.ends_with(".html") || path.ends_with(".htm")) return "text/html";
    if (path.ends_with(".css")) return "text/css";
    if (path.ends_with(".js")) return "application/javascript";
    if (path.ends_with(".json")) return "application/json";
    if (path.ends_with(".png")) return "image/png";
    if (path.ends_with(".jpg") || path.ends_with(".jpeg")) return "image/jpeg";
    if (path.ends_with(".gif")) return "image/gif";
    if (path.ends_with(".txt")) return "text/plain";
    return "application/octet-stream";
}

// Read the requested file and build HTTP response
std::string get_http_response(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    std::string body;

    if (!file.is_open()) {
        body = "<h1>404 Not Found</h1>";
        return "HTTP/1.1 404 Not Found\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n\r\n" + body;
    }

    std::string extension_type = get_content_type(path);

    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string content(file_size, '\0');
    file.read(&content[0], file_size);
    file.close();

    std::string response;
    response += "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: " + extension_type + "\r\n";
    response += "Connection: close\r\n";
    response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
    response += "\r\n";
    response += content;

    return response;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    read(client_socket, buffer, BUFFER_SIZE);
    std::string request(buffer);

    std::string first_line = request.substr(0, request.find("\r\n"));
    std::string path = "index.html";

    if (first_line.find("GET") != std::string::npos) {
        size_t start = first_line.find(" ") + 1;
        size_t end = first_line.find(" ", start);
        std::string requested = first_line.substr(start, end - start);
        if (requested != "/") path = requested.substr(1);
    }

    std::string response = get_http_response(path);
    write(client_socket, response.c_str(), response.size());
    close(client_socket);

    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "[+] Served " << path << " on thread "
              << std::this_thread::get_id() << std::endl;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on http://localhost:" << PORT << std::endl;

    std::vector<std::thread> threads;
    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        threads.emplace_back(handle_client, client_socket);
    }

    for (auto &t : threads) {
        if (t.joinable()) t.join();
    }

    return 0;
}
