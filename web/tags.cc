#include "web/tags.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WText.h>

#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

TagsWidget::TagsWidget(
    const Wt::Dbo::collection<Wt::Dbo::ptr<engine::Tag>>& tags,
    std::function<void(absl::Span<const std::string>)>
        selection_change_callback) {
  Wt::WSelectionBox* sb = addNew<Wt::WSelectionBox>();
  for (const auto& tag : tags) {
    sb->addItem(tag->name);
  }
  sb->setSelectionMode(Wt::SelectionMode::Extended);
  sb->setMargin(10, Wt::Side::Right);

  sb->activated().connect([=] {
    std::vector<std::string> selected;

    std::set<int> newSelection = sb->selectedIndexes();
    for (std::set<int>::iterator it = newSelection.begin();
         it != newSelection.end(); ++it) {
      selected.push_back(sb->itemText(*it).toUTF8());
    }

    tag_selection_changed_.emit(selected);
  });

  tag_selection_changed_.connect(selection_change_callback);
}

}  // namespace stacuist::web
