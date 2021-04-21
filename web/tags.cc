#include "web/tags.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WText.h>

#include "engine/recipe.h"

namespace stacuist::web {

TagsWidget::TagsWidget(
    const Wt::Dbo::collection<Wt::Dbo::ptr<engine::Tag>>& tags) {
  Wt::WSelectionBox* sb = addNew<Wt::WSelectionBox>();
  for (const auto& tag : tags) {
    sb->addItem(tag->name);
  }
  sb->setSelectionMode(Wt::SelectionMode::Extended);
  sb->setMargin(10, Wt::Side::Right);

  Wt::WText* out = addNew<Wt::WText>();

  sb->activated().connect([=] {
    Wt::WString selected;

    std::set<int> newSelection = sb->selectedIndexes();
    for (std::set<int>::iterator it = newSelection.begin();
         it != newSelection.end(); ++it) {
      if (!selected.empty()) selected += ", ";

      selected += sb->itemText(*it);
    }

    out->setText(Wt::WString("Odabrani tagovi: {1}.").arg(selected));
  });
}

}  // namespace stacuist::web
