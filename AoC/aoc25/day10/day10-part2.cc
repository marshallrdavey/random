#include <glpk.h>

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
  std::ifstream f("input.day10", std::ios::in);
  int ans = 0;

  // GLP initialization
  glp_prob* ilp;
  glp_term_out(GLP_OFF);
  glp_iocp ilp_param;
  glp_init_iocp(&ilp_param);
  ilp_param.msg_lev = GLP_MSG_OFF;
  ilp_param.presolve = GLP_ON;

  timer t;
  t.start();
  for (std::string ln; std::getline(f, ln);)
    {
      const std::size_t col_length = ln.find(']') - ln.find('[') - 1;
      std::vector<std::vector<double>> cols;
      std::string::size_type p0 = ln.find('(');
      std::string::size_type p1 = ln.find(')');
      while(p0 != std::string::npos || p1 != std::string::npos)
        {
          cols.push_back(std::vector<double>(col_length, 0.0));
          const auto ts = ln.substr(p0 + 1, p1 - p0 - 1);
          p0 = ln.find('(', p1);
          p1 = ln.find(')', p0);
          std::string::size_type c0 = 0;
          std::string::size_type c1 = ts.find(',', c0);
          while (c1 != std::string::npos)
            {
              cols.back()[std::stoul(ts.substr(c0, c1 - c0))] = 1.0;
              c0 = c1 + 1;
              c1 = ts.find(',', c0);
            }
          cols.back()[std::stoul(ts.substr(c0))] = 1.0;
        }

      std::vector<int> bnds(col_length);
      p0 = ln.find('{');
      p1 = ln.find('}');
      const auto as = ln.substr(p0 + 1, p1 - p0 - 1);
      p0 = 0;
      p1 = as.find(',', p0);
      std::size_t idx = 0;
      while (p1 != std::string::npos)
        {
          bnds[idx++] = std::stoi(as.substr(p0, p1 - p0));
          p0 = p1 + 1;
          p1 = as.find(',', p0);
        }
      bnds[idx] = std::stoi(as.substr(p0));
      const std::size_t row_length = cols.size();

      // Construct the matrix vectors. The glp internal loops start at 1 so we
      // need a dummy value at the front of each vector, hence the lengths. This
      // feels psychotic when using C/C++;
      std::vector<int>
        is(row_length * col_length + 1), js(row_length * col_length + 1);
      std::vector<double> ms(row_length * col_length + 1);
      for (std::size_t i = 0; i < row_length; ++i)
        for (std::size_t j = 0; j < col_length; ++j)
          {
            is[i * col_length + j + 1]  = (int)(j + 1);
            js[i * col_length + j + 1]  = (int)(i + 1);
            ms[i * col_length + j + 1] = cols[i][j];
          }

      // Create minimization problem
      ilp = glp_create_prob();
      glp_set_obj_dir(ilp, GLP_MIN);

      // Add desired system solutions as exact
      glp_add_rows(ilp, (int)col_length);
      for (std::size_t i = 0; i < col_length; ++i)
        glp_set_row_bnds(ilp, (int)(i + 1), GLP_FX, bnds[i], 0);

      // Add x_i >= 0, x_i need to be integers, and the objective coefficients
      glp_add_cols(ilp, (int)row_length);
      for (std::size_t i = 1; i <= row_length; ++i)
        {
          glp_set_col_bnds(ilp, (int)i, GLP_LO, 0.0, 0.0);
          glp_set_col_kind(ilp, (int)i, GLP_IV);
          glp_set_obj_coef(ilp, (int)i, 1.0); 
        }

      // Set matrix 
      glp_load_matrix(
          ilp, (int)(ms.size() - 1), is.data(), js.data(), ms.data());
      
      // solve problem
      glp_intopt(ilp, &ilp_param);
      ans += glp_mip_obj_val(ilp);
      glp_delete_prob(ilp);
    }
  t.stop("Part 2: ");
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
