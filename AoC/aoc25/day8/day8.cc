#include <chrono>
#include <iostream>
#include <fstream>

#include <algorithm>
#include <array>
#include <vector>
#include <tuple>
#include <set>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

inline unsigned long
dist(const std::array<unsigned long, 3> &a,
     const std::array<unsigned long, 3> &b)
{
  return (a[0] - b[0]) * (a[0] - b[0]) +
         (a[1] - b[1]) * (a[1] - b[1]) +
         (a[2] - b[2]) * (a[2] - b[2]);
};

int main(int, char**)
{
  std::ifstream f("input.day8", std::ios::in);
  timer t;
  std::vector<std::array<unsigned long, 3>> ps;
  std::string::size_type c0, c1;
  t.start();
  for (std::string ln; std::getline(f, ln);)
    {
      c0 = ln.find(',');
      c1 = ln.find(',', c0 + 1);
      ps.push_back({std::stoul(ln.substr(0, c0)),
                    std::stoul(ln.substr(c0 + 1, c1 - c0 - 1)),
                    std::stoul(ln.substr(c1 + 1))});
    }
  std::vector<std::tuple<unsigned long, unsigned int, unsigned int>> ts;
  for (unsigned int i = 0; i < ps.size() - 1; ++i)
    for (unsigned int j = i + 1; j < ps.size(); ++j)
      ts.emplace_back(dist(ps[i], ps[j]), i, j);
  std::sort(ts.begin(), ts.end());

  std::vector<std::set<unsigned int>> ss(1); 
  ss.back().emplace_hint(ss.back().begin(), std::get<1>(ts[0]));
  ss.back().emplace_hint(ss.back().end(), std::get<2>(ts[0]));
  unsigned int s0, s1;
  for (unsigned int i = 1; i < 1000; ++i)
    {
      s0 = -1; 
      s1 = -1;
      const auto &t0 = std::get<1>(ts[i]);
      const auto &t1 = std::get<2>(ts[i]);
      for (unsigned int j = 0; j < ss.size(); ++j)
        {
          if (ss[j].count(t0) == 1 && ss[j].count(t1) == 1)
            {
              s0 = j;
              s1 = j;
              break;
            }
          else if (ss[j].count(t0) == 1)
            s0 = j;
          else if (ss[j].count(t1) == 1)
            s1 = j;
        }
      if (s0 == -1 && s1 == -1)
        {
          ss.emplace_back();
          ss.back().emplace_hint(ss.back().begin(), std::get<1>(ts[i]));
          ss.back().emplace_hint(ss.back().end(), std::get<2>(ts[i]));
        }
      else if (s0 == s1)
        continue;
      else if (s0 == -1)
        ss[s1].insert(t0);
      else if (s1 == -1)
        ss[s0].insert(t1);
      else
        ss[s0].merge(ss[s1]);
    }
  std::sort(ss.begin(), ss.end(),
      [](const std::set<unsigned int> &a,
         const std::set<unsigned int> &b)
      {return a.size() > b.size();});
  const unsigned int ans = ss[0].size() * ss[1].size() * ss[2].size();
  t.stop("Part 1: ");
  std::cout << ans << '\n';

  unsigned long ans2 = 0;
  t.start();
  for (unsigned int i = 1000; i < ts.size(); ++i)
    {
      s0 = -1; 
      s1 = -1;
      const auto &t0 = std::get<1>(ts[i]);
      const auto &t1 = std::get<2>(ts[i]);
      for (unsigned int j = 0; j < ss.size(); ++j)
        {
          if (ss[j].count(t0) == 1 && ss[j].count(t1) == 1)
            {
              s0 = j;
              s1 = j;
              break;
            }
          else if (ss[j].count(t0) == 1)
            s0 = j;
          else if (ss[j].count(t1) == 1)
            s1 = j;
        }
      if (s0 == -1 && s1 == -1)
        {
          ss.emplace_back();
          ss.back().emplace_hint(ss.back().begin(), std::get<1>(ts[i]));
          ss.back().emplace_hint(ss.back().end(), std::get<2>(ts[i]));
          continue;
        }
      else if (s0 == s1)
        continue;
      else if (s0 == -1)
        {
          ss[s1].insert(t0);
          s0 = s1;
        }
      else if (s1 == -1)
        {
          ss[s0].insert(t1);
          s1 = s0;
        }
      else
        ss[s0].merge(ss[s1]);
      if (ss[s0].size() == 1000 || ss[s1].size() == 1000)
        {
          ans2 = ps[t0][0] * ps[t1][0];
          break;
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
