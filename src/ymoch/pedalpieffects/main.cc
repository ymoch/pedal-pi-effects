#include <chrono>
#include <iostream>
#include <thread>

int main() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
}
