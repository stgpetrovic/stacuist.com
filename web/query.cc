#include "web/query.h"

#include <Wt/Dbo/Session.h>
#include <absl/status/status.h>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {
absl::StatusOr<engine::Recipe> GetRecipe(absl::Span<const std::string> tags,
                                         Wt::Dbo::Session* session) {
  const auto tag_string = absl::StrJoin(tags, ", ");
  Wt::Dbo::Transaction transaction{*session};
  std::string matcher = "1=1";
  if (!tags.empty()) {
    matcher = "";
    for (const auto& tag : tags) {
      if (!matcher.empty()) {
        matcher += " and ";
      }
      matcher += absl::StrCat("t.name like '", tag, "'");
    }
  }
  Wt::Dbo::collection<engine::RecipeInfo> recipe =
      session
          ->query<engine::RecipeInfo>(absl::StrCat(
              "select r.id, r.name, r.author, r.text, r.ingredients from "
              "recipe r where id in "
              "( "
              "select r.id from "
              "recipe r join recipe_tags rt on "
              "r.id = rt.recipe_id join tag t on t.id = rt.tag_id "
              "where ",
              matcher, ")"))
          .limit(1)
          .orderBy("random()")
          .resultList();
  if (recipe.size() > 0) {
    return engine::Recipe(*(recipe.begin()));
  }
  return absl::NotFoundError(
      absl::StrCat("No recipes found for tags: ", tag_string));
}

}  // namespace stacuist::web
