#include "web/app.h"

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>

#include "absl/algorithm/container.h"
#include "absl/strings/match.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/strip.h"
#include "absl/types/span.h"
#include "engine/recipe.h"
#include "web/query.h"
#include "web/recipe_view.h"
#include "web/tags.h"

namespace stacuist::web {

namespace {
constexpr absl::string_view kRecipe = "/recipe/";
}  // namespace

StaCuIstApplication::StaCuIstApplication(
    const Wt::WEnvironment &env, std::unique_ptr<Wt::Dbo::Session> session)
    : Wt::WApplication(env), session_(std::move(session)) {
  auto c = std::make_unique<Wt::WContainerWidget>();
  auto fit = c->setLayout(std::make_unique<Wt::WVBoxLayout>());

  // Logo.
  auto h1 = fit->addWidget(std::make_unique<Wt::WText>());
  h1->setText(
      "<h1>Šta ću ist?! <a "
      "href='https://github.com/stgpetrovic/stacuist.com' "
      "target='_blank' class='btn btn-primary'>github</a></h1>");

  // Set the theme.
  auto theme = std::make_shared<Wt::WBootstrapTheme>();
  theme->setVersion(Wt::BootstrapVersion::v3);
  theme->setResponsive(true);
  setTheme(theme);

  // Add an external style sheet to the application.
  Wt::WApplication::instance()->useStyleSheet("style/main.css");

  // Set up the app.
  setTitle("Šta ću ist?!");

  std::vector<std::string> tag_names;
  {
    Wt::Dbo::Transaction transaction{*session_};
    Wt::Dbo::collection<Wt::Dbo::ptr<engine::Tag>> tags =
        session_->find<engine::Tag>();
    for (const auto &tag : tags) {
      tag_names.push_back(tag->name);
    }
    tags_view_ = fit->addWidget(std::make_unique<TagsWidget>(
        tag_names, [this](absl::Span<const std::string> tags) {
          SetRecipe(GetRecipe(tags, session_.get()));
        }));
  }

  fit->addWidget(std::make_unique<Wt::WBreak>());

  // Button for reloading the recipe using the same filters.
  auto reload_button =
      fit->addWidget(std::make_unique<Wt::WPushButton>("Daj nešto drugo"));
  reload_button->clicked().connect([this] {
    SetRecipe(GetRecipe(tags_view_->selected_tags(), session_.get()));
  });

  recipe_view_ = fit->addWidget(std::make_unique<RecipeView>());

  if (internalPath() != "/") {
    ProcessPath(internalPath());
  } else {
    SetRecipe(GetRecipe({}, session_.get()));
  }

  root()->addWidget(std::move(c));
}

void StaCuIstApplication::ProcessPath(absl::string_view path) {
  if (!absl::StartsWith(path, kRecipe)) {
    return;
  }

  absl::string_view p(path);
  absl::ConsumePrefix(&p, kRecipe);

  Wt::Dbo::Transaction transaction{*session_};
  Wt::Dbo::ptr<engine::Recipe> recipe =
      session_->find<engine::Recipe>().where("name = ?").bind(std::string(p));
  if (!recipe) {
    recipe_view_->SetError(
        absl::StrCat("nema ti te ricete odi, odi na gugl i trazi tamo ", p));
    return;
  }
  SetRecipe(*recipe);
}

void StaCuIstApplication::SetRecipe(
    const absl::StatusOr<engine::Recipe> &recipe) {
  if (!recipe.ok()) {
    recipe_view_->SetError(std::string(recipe.status().message()));
    return;
  }
  recipe_view_->SetRecipe(*recipe);
}

};  // namespace stacuist::web
