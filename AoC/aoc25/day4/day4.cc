#include <chrono>
#include <iostream>
#include <fstream>

#include <vector>
#include <string>

struct timer 
{
  std::chrono::time_point<std::chrono::system_clock> m_time;
  inline void start();
  inline void stop(const std::string &msg = "") const;
};

int main(int, char**)
{
  std::ifstream f("input.test", std::ios::in);
  std::vector<std::string> in;
  unsigned int nl = 0;
  unsigned int nc = 0;
  for (std::string ss; std::getline(f, ss);)
    {
      if (nl == 0)
        {
          in.emplace_back(std::string(ss.length() + 2, '.'));
          nc = ss.length() + 2;
          nl += 1;
        }
      in.emplace_back('.' + ss + '.');
    }
  in.emplace_back(in.front());
  nl = in.size();

  auto neighbors = [&](const unsigned int i, const unsigned int j)
    {
      return (in[i - 1][j - 1] != '.') +
             (in[i - 1][j] != '.') +
             (in[i - 1][j + 1] != '.') +
             (in[i][j - 1] != '.') +
             (in[i][j + 1] != '.') +
             (in[i + 1][j - 1] != '.') +
             (in[i + 1][j] != '.') +
             (in[i + 1][j + 1] != '.');
    };

  unsigned int ans = 0;
  timer t;
  t.start();
  for (unsigned int i = 1; i < nl - 1; ++i)
    for (unsigned int j = 1; j < nc - 1; ++j)
      if (in[i][j] != '.')
        if(neighbors(i, j) < 4)
          ans++;
  t.stop("Part 1: ");
  std::cout << ans << '\n';

  unsigned int count = 0;
  unsigned int ans2 = 0;
  std::vector<unsigned int> rm;
  t.start();
  do 
    {
      rm.clear();
      for (unsigned int i = 1; i < nl - 1; ++i)
        for (unsigned int j = 1; j < nc - 1; ++j)
          if (in[i][j] != '.')
            if(neighbors(i, j) < 4)
              {
                ans2++;
                rm.emplace_back(i);
                rm.emplace_back(j);
              }
      count = rm.size();
      for (unsigned int i = 0; i < rm.size() /2; ++i)
        in[rm[2 * i]][rm[2 * i + 1]] = '.';
    }
  while (count != 0);
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
