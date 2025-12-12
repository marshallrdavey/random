#include <chrono>
#include <iostream>
#include <string>
#include <fstream>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

int main(int, char**)
{
  std::ifstream f1("input.test", std::ios::in);
  int no = 50;
  unsigned int count = 0;

  // Part 1:
  timer t;
  t.start();
  for (std::string ln; f1 >> ln;)
    {
      no += ln[0] == 'L' ? -std::stoi(ln.substr(1)) : std::stoi(ln.substr(1));
      
      no %= 100;
      if (no == 0)
        count++;
    }
  t.stop("Part 1: ");
  std::cout << count << '\n';
  

  std::ifstream f2("input.test", std::ios::in);
  no = 50;
  count = 0;
  int no_new;
  // Part 2:
  t.start();
  for (std::string ln; f2 >> ln;)
    {
      no_new = no + (ln[0] == 'L' ? -std::stoi(ln.substr(1)) : std::stoi(ln.substr(1)));
      
      const unsigned int c_d = std::abs(no_new / 100);
      count += c_d;

      if (no_new * no < 0)
        count++;

      no_new %= 100;
      if (no_new == 0 && c_d == 0)
        count++;

      no = no_new;
    }
  t.stop("Part 2: ");
  std::cout << count << '\n';
}

inline void
timer::start()
{
  m_time = std::chrono::high_resolution_clock::now();
}

inline void
timer::stop(const std::string &msg) const
{
  std::cout << msg <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::high_resolution_clock::now() - m_time).count() << " us\n";
}
