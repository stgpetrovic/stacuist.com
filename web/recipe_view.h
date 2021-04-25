#ifndef WEB_RECIPE_VIEW_H
#define WEB_RECIPE_VIEW_H

#include <Wt/WCheckBox.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WSignal.h>
#include <Wt/WText.h>

#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

// Shows the recipe.
struct RecipeView : public Wt::WContainerWidget {
 public:
  RecipeView();

  void SetRecipe(const engine::Recipe& recipe);
  void SetError(const std::string& error);

 private:
  engine::Recipe recipe_;
  Wt::WText* text_;
};

}  // namespace stacuist::web

#endif  // WEB_RECIPE_VIEW_H
