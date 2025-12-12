#include <chrono>
#include <fstream>
#include <string>
#include <iostream>
#include <set>
#include <cmath>
#include <map>
#include <algorithm>
#include <numeric>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

std::set<unsigned long>
factorization(const unsigned long &n)
{
  std::set<unsigned long> ans({1});
  for (unsigned long i = 2; i <= std::sqrt(n); ++i)
    if (n % i == 0)
      {
        ans.insert(i);
        ans.insert(n / i);
      }
  return ans;
};

std::set<unsigned long>
invalid_ids(const unsigned long &n)
{
  std::set<unsigned long> ans;
  if (n == 1)
    return ans;
  const auto ps = factorization(n);
  std::string temp;
  temp.reserve(n);
  for (const auto & p : ps)
    {
      for (unsigned long i = pow(10, p - 1); i < pow(10, p); ++i)
        {
          temp.clear();
          for (int j = 0; j < n / p; ++j)
            temp += std::to_string(i);
          ans.insert(std::stoul(temp));
        }
    }
  return ans;
}

int main (int, char**)
{
  timer t;
  t.start();
  std::ifstream f("input.test", std::ios::in);
  std::string ss;
  f >> ss;
  std::string::size_type p0 = 0;
  unsigned long ans = 0;
  std::string::size_type p1 = ss.find(',');
  do
    {
      const std::string r = ss.substr(p0, p1 - p0);
      const std::string::size_type d0 = r.find('-');
      const std::string r0 = r.substr(0, d0);
      const std::string r1 = r.substr(d0 + 1);
      p0 = p1 + 1;
      p1 = ss.find(',', p0);
      if (r0.length() % 2 && r1.length() % 2)
        continue;
      for (unsigned long n = std::stoul(r0); n <= std::stoul(r1); ++n)
        {
          const std::string ns = std::to_string(n);
          if (ns.length() + 1 % 2)
            if(ns.substr(0, ns.length()/2) == ns.substr(ns.length()/2))
              ans += n;
        }
    }
  while (p0 != 0);
  t.stop("Part 1: ");
  std::cout << ans << '\n';

  // Part 2:
  std::map<unsigned long, std::set<unsigned long>> cache; 
  p0 = 0;
  p1 = ss.find(',');
  unsigned long long ans2 = 0;
  t.start();
  do 
    {
      const std::string r = ss.substr(p0, p1 - p0);
      const std::string::size_type d0 = r.find('-');
      const std::string r0 = r.substr(0, d0);
      const std::string r1 = r.substr(d0 + 1);
      p0 = p1 + 1;
      p1 = ss.find(',', p0);

      for (unsigned long i = r0.length(); i <= r1.length(); ++i)
        if (cache.count(i) == 0)
          cache[i] = invalid_ids(i);
      for (unsigned long i = r0.length(); i <= r1.length(); ++i)
        {
          const auto s0 = cache[i];
          const auto lb = std::lower_bound(s0.begin(), s0.end(), std::stoul(r0));
          const auto ub = std::upper_bound(lb, s0.end(), std::stoul(r1));
          ans2 += std::accumulate(lb, ub, 0LL);
        }
    }
  while (p0 != 0);
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
