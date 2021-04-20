#ifndef ENGINE_RECIPE_H_
#define ENGINE_RECIPE_H_

#include <Wt/Dbo/Dbo.h>

#include <string>

#include "absl/strings/str_split.h"

namespace stacuist::engine {

class Recipe;

class Tag {
 public:
  std::string name;

  Wt::Dbo::collection<Wt::Dbo::ptr<Recipe> > recipes;

  template <class Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, name, "name");

    Wt::Dbo::hasMany(a, recipes, Wt::Dbo::ManyToMany, "recipe_tags");
  }
};

class Recipe {
 public:
  std::string name;
  std::string author;
  std::string text;
  std::string ingredients;

  Wt::Dbo::collection<Wt::Dbo::ptr<Tag> > tags;

  std::vector<std::string> GetIngredients() const {
    return absl::StrSplit(ingredients, ";");
  }

  template <class Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, name, "name");
    Wt::Dbo::field(a, author, "author");
    Wt::Dbo::field(a, text, "text");
    Wt::Dbo::field(a, ingredients, "ingredients");

    Wt::Dbo::hasMany(a, tags, Wt::Dbo::ManyToMany, "recipe_tags");
  }
};

}  // namespace stacuist::engine

#endif  // ENGINE_RECIPE_H_
