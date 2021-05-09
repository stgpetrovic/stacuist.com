#include "web/recipe_view.h"

#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WText.h>

#include "absl/random/random.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

std::string RecipeView::RandomHint() {
  static const auto* const hints = new std::vector<std::string>{
      "A sta ne bi ovo?", "Ma najlakse ovako.",
      "Ajde probaj ovo:", "Ni ora ni kopa, a ludilo.", "Ajde baci oko na ovi:"};
  return hints->at(absl::Uniform(bitgen_, 0u, hints->size()));
}

RecipeView::RecipeView() {
  hint_ = addNew<Wt::WText>();
  addNew<Wt::WBreak>();
  title_ = addNew<Wt::WText>();
  addNew<Wt::WBreak>();

  addNew<Wt::WBreak>();

  text_ = addNew<Wt::WText>();
  addNew<Wt::WBreak>();
}

void RecipeView::SetRecipe(const engine::Recipe& recipe) {
  recipe_ = recipe;
  title_->setText(absl::StrCat("<h2>", recipe_.name, "</h2>"));
  text_->setText(absl::StrReplaceAll(recipe_.text, {{"\n", "<br/>"}}));
  hint_->setText(RandomHint());

  ingredients_->clear();
  for (const auto& ingredient : recipe_.GetIngredients()) {
    auto cb = ingredients_->addNew<Wt::WCheckBox>(Wt::WString(ingredient));
    cb->setInline(false);
    cb->setChecked(false);
  }
}

void RecipeView::SetError(const std::string& error) { text_->setText(error); }

}  // namespace stacuist::web
