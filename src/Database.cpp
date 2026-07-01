#include "../include/Database.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

long long Database::getCurrentTimeMs() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

void Database::set(const std::string& key, const std::string& value, int ttlSeconds) {
    std::lock_guard<std::mutex> lock(db_mutex);
    long long expiry = (ttlSeconds > 0) ? getCurrentTimeMs() + (ttlSeconds * 1000LL) : 0;
    store[key] = {value, expiry};
}

std::string Database::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    auto it = store.find(key);
    if (it != store.end()) {
        if (it->second.expiryTime > 0 && getCurrentTimeMs() > it->second.expiryTime) {
            store.erase(it);
            return "(nil)";
        }
        return "\"" + it->second.value + "\"";
    }
    return "(nil)";
}

bool Database::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    return store.erase(key) > 0;
}

// ... (Keep saveToFile and loadFromFile the same as before, just add std::lock_guard<std::mutex> lock(db_mutex); at the top of each) ...

std::string Database::processCommand(const std::string& input) {
    std::vector<std::string> args;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) args.push_back(token);
    
    if (args.empty()) return "";
    for (char &c : args[0]) c = toupper(c);
    std::string cmd = args[0];

    if (cmd == "SET" && args.size() >= 3) {
        int ttl = (args.size() == 4) ? std::stoi(args[3]) : 0;
        set(args[1], args[2], ttl);
        return "OK";
    } else if (cmd == "GET" && args.size() == 2) {
        return get(args[1]);
    } else if (cmd == "DEL" && args.size() == 2) {
        return del(args[1]) ? "(integer) 1" : "(integer) 0";
    }
    return "ERR unknown command";
}