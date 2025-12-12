#include <iostream>
#include <fstream>
#include <chrono>
#include <array>
#include <cmath>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

int main(int, char**)
{
  std::ifstream f("input.test", std::ios::in);
  timer t;
  unsigned int ans = 0;
  unsigned int n0;
  unsigned int n1;
  unsigned int i0;
  t.start();
  for (std::string ss; std::getline(f, ss);)
    {
      i0 = 0;
      n0 = ss[0];
      for (unsigned int i = 1; i < ss.length() - 1; ++i)
        {
          if (n0 == 57)
            break;
          if (n0 < ss[i])
            {
              i0 = i;
              n0 = ss[i];
            }
        }
      i0 += 1;
      n1 = ss[i0];
      for (unsigned int i = i0 + 1; i < ss.length(); ++i)
        {
          if (n1 == 57)
            break;
          if (n1 < ss[i])
            {
              i0 = i;
              n1 = ss[i];
            }
        }
      ans += 10 * (n0 - 48) + n1 - 48;
    }
  t.stop("Part 1: ");
  std::cout << ans << '\n';

  unsigned long long ans2 = 0;
  unsigned int i1;
  unsigned int ns;
  std::ifstream f2("input.test", std::ios::in);
  t.start();
  for (std::string ss; std::getline(f2, ss);)
    {
      i1 = 0;
      for (unsigned int v = 0; v < 12; ++v)
        {
          ns = ss[i1];
          for (unsigned int i = i1 + 1; i < ss.length() - 11 + v; ++i)
            {
              if (ns == 57)
                break;
              if (ns < ss[i])
                {
                  i1 = i;
                  ns = ss[i];
                }
            }
          i1 += 1;
          ans2 += (unsigned long long)(ns - 48)*std::pow(10, 11 - v);
        }
    }
  t.stop("Part 2: ");
  std::cout << ans2 << '\n';
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
