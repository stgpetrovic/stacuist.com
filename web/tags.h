#ifndef WEB_TAGS_H
#define WEB_TAGS_H

#include <Wt/WCheckBox.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WPushButton.h>
#include <Wt/WSignal.h>
#include <Wt/WText.h>

#include "absl/container/flat_hash_map.h"
#include "absl/synchronization/mutex.h"
#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

struct TagState {
  std::string tag;
  Wt::WPushButton* btn;
  bool selected;
};

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
  absl::Mutex m_;
  absl::flat_hash_map<std::string, TagState> btns_ ABSL_GUARDED_BY(m_);
};

}  // namespace stacuist::web

#endif  // WEB_TAGS_H
