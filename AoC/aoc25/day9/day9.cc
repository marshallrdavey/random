#include <chrono>
#include <iostream>
#include <fstream>

#include <array>
#include <string>
#include <vector>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

/**
 * Avoiding std::abs for no real reason.
 */
inline unsigned long
area(const std::array<unsigned long, 2> &a,
     const std::array<unsigned long, 2> &b)
{
  return (a[0] > b[0] ? a[0] - b[0] + 1: b[0] - a[0] + 1) *
         (a[1] > b[1] ? a[1] - b[1] + 1: b[1] - a[1] + 1);
};

inline std::array<unsigned long, 4>
edge(const std::array<unsigned long, 2> &a,
     const std::array<unsigned long, 2> &b)
{
  return {a[0] < b[0] ? a[0] : b[0],
          a[0] > b[0] ? a[0] : b[0],
          a[1] < b[1] ? a[1] : b[1],
          a[1] > b[1] ? a[1] : b[1]};
};

int main(int, char**)
{
  std::ifstream f("input.day9", std::ios::in);
  timer t;
  std::vector<std::array<unsigned long, 2>> rs;
  t.start();
  for (std::string ln; std::getline(f, ln);)
    {
      const auto c = ln.find(',');
      rs.push_back({std::stoul(ln.substr(0, c)),
                    std::stoul(ln.substr(c + 1))});
    }
  unsigned long ans = 0;
  for (unsigned int i = 0; i < rs.size() - 1; ++i)
    for (unsigned int j = i + 1; j < rs.size(); ++j)
      ans = ans > area(rs[i], rs[j]) ? ans : area(rs[i], rs[j]);
  t.stop("Part 1: ");
  std::cout << ans << '\n';

  /**
   * Not generalizable. Dependent on the fact that the shape is mostly convex.
   * To make generalizable, each edge would need a classifier based on the path
   * being clockwise or counter clockwise (e.g., is the internal of the shape
   * up, down, left, or right of the edge).
   */
  std::vector<std::array<unsigned long, 4>> es;
  es.reserve(rs.size());
  t.start();
  for (unsigned int i = 0; i < rs.size(); ++i)
    es.push_back(edge(rs[i], rs[(i + 1) % rs.size()]));
  unsigned long ans2 = 0;
  std::array<unsigned long, 4> tmp;
  bool crossed;
  for (unsigned int i = 0; i < rs.size(); ++i)
    for (unsigned int j = i + 1; j < rs.size(); ++j)
      {
        crossed = false;
        if (area(rs[i], rs[j]) < ans2)
          continue;
        tmp = edge(rs[i], rs[j]);
        for (const auto & e : es)
          {
            if (tmp[0] < e[1] && 
                tmp[1] > e[0] &&
                tmp[2] < e[3] &&
                tmp[3] > e[2])
              {
                crossed = true;
                break;
              }
          }
        if (!crossed)
          ans2 = area(rs[i], rs[j]);
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
