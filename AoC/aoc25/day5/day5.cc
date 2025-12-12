#include <chrono>
#include <iostream>
#include <fstream>

#include <algorithm>
#include <string>
#include <tuple>
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
  std::vector<std::pair<unsigned long, unsigned long>> ranges;
  unsigned int ans = 0;
  timer t;
  t.start();
  for (std::string ss; std::getline(f, ss);)
    {
      const auto pos = ss.find('-');
      if (pos == std::string::npos)
        break;
      ranges.emplace_back(std::stoul(ss.substr(0, pos)),
                          std::stoul(ss.substr(pos + 1)));
    }
  std::sort(ranges.begin(), ranges.end());
  for (std::string ss; std::getline(f, ss); )
    {
      const auto n = std::stoul(ss);
      for (const auto & r : ranges)
        if (n >= r.first && n <= r.second)
          {
            ++ans;
            break;
          }
    }
  t.stop("Part 1: ");
  std::cout << ans << '\n';

  unsigned long ans2 = 0;
  std::vector<std::pair<unsigned long, unsigned long>> merged({ranges[0]});
  t.start();
  for (unsigned int i = 1; i < ranges.size(); ++i)
    {
      auto &p = merged.back();
      if (ranges[i].first <= p.second + 1)
        p.second = p.second >= ranges[i].second ? p.second : ranges[i].second;
      else
        merged.push_back(ranges[i]);
    }
  for (const auto & r : merged)
    ans2 += r.second - r.first  + 1;
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
