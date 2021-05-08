#include "web/recipe_view.h"

#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WText.h>

#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

RecipeView::RecipeView() {
  title_ = addNew<Wt::WText>();
  title_->setStyleClass("recipe-title");
  addNew<Wt::WBreak>();

  ingredients_ = addNew<Wt::WContainerWidget>();
  addNew<Wt::WBreak>();

  text_ = addNew<Wt::WText>();
  addNew<Wt::WBreak>();
}

void RecipeView::SetRecipe(const engine::Recipe& recipe) {
  recipe_ = recipe;
  title_->setText(recipe_.name);
  text_->setText(recipe_.text);

  ingredients_->clear();
  for (const auto& ingredient : recipe_.GetIngredients()) {
    auto cb = ingredients_->addNew<Wt::WCheckBox>(Wt::WString(ingredient));
    cb->setInline(false);
    cb->setChecked(false);
  }
}

void RecipeView::SetError(const std::string& error) { text_->setText(error); }

}  // namespace stacuist::web
