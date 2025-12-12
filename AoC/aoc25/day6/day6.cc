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

int main(int, char**)
{
  std::ifstream f("input.test", std::ios::in);
  std::vector<unsigned long> vs;
  unsigned int nl = 0;
  timer t;
  t.start();
  for (std::string ss; std::getline(f, ss); ++nl)
    {
      std::string::size_type p0 = ss.find_first_not_of(' ');
      std::string::size_type p1 = ss.find(' ', p0);
      while(p0 != std::string::npos)
        {
          const auto sb = ss.substr(p0, p1 - p0);
          if (sb == "+")
            vs.push_back(0);
          else if (sb == "*")
            vs.push_back(1);
          else
            vs.push_back(std::stoul(sb));
          p0 = ss.find_first_not_of(' ', p1);
          p1 = ss.find(' ', p0);
        }
    }
  const unsigned int nc = vs.size() / nl;
  unsigned long ans = 0;
  for (unsigned int i = 0; i < nc; ++i)
    {
      unsigned long tmp = vs[i];
      switch(vs[nl * nc - nc + i])
        {
          case 0:
            for (unsigned int j = 1; j < nl - 1; ++j)
              tmp += vs[nc * j + i];
            break;
          case 1:
            for (unsigned int j = 1; j < nl - 1; ++j)
              tmp *= vs[nc * j + i];
        }
      ans += tmp;
    }
  t.stop("Part 1: ");
  std::cout << ans << '\n';
  f.close();

  f.open("input.day6");
  std::vector<std::string> ls;
  std::string::size_type p;
  std::vector<char> os;
  std::vector<std::string::size_type> ps;
  nl = 0;
  t.start();
  for (std::string ss; std::getline(f, ss); ++nl)
    {
      if (ss.find('+') == std::string::npos)
        {
          ls.push_back(ss);
          continue;
        }
      p = ss.find_first_not_of(' ');
      while (p != std::string::npos)
        {
          if (ss[p] == '+')
            { 
              os.push_back(0);
              ps.push_back(p);
            }
          else if (ss[p] == '*')
            {
              os.push_back(1);
              ps.push_back(p);
            }
          p = ss.find_first_not_of(' ', p + 1);
        }
      ps.push_back(ss.length() + 1);
    }
  nl -= 1;
  std::vector<unsigned long> as;
  std::string tmp;
  unsigned long ans2 = 0, tn = 0;
  for (unsigned int i = 0; i < os.size(); ++i)
    {
      as.clear();
      for (unsigned int j = ps[i]; j < ps[i + 1] - 1; ++j)
        {
          tmp.clear();
          for (const auto & ln : ls)
            if (ln[j] != ' ')
              tmp += ln[j];
          as.push_back(std::stoul(tmp));
        }
      tn = as.front();
      if (os[i] == 0)
        for (auto it = as.begin() + 1; it != as.end(); ++it)
          tn += *it;
      else if (os[i] == 1)
        for (auto it = as.begin() + 1; it != as.end(); ++it)
          tn *= *it;
      ans2 += tn;
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
