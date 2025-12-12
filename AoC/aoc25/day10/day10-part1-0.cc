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

class bitvec : public std::vector<bool>
{
public:
  using std::vector<bool>::vector;

  bitvec
  xorop(const bitvec &a) const
  {
    bitvec result = *this;
    for (unsigned int i = 0; i < this->size(); ++i)
      result[i] = result[i] ^ a[i];
    return result;
  };

  void
  out() const
  {
    for (const auto &b : *this)
      std::cout << b;
    std::cout << '\n';
  };
};

unsigned int
permute(unsigned int              count,
        const std::vector<bitvec> &in,
        const bitvec              &ans,
        const std::vector<bitvec> &ls)
{
  ++count;
  std::vector<bitvec> result;
  for (const auto & l : ls)
    for (const auto & i : in)
      {
        result.push_back(l.xorop(i));
        if (result.back() == ans)
          return count;
      }
  return permute(count, result, ans, ls);
};


int main(int, char**)
{
  std::ifstream f("input.day10", std::ios::in);
  unsigned int ans = 0;
  timer t;
  t.start();
  for (std::string ln; std::getline(f, ln);)
    {
      const auto ls = ln.substr(ln.find('[') + 1, ln.find(']') - 1);
      bitvec lb;
      for (const auto &c : ls)
        {
          if (c == '#')
            lb.push_back(1);
          else
            lb.push_back(0);
        }
      std::vector<bitvec> bs;
      std::string::size_type p0 = ln.find('(');
      std::string::size_type p1 = ln.find(')');
      while(p0 != std::string::npos || p1 != std::string::npos)
        {
          bs.push_back(bitvec(lb.size(), 0));
          const auto ts = ln.substr(p0 + 1, p1 - p0 - 1);
          p0 = ln.find('(', p1);
          p1 = ln.find(')', p0);
          std::string::size_type c0 = 0;
          std::string::size_type c1 = ts.find(',', c0);
          while (c1 != std::string::npos)
            {
              bs.back()[std::stoul(ts.substr(c0, c1 - c0))] = 1;
              c0 = c1 + 1;
              c1 = ts.find(',', c0);
            }
          bs.back()[std::stoul(ts.substr(c0))] = 1;
        }
      ans += permute(0,
                               {bitvec(lb.size(), 0)},
                               lb,
                               bs);
      // std::cout << out << '\n';
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
