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
      "A sta ne bi ovo?",
      "Ma najlakse ovako",
      "Ajde probaj ovo",
      "Ni ora ni kopa, a ludilo",
      "Ajde baci oko na ovi",
      "Lipo ti ovako",
      "Kuvacu u ruke i ajmo",
      "Nemas se sta mislit",
      "Ajmo, rizi kapulu",
      "Vidi vako",
      "Nije sarma, al aj",
      "Ćirni ovo",
      "Ko se misli ostane gladan",
      "Ajmo, dica su gladna",
      "Samo ti pomalo",
  };
  return hints->at(absl::Uniform(bitgen_, 0u, hints->size()));
}

RecipeView::RecipeView() {
  hint_ = addNew<Wt::WText>();
  addNew<Wt::WBreak>();
  title_ = addNew<Wt::WText>();
  addNew<Wt::WBreak>();
  ingredients_ = addNew<Wt::WContainerWidget>();

  addNew<Wt::WBreak>();

  text_ = addNew<Wt::WText>();
  addNew<Wt::WBreak>();
}

void RecipeView::SetRecipe(const engine::Recipe& recipe) {
  recipe_ = recipe;
  title_->setText(absl::StrCat("<h3>", recipe_.name, "</h3>"));
  text_->setText(absl::StrReplaceAll(recipe_.text, {{"\n", "<br/>"}}));
  hint_->setText(absl::StrCat("<em>", RandomHint(), "</em>"));

  ingredients_->clear();
  for (const auto& ingredient : recipe_.GetIngredients()) {
    auto cb = ingredients_->addNew<Wt::WCheckBox>(Wt::WString(ingredient));
    if (absl::EndsWith(ingredient, ":")) {
      cb->setEnabled(false);
      cb->setStyleClass("btn");
      cb->setStyleClass("btn-info");
    }
    cb->setInline(false);
    cb->setChecked(false);
  }
}

void RecipeView::SetError(const std::string& error) { text_->setText(error); }

}  // namespace stacuist::web
