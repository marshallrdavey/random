#include <chrono>
#include <iostream>
#include <fstream>

#include <vector>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

int main(int, char**)
{
  // Gimmic day. Does not hold for test input. Only one part.
  std::fstream f("input.test", std::ios::in);
  // const std::vector<unsigned int> as({7, 7, 6, 7, 5, 7});
  // const std::vector<unsigned int> as({9, 9, 9, 9, 9, 9});
  unsigned int ans = 0;
  unsigned int tmp = 0;
  timer t;
  t.start();
  for (std::string ln; std::getline(f, ln);)
    {
      tmp = 0;
      if (ln.find('x') == std::string::npos)
        continue;
      auto p0 = ln.find('x');
      auto p1 = ln.find(':');
      const unsigned int area = std::stoul(ln.substr(0, p0)) *
                                 std::stoul(ln.substr(p0 + 1, p1 - p0 - 1));
      p0 = ln.find(' ');
      // std::size_t count = 0;
      while (p0 != std::string::npos)
        {
          tmp += std::stoul(ln.substr(p0 + 1, 2)) * 9;//as[count++];
          p0 = ln.find(' ', p0 + 1);
        }
      if (tmp <= area)
      {
        ++ans;
        // std::cout << ((double)area - (double)tmp)/(double)area << '\n';
      }
    }
  t.stop("Part 1: ");
  std::cout << ans << '\n';
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
