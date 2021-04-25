#ifndef WEB_TAGS_H
#define WEB_TAGS_H

#include <Wt/WCheckBox.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WSignal.h>
#include <Wt/WText.h>

#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

// Shows available tags and allows filtering by them.
struct TagsWidget : public Wt::WContainerWidget {
 public:
  TagsWidget(absl::Span<const std::string> tags,
             std::function<void(absl::Span<const std::string>)>
                 selection_change_callback);

  std::vector<std::string> selected_tags() { return selected_tags_; }

 private:
  Wt::Signal<absl::Span<const std::string>> tag_selection_changed_;
  std::vector<std::string> selected_tags_;
};

}  // namespace stacuist::web

#endif  // WEB_TAGS_H
