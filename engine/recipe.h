#include <Wt/Dbo/Dbo.h>

#include <string>

namespace stacuist::engine {

class Recipe {
 public:
  std::string name;
  std::string author;
  std::string text;
  std::string tags;

  template <class Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, name, "name");
    Wt::Dbo::field(a, author, "author");
    Wt::Dbo::field(a, text, "text");
    Wt::Dbo::field(a, tags, "tags");
  }
};

};  // namespace stacuist::engine
