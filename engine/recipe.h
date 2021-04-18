#include <Wt/Dbo/Dbo.h>

#include <string>

namespace stacuist::engine {

class Recipe;

class Ingredient {
 public:
  std::string name;
  float quantity;

  Wt::Dbo::collection<Wt::Dbo::ptr<Recipe> > recipes;

  template <class Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, name, "name");
    Wt::Dbo::field(a, quantity, "quantity");

    Wt::Dbo::hasMany(a, recipes, Wt::Dbo::ManyToMany, "recipe_ingredients");
  }
};

class Recipe {
 public:
  std::string name;
  std::string author;
  std::string text;
  std::string tags;

  Wt::Dbo::collection<Wt::Dbo::ptr<Ingredient> > ingredients;

  template <class Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, name, "name");
    Wt::Dbo::field(a, author, "author");
    Wt::Dbo::field(a, text, "text");
    Wt::Dbo::field(a, tags, "tags");

    Wt::Dbo::hasMany(a, ingredients, Wt::Dbo::ManyToMany, "recipe_ingredients");
  }
};

}  // namespace stacuist::engine
