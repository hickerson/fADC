#include "TabSepParser.hh"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINE_LENGTH		(1<<16)
#define MAX_FIELDS		256

TabSepParser::TabSepParser()
{
  seperator_ = '\t';
  skip_lines_ = 0;
}

TabSepParser::TabSepParser(char seperator)
{
  seperator_ = '\t';
  skip_lines_ = 0;
}

TabSepParser::~TabSepParser()
{
}

void TabSepParser::set_skip(unsigned skip)
{
  skip_lines_ = skip;
}
unsigned TabSepParser::get_skip() const
{
  return skip_lines_;
}

void TabSepParser::add_column(unsigned column_number, double *values, unsigned value_step)
{
  columns_.push_back(Column(column_number, COLUMN_DOUBLE, (void*) values, value_step));
}

void TabSepParser::add_column(unsigned column_number, std::string *values, unsigned value_step)
{
  columns_.push_back(Column(column_number, COLUMN_STRING, (void*) values, value_step));
}

unsigned TabSepParser::split_tabsep (char *buf, char **fields, unsigned max_fields)
{
  unsigned rv = 0;
  char *nl = strchr (buf, '\n');
  if (nl)
    *nl = 0;
  while (rv < max_fields && buf != NULL)
    {
      fields[rv++] = buf;
      buf = strchr (buf, '\t');
      //buf = strchr (buf, (int)seperator_);
      if (buf)
        *buf++ = 0;
    }
  return rv;
}

///void TabSepParser::add_column(const char *name, double *values, unsigned value_step)
///{
///  columns_.push_back(Column(name, COLUMN_DOUBLE, (void*) values, value_step));
///}
///
///void TabSepParser::add_column(const char *name, std::string *values, unsigned value_step)
///{
///  columns_.push_back(Column(name, COLUMN_STRING, (void*) values, value_step));
///}
///
bool TabSepParser::parse_file(const char *filename, unsigned max_rows, unsigned *n_rows_out)
{
  char buf[MAX_LINE_LENGTH];
  char *fields[MAX_FIELDS];
  FILE *fp = fopen (filename, "r");
  if (fp == NULL)
    return false;
  unsigned remaining_skip = skip_lines_;
  unsigned line_no = 1;
  while (remaining_skip > 0)
    {
      if (fgets (buf, sizeof (buf), fp) == NULL)
        {
	  std::cerr << "end-of-file skipping header (file " << filename << "," << "line " << line_no << ")" << std::endl;
	  fclose (fp);
	  return false;
	}
      line_no++;
    }
  unsigned row = 0;
  while (row < max_rows && fgets (buf, sizeof (buf), fp) != NULL)
    {
      unsigned n_columns = split_tabsep (buf, fields, MAX_FIELDS);
      for (std::vector<TabSepParser::Column>::iterator iter = columns_.begin();
           iter != columns_.end();
	   ++iter)
        {
	  if (iter->column_ >= n_columns)
	    {
	      std::cerr << "column " << iter->column_ << " out-of-bounds (file " << filename << "," << "line " << line_no << ")" << std::endl;
	      fclose (fp);
	      return false;
	    }
	  const char *col_str = fields[iter->column_];
	  char *at = (char*)(iter->values_) + iter->value_step_ * row;
	  switch (iter->type_)
	    {
	    case COLUMN_STRING:
	      * (std::string *) at = fields[iter->column_];
	      break;
	    case COLUMN_DOUBLE:
	      * (double *) at = strtod (fields[iter->column_], 0);
	      break;
	    }
	}
      line_no++;
      row++;
    }
  *n_rows_out = row;
  fclose (fp);
  return true;
}
