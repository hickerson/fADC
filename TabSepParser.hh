#include <string>
#include <vector>

class TabSepParser
{
public:
  TabSepParser();
  TabSepParser(char seperator);
  ~TabSepParser();

  void set_skip(unsigned n_skip);
  unsigned get_skip() const;
  void add_column(unsigned column_number, double *values, unsigned value_step = sizeof (double));
  void add_column(unsigned column_number, std::string *values, unsigned value_step = sizeof (std::string));
  //void add_column(const char *name, double *values, unsigned value_step = sizeof (double));
  //void add_column(const char *name, std::string *values, unsigned value_step = sizeof (std::string));

  bool parse_file(const char *filename, unsigned max_rows, unsigned *n_rows_out = 0);

private:

  enum ColumnType
  {
    COLUMN_STRING,
    COLUMN_DOUBLE
  };
  struct Column
  {
    Column(unsigned column, ColumnType type, void *values, unsigned value_step)
      : /*has_name_(false),*/ column_(column), type_(type), values_(values), value_step_(value_step) { }
    //Column(const char *name, ColumnType type, void *values, unsigned value_step)
      //: has_name_(true), name_(name), type_(type), values_(values), value_step_(value_step) { }

    //bool has_name_;
    //std::string name_;
    unsigned column_;
    ColumnType type_;
    void *values_;
    unsigned value_step_;
  };
  unsigned skip_lines_;
  char seperator_;
  std::vector<Column> columns_;
  static unsigned split_tabsep(char *buf, char **fields, unsigned max_fields);
};

