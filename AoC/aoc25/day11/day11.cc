#include <chrono>
#include <iostream>
#include <fstream>

#include <set>
#include <map>
#include <vector>
#include <string>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

unsigned long
paths_a_b(std::map<std::string, std::set<std::string>> &mp,
          const std::string &start,
          const std::string &stop)
{
  unsigned long result = 0ul;
  std::map<std::string, unsigned long> m0;
  m0[start] = 1ul;
  std::map<std::string, unsigned long> m1;
  while (!m0.empty())
    {
      for (const auto & m : m0)
        {
          if (m.first == "out" || m.first == stop)
            continue;
          for (const auto & k : mp[m.first])
            m1[k] += m.second;
        }
      if (m1.count(stop) == 1)
        result += m1[stop];
      m0 = m1;
      m1.clear();
    }
  return result;
}

int main(int, char**)
{
  std::ifstream f("input.test-part1", std::ios::in);
  timer t;
  std::map<std::string, std::set<std::string>> mp;
  t.start();
  for (std::string ln; std::getline(f, ln);)
    {
      auto p0 = ln.find(':');
      const auto key = ln.substr(0, p0++);
      mp[key] = {};
      do 
        {
          p0 = ln.find_first_not_of(' ', p0);
          mp[key].insert(ln.substr(p0, 3));
          p0 += 3;
        }
      while (p0 != ln.size());
    }
  const auto ans = paths_a_b(mp, "you", "out");
  t.stop("Part 1: ");
  std::cout << ans << '\n';

  /**
   * One of fft or dac will always have to be "above" the other or else you
   * could end up in a cycle. See that paths_a_b(dac, fft) == 0. Since we know
   * the order, we just need to count the number of paths between each step.
   *
   * You could add a counts variable to the paths_a_b input to verify, but its
   * multiplicative (see math class).
   */
  unsigned long ans2 = 1ul;
  t.start();
  ans2 *= paths_a_b(mp, "svr", "fft");
  ans2 *= paths_a_b(mp, "fft", "dac");
  ans2 *= paths_a_b(mp, "dac", "out");
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
