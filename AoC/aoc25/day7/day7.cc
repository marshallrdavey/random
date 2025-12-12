#include <chrono>
#include <iostream>
#include <fstream>

#include <numeric>
#include <set>
#include <string>
#include <vector>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

int main(int, char**)
{
  std::ifstream f("input.test", std::ios::in);
  std::string ln;
  std::getline(f, ln);
  std::set<std::string::size_type> tmp;
  auto it = tmp.begin();
  unsigned int n = 1;
  unsigned int ans = 0;
  timer t;
  t.start();
  std::set<std::string::size_type> ps({ln.find('S')});
  for (;getline(f,ln); ++n)
    {
      tmp.clear();
      if(n % 2)
        continue;
      for (const auto & p : ps)
        {
          if (ln[p] == '^')
            {
              tmp.emplace_hint(it, p - 1);
              it = tmp.end();
              tmp.emplace_hint(it, p + 1);
              it = tmp.end();
              ++ans;
            }
          else
            {
              tmp.emplace_hint(it, p);
              it = tmp.end();
            }
        }
      ps = tmp;
    }
  t.stop("Part 1: ");
  std::cout << ans << '\n';
  f.close();

  f.open("input.day7");
  std::getline(f, ln);
  ps.clear();
  tmp.clear();
  it = tmp.begin();
  n = 1;
  std::vector<unsigned long> cs(ln.length(), 0);
  t.start();
  ps.insert(ln.find('S'));
  cs[*ps.begin()] = 1;
  for (;getline(f,ln); ++n)
    {
      tmp.clear();
      if(n % 2)
        continue;
      for (const auto & p : ps)
        {
          if (ln[p] == '^')
            {
              tmp.emplace_hint(it, p - 1);
              it = tmp.end();
              tmp.emplace_hint(it, p + 1);
              it = tmp.end();
              cs[p - 1] += cs[p];
              cs[p + 1] += cs[p];
              cs[p] = 0;
            }
          else
            {
              tmp.emplace_hint(it, p);
              it = tmp.end();
            }
        }
      ps = tmp;
    }
  const unsigned long ans2 = std::accumulate(cs.begin(), cs.end(), 0ul);
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
