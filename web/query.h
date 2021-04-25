#ifndef WEB_QUERY_H
#define WEB_QUERY_H

#include <Wt/Dbo/Session.h>

#include "absl/status/statusor.h"
#include "absl/types/span.h"
#include "engine/recipe.h"

namespace stacuist::web {

// Gets a random recipe that matches the tags. If no tags specified, returns a
// random recipe over all recipes.
absl::StatusOr<engine::Recipe> GetRecipe(absl::Span<const std::string> tags,
                                         Wt::Dbo::Session* session);

}  // namespace stacuist::web

#endif  // WEB_QUERY_H
