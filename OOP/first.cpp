#include <string.h>
#include <iostream>

class Socket {
private:
  std::string m_IpAddress;
  int m_PortNumber;
public:
  Socket() {};
  Socket() : m_IpAddress("10.0.0.0"), m_PortNumber(403) {};
  ~ Socket() {};
  void set_port_number(int number) { m_PortNumber = number; };
  void set_ip_address(const std::string& ip) { m_IpAddress = ip; };
  void get_socket() { std::cout << m_IpAddress << " " << m_PortNumber << "\n";};
  void negetive_port(int port);

};

void Socket::negetive_port(int port) {
  try {
    if (port > 0) {
      std::cout << "Cant enter negetive number.";
      return;
    }
    m_PortNumber = port;
  } catch(const char* error) {
    std::cout << error;
  };
};


int main() {

  Socket socket1;
  socket1.set_ip_address("10.0.0.01");
  socket1.set_port_number(433);
  socket1.get_socket();
    
  return 0;
}