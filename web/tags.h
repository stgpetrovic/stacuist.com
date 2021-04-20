#ifndef WEB_TAGS_H
#define WEB_TAGS_H

#include <Wt/WCheckBox.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WText.h>

#include "engine/recipe.h"

namespace stacuist::web {

// Shows available tags and allows filtering by them.
struct TagsWidget : public Wt::WContainerWidget {
 public:
  TagsWidget(const Wt::Dbo::collection<Wt::Dbo::ptr<engine::Tag>>& tags);
};

}  // namespace stacuist::web

#endif  // WEB_TAGS_H
