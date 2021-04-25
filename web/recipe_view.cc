#include "web/recipe_view.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WText.h>

#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

RecipeView::RecipeView() { text_ = addNew<Wt::WText>(); }

void RecipeView::SetRecipe(const engine::Recipe& recipe) {
  recipe_ = recipe;
  text_->setText(recipe_.text);
}

void RecipeView::SetError(const std::string& error) { text_->setText(error); }

}  // namespace stacuist::web
