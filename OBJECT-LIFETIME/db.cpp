#include <iostream>
#include <vector>
#include <memory>
#include <utility>

// RULE OF SIX
class DatabaseConnection {
public:
  DatabaseConnection()
    : m_connection_id(-1), m_is_connected(false) {}

  DatabaseConnection(const std::string& connstr)
    : m_connection_string(connstr) {}

  // creating a new object from an existing object - copy ctor.
  // DatabaseConnection(const DatabaseConnection& other)
  //   : m_connection_id(other.m_connection_id), m_is_connected(other.m_is_connected) {}

  DatabaseConnection(DatabaseConnection&& other) noexcept
    : m_connection_id(other.m_connection_id), m_is_connected(other.m_is_connected) {
      other.m_connection_id = -1;
      other.m_is_connected = false;
  };

  ~DatabaseConnection() { if (m_is_connected) { m_is_connected = false; } }

  DatabaseConnection(const DatabaseConnection&) = delete;
  DatabaseConnection& operator=(const DatabaseConnection&) = delete;

  // DatabaseConnection& operator=(const DatabaseConnection& other) {
  //   if (this != &other) {
  //     m_connection_id = other.m_connection_id;
  //     m_is_connected = other.m_is_connected;
  //   } return *this;
  // };

  DatabaseConnection& operator=(DatabaseConnection&& other) noexcept {
    if (this != &other) {
      m_connection_id = std::move(other.m_connection_id);
      m_is_connected = std::move(other.m_is_connected);
      m_connection_string = std::move(other.m_connection_string);
      other.m_connection_id = -1;
      other.m_is_connected = false;
    } return *this;
  };

  void connect(int id) {
    m_connection_id = id;
    m_is_connected = true;
    std::cout << "Connection " << m_connection_id << " established.\n";
  }

  void query(const std::string& sql) {
    if (m_is_connected) { std::cout << "Query"; } // further implement.
  }

private:
  int m_connection_id;
  bool m_is_connected;
  std::string m_connection_string
};

// RULE OF ZERO
class DatabaseConnectionManager {
private:
  std::vector<std::shared_ptr<DatabaseConnection> > connetions;
public:
  void m_add_connection(int id) {
    connetions.emplace_back(std::make_shared<DatabaseConnection>);
  }
};

// UNIQUE PTR FOR MANAGING CONNECTION
// EXCLUSIVE OWNERSHIP
// AUTO RESOURCE MANAGEMENT
class ConnectionManager {
public:
  void m_create_connection() {
    conn = std::make_unique<DatabaseConnection>();
  }
  void m_release_connection() {
    conn.reset(); // unique ptr method.
  }
  void m_execute_query(const std::string& sql) {
    if (conn) {
      conn->query(sql);
    } else { std::cout << "No Active Connection.\n"; }
  }
private:
  std::unique_ptr<DatabaseConnection> conn;
};

// SHARED PTR EXAMPLE
class Config {
public:
  Config() { std::cout << "Config Loaded.\n"; }
  ~Config() { std::cout << "Config Unloaded.\n"; }
  void print() const { std::cout << "Config Settings.\n"; }
};

class Session {
public:
  Session(std::shared_ptr<Config> config)
    : config(config) {}
  
  void m_use_config() { config->print(); }
private:
  std::shared_ptr<Config> config;
};

// auto config = make_shared<Config>();
// Session session1(config);
// Session session2(config);

// WEAK PTR EXAMPLE
// NON OWNING REFRENCE, REFRENCES A RESOURCE MANAGED BY SHARED_PTR WITHOUT OWNING IT
// PREVENTS CYCLIC REFRENCES, AVOIDS MEMORY LEAKS CAUSE BY CYCLIC DEPENDENCIES.
class Publisher;

class Subscriber {
public:
  Subscriber(std::shared_ptr<Publisher> publisher)
  : publisher(publisher) {}
  void m_notify() {
    if (auto pub = publisher.lock()) {
      std::cout << "Subscriber notified by publisher.\n"; 
    } else {
      std::cout << "publisher is no longer available.\n";
    }
  }
private:
  std::weak_ptr<Publisher> publisher;
};

class Publisher : public std::enable_shared_from_this<Publisher> {
public:
  void m_add_subscriber(std::shared_ptr<Subscriber> subscriber) {
    subscribers.push_back(subscriber);
  }
  void m_notify_subscribers() {
    for (const auto& sub : subscribers) {
      sub->m_notify();
    }
  }
private:
  std::vector<std::shared_ptr<Subscriber> > subscribers;
};

// PERFECT FORWARDING EXAMPLE
class ConnectionPool {
public:

  ~ConnectionPool() {
    for (auto& conn : pool) {
      conn.~DatabaseConnection();
    }
  }

  template <typename T>
  void m_add_connection(T&& conn) {
    pool.emplace_back(std::forward<T>(conn));
  }

  void m_show_connections() {
    for (const auto& conn: pool) {
      std::cout << "Connection: " << conn.m_connection_string << ".\n";
    }
  }
private:
  std::vector<DatabaseConnection> pool;
};
