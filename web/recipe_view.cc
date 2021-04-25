#include "web/recipe_view.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WText.h>

#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

RecipeView::RecipeView(const engine::Recipe& recipe) {
  text_ = addNew<Wt::WText>();
  SetRecipe(recipe_);
}

void RecipeView::SetRecipe(const engine::Recipe& recipe) {
  recipe_ = recipe;
  text_->setText(recipe_.text);
}

}  // namespace stacuist::web
