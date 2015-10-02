#include "bench_framework.hpp"
#include <cstring>
#include <cstdlib>
#include "../plugins/input/csv/csv_utils.hpp"

class test : public benchmark::test_case
{
public:
    std::string line_data_;
    test(mapnik::parameters const& params)
     : test_case(params),
       line_data_("this is one line\nand this is a second line\nand a third line")
       {
          boost::optional<std::string> line_data = params.get<std::string>("line");
          if (line_data)
          {
              line_data_ = *line_data;
          }
       }

    bool validate() const
    {
        std::string first = line_data_.substr(line_data_.find_first_not_of('\n'));
        char newline = '\n';
        std::string csv_line;
        std::stringstream s;
        s << line_data_;
        std::getline(s,csv_line,s.widen(newline));
        if (csv_line != first)
        {
            return true;
        }
        else
        {
            std::clog << "Error: the parsed line (" << csv_line << ") should be a subset of the original line (" << line_data_ << ") (ensure you pass a line with a \\n)\n";
        }
        return true;
    }
    bool operator()() const
    {
        char newline = '\n';
        std::string csv_line;
        std::stringstream s;
        s << line_data_;
        auto newline_widened = s.widen(newline);
        for (unsigned i=0;i<iterations_;++i)
        {
            std::getline(s,csv_line,newline_widened);
        }
        return true;
    }
};


class test2 : public benchmark::test_case
{
public:
    std::string line_data_;
    test2(mapnik::parameters const& params)
     : test_case(params),
       line_data_("this is one line\nand this is a second line\nand a third line")
       {
          boost::optional<std::string> line_data = params.get<std::string>("line");
          if (line_data)
          {
              line_data_ = *line_data;
          }
       }

    bool validate() const
    {
        std::string first = line_data_.substr(line_data_.find_first_not_of('\n'));
        char newline = '\n';
        std::string csv_line;
        std::stringstream s;
        s << line_data_;
        csv_utils::getline_csv(s,csv_line,s.widen(newline));
        if (csv_line != first)
        {
            return true;
        }
        else
        {
            std::clog << "Error: the parsed line (" << csv_line << ") should be a subset of the original line (" << line_data_ << ") (ensure you pass a line with a \\n)\n";
        }
        return true;
    }
    bool operator()() const
    {
        char newline = '\n';
        std::string csv_line;
        std::stringstream s;
        s << line_data_;
        auto newline_widened = s.widen(newline);
        for (unsigned i=0;i<iterations_;++i)
        {
            csv_utils::getline_csv(s,csv_line,newline_widened);
        }
        return true;
    }
};

int main(int argc, char** argv)
{
    int return_value = 0;
    try
    {
        mapnik::parameters params;
        benchmark::handle_args(argc,argv,params);
        {
            test test_runner(params);
            return_value = return_value | run(test_runner,"std::getline");
        }
        {
            test2 test_runner2(params);
            return_value = return_value | run(test_runner2,"csv_utils::getline_csv");
        }
    }
    catch (std::exception const& ex)
    {
        std::clog << ex.what() << "\n";
        return -1;
    }
    return return_value;
}
