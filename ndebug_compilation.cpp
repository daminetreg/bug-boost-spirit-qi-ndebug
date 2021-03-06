#include <iostream>
#include <typeinfo>
#include <cassert>

#include <boost/variant.hpp>
#include <boost/spirit/include/qi.hpp>

typedef boost::variant<int, bool, std::string> frame;

struct parse_asserter : public boost::static_visitor<> {

  frame _expected;
  parse_asserter(frame expected) 
    : _expected(expected) {}

  template <typename T>
  void operator()(T const& val) const
  {
    std::cout << typeid(T).name() << " " << val << std::endl;
    assert(boost::get<T>(_expected) == val);
  }

};

int main() {
  typedef std::string::const_iterator Iterator;

  using namespace boost::spirit::qi;

  rule<Iterator, frame()> r = 
      int_
    | bool_
    | as_string[*char_]
  ;

  {
    frame frm{};
    const std::string text = "this is a string";

    assert( parse(text.cbegin(), text.cend(), r, frm)  );
    boost::apply_visitor(parse_asserter{text}, frm);
  }

  {
    frame frm{};
    const std::string text = "42";

    assert( parse(text.cbegin(), text.cend(), r, frm) );
    boost::apply_visitor(parse_asserter{42}, frm);
  }

  {
    frame frm{};
    const std::string text = "true";

    assert( parse(text.cbegin(), text.cend(), r, frm) );
    boost::apply_visitor(parse_asserter{true}, frm);
  }


  return EXIT_SUCCESS;
}
