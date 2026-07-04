#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include <unordered_map>
#include <chrono>
#include <mutex>

struct Record {
    std::string value;
    long long expiryTime; 
};

class Database {
private:
    std::unordered_map<std::string, Record> store;
    std::mutex db_mutex;                       //thread safety
    long long getCurrentTimeMs();
public:
    Database()=default;
    ~Database()=default;

    std::string processCommand(const std::string& input);       //parses and executes
    
    void set(const std::string& key,const std::string& value,int ttlSeconds=0);
    std::string get(const std::string& key);
    bool del(const std::string& key);
    
    bool saveToFile(const std::string& filepath);
    bool loadFromFile(const std::string& filepath);
    std::string displayAll();
};

#endif
