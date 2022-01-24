#ifndef ENGINE_RECIPE_H_
#define ENGINE_RECIPE_H_

#include <Wt/Dbo/Dbo.h>

#include <string>

#include "absl/strings/str_split.h"

namespace stacuist::engine {

// For complex SQL querying.
using RecipeInfo =
    std::tuple<int32_t, std::string, std::string, std::string, std::string>;

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
  int32_t id;
  std::string name;
  std::string author;
  std::string text;
  std::string ingredients;

  Recipe() = default;

  Recipe(const RecipeInfo& recipe_info) {
    id = std::get<0>(recipe_info);
    name = std::get<1>(recipe_info);
    author = std::get<2>(recipe_info);
    text = std::get<3>(recipe_info);
    ingredients = std::get<4>(recipe_info);
  }

  Wt::Dbo::collection<Wt::Dbo::ptr<Tag> > tags;

  std::vector<std::string> GetIngredients() const {
    return absl::StrSplit(ingredients, "|");
  }

  template <class Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, id, "id");
    Wt::Dbo::field(a, name, "name");
    Wt::Dbo::field(a, author, "author");
    Wt::Dbo::field(a, text, "text");
    Wt::Dbo::field(a, ingredients, "ingredients");

    Wt::Dbo::hasMany(a, tags, Wt::Dbo::ManyToMany, "recipe_tags");
  }
};

}  // namespace stacuist::engine

#endif  // ENGINE_RECIPE_H_
