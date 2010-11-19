#include <iostream>
#include "TabSepParser.hh"

int main(int argc, char **argv)
{
  double x[10000];
  double y[10000];
  TabSepParser parser;
  unsigned n_rows;
  parser.set_skip(0);		/* default */
  parser.add_column(0, x);
  parser.add_column(1, y);
  if (!parser.parse_file (argv[1], 100, &n_rows))
    {
      std::cerr << "error parsing " << argv[1] << std::endl;
      return 1;
    }
  for (unsigned i = 0; i < n_rows; i++)
    std::cerr << "row " << i << ": (" << x[i] << "," << y[i] << ")" << std::endl;
  return 0;
}
