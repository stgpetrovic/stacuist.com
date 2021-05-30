#include "web/tags.h"

#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WText.h>

#include "absl/strings/str_join.h"
#include "absl/synchronization/mutex.h"
#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

TagsWidget::TagsWidget(absl::Span<const std::string> tags,
                       std::function<void(absl::Span<const std::string>)>
                           selection_change_callback) {
  auto hbox = setLayout(std::make_unique<Wt::WHBoxLayout>());

  for (const auto& tag : tags) {
    auto item = std::make_unique<Wt::WPushButton>(tag);

    {
      absl::WriterMutexLock l(&m_);
      btns_.insert(
          {tag, TagState{.tag = tag, .btn = item.get(), .selected = false}});
    }

    item->clicked().connect([this, tag] {
      selected_tags_.clear();
      {
        absl::WriterMutexLock l(&m_);
        btns_[tag].selected = !btns_[tag].selected;
        if (btns_[tag].selected) {
          btns_[tag].btn->addStyleClass("btn-success");
        } else {
          btns_[tag].btn->removeStyleClass("btn-success");
        }
      }

      selected_tags_.clear();
      absl::ReaderMutexLock l(&m_);
      for (const auto& [tag, state] : btns_) {
        if (state.selected) {
          selected_tags_.push_back(tag);
        }
      }

      tag_selection_changed_.emit(selected_tags_);
    });
    hbox->addWidget(std::move(item));
  }

  hbox->addWidget(std::make_unique<Wt::WBreak>());

  tag_selection_changed_.connect(selection_change_callback);
}

}  // namespace stacuist::web
