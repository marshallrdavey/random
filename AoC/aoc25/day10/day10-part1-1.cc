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

unsigned int
permute(std::uint16_t                                   count,
        const std::vector<std::array<std::uint16_t, 2>> &in,
        const std::uint16_t                             &ans,
        const std::vector<std::array<std::uint16_t, 2>> &ls)
{
  ++count;
  std::vector<std::array<std::uint16_t, 2>> result;
  for (const auto & l : ls)
    for (const auto & i : in)
      {
        if (l[0] & i[0])
          continue;
        result.push_back({0, 0});
        result.back()[0] = l[0] | i[0];
        result.back()[1] = l[1] ^ i[1];
        if (result.back()[1] == ans)
          return count;
      }
  return permute(count, result, ans, ls);
};

bool
permute_no_recursion(std::vector<std::array<std::uint16_t, 2>> &in,
                     const std::uint16_t                       &ans,
                     const std::vector<std::array<std::uint16_t, 2>> &ls)
{
  std::vector<std::array<std::uint16_t, 2>> result;
  for (const auto & l : ls)
    for (const auto & i : in)
      {
        if (l[0] & i[0])
          continue;
        result.push_back({0, 0});
        result.back()[0] = l[0] | i[0];
        result.back()[1] = l[1] ^ i[1];
        if (result.back()[1] == ans)
          return true;
      }
  in = result;
  return false;
};

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
      std::vector<std::array<std::uint16_t, 2>> bs;
      std::string::size_type p0 = ln.find('(');
      std::string::size_type p1 = ln.find(')');
      std::uint16_t cc = 0;
      while(p0 != std::string::npos || p1 != std::string::npos)
        {
          bs.push_back({0, 0});
          const auto ts = ln.substr(p0 + 1, p1 - p0 - 1);
          p0 = ln.find('(', p1);
          p1 = ln.find(')', p0);
          std::string::size_type c0 = 0;
          std::string::size_type c1 = ts.find(',', c0);
          while (c1 != std::string::npos)
            {
              bs.back()[1] |= 1 << std::stoul(ts.substr(c0, c1 - c0));
              c0 = c1 + 1;
              c1 = ts.find(',', c0);
            }
          bs.back()[1] |= 1 << std::stoul(ts.substr(c0));
          bs.back()[0] |= 1 << cc++;
        }

      // First attempt
      // use recursion with permutations
      ans += permute(0, {{0, 0}}, lb, bs);

      // Second attempt
      // use recursion without permutations
      // std::vector<std::array<std::uint16_t, 2>> in({{0, 0}});
      // for (unsigned int i = 1; i <= bs.size(); ++i)
      //   if (permute_no_recursion(in, lb, bs))
      //     {
      //       ans += i;
      //       break;
      //     }
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
