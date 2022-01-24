#ifndef WEB_APP_H
#define WEB_APP_H

#include <Wt/WApplication.h>

#include "engine/recipe.h"
#include "web/query.h"
#include "web/recipe_view.h"
#include "web/tags.h"

namespace stacuist::web {

class StaCuIstApplication : public Wt::WApplication {
 public:
  StaCuIstApplication(const Wt::WEnvironment &env,
                      std::unique_ptr<Wt::Dbo::Session> session);

 private:
  void ProcessPath(absl::string_view path);
  void SetRecipe(const absl::StatusOr<engine::Recipe> &recipe);

  TagsWidget *tags_view_;
  RecipeView *recipe_view_;
  std::unique_ptr<Wt::Dbo::Session> session_;
};

};  // namespace stacuist::web

#endif  // WEB_APP_H
