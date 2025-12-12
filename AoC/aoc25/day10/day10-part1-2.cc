#include <chrono>
#include <iostream>
#include <fstream>

#include <array>
#include <cstdint>
#include <numeric>
#include <vector>
#include <tuple>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

/**
 * Iteration for n choose p. Return false when restarted.
 */
bool
combinations (std::vector<std::uint16_t> &a,
              const std::size_t           n,
              const std::size_t           p)
{
  for (std::size_t i = p - 1; i != -1; --i)
    if(a[i] < n + i - p)
      {
        for (std::size_t j = a[i] + 1; i < p; ++i, ++j)
          a[i] = j;
        return true;
      }
  for (std::size_t i = 0; i < p; ++i)
    a[i] = i;
  return false;
}

/**
 * Iteration for n choose p. Return false when restarted.
 */
bool
combinations (std::uint16_t     *a,
              const std::size_t  n,
              const std::size_t  p)
{
  for (std::size_t i = p - 1; i != -1; --i)
    if(a[i] < n + i - p)
      {
        for (std::size_t j = a[i] + 1; i < p; ++i, ++j)
          a[i] = j;
        return true;
      }
  for (std::size_t i = 0; i < p; ++i)
    a[i] = i;
  return false;
}

int main(int, char**)
{
  std::ifstream f("input.test", std::ios::in);
  unsigned int ans = 0;
  timer t;
  t.start();
  for (std::string ln; std::getline(f, ln);)
    {
      std::uint16_t lb = 0;
      for (unsigned int i = ln.find('[') + 1; i < ln.find(']'); ++i)
        if (ln[i] == '#')
          lb |= 1 << i - ln.find('[') - 1;
      std::vector<std::uint16_t> bs;
      std::string::size_type p0 = ln.find('(');
      std::string::size_type p1 = ln.find(')');
      std::uint16_t cc = 0;
      while(p0 != std::string::npos || p1 != std::string::npos)
        {
          bs.push_back(0);
          const auto ts = ln.substr(p0 + 1, p1 - p0 - 1);
          p0 = ln.find('(', p1);
          p1 = ln.find(')', p0);
          std::string::size_type c0 = 0;
          std::string::size_type c1 = ts.find(',', c0);
          while (c1 != std::string::npos)
            {
              bs.back() |= 1 << std::stoul(ts.substr(c0, c1 - c0));
              c0 = c1 + 1;
              c1 = ts.find(',', c0);
            }
          bs.back() |= 1 << std::stoul(ts.substr(c0));
        }

      // Third attempt
      // Use combinations (xor commutes).
      // Comments for using C-style arrays instead.
      std::vector<std::uint16_t> cs;
      // uint16_t* cs;
      std::uint16_t trial;
      bool found = false;
      for (unsigned int i = 1; i < bs.size() + 1; ++i)
        {
          cs.resize(i, i - 1);
          // std::iota(cs.begin(), cs.end(), (std::uint16_t)0);
          // cs = new std::uint16_t[i];
          // std::iota(cs, cs + i, (std::uint16_t)0);
          do 
            {
              trial = 0;
              for (const auto & c : cs)
                trial ^= bs[c];
              // for (std::size_t j = 0; j < i; ++j)
              //  trial ^= bs[cs[j]];
              if (trial == lb)
                {
                  found = true;
                  ans += i;
                  break;
                }
            }
          while (combinations(cs, bs.size(), i));
          // delete [] cs;
          if (found)
            break;
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
