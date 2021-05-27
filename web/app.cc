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
#include <glog/logging.h>

#include "absl/algorithm/container.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
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

ABSL_FLAG(std::string, db_path, "", "path to the sqlite3 database");

namespace stacuist::web {

namespace {
constexpr absl::string_view kRecipe = "/recipe/";
}  // namespace

class StaCuIstApplication : public Wt::WApplication {
 public:
  StaCuIstApplication(const Wt::WEnvironment &env,
                      std::unique_ptr<Wt::Dbo::Session> session);

 private:
  void ProcessPath(absl::string_view path);
  void SetRecipe(const absl::StatusOr<engine::Recipe> &recipe);

  TagsWidget *tags_view_;
  RecipeView *recipe_view_;
  std::unique_ptr<Wt::Dbo::Session> session_;
};

void StaCuIstApplication::SetRecipe(
    const absl::StatusOr<engine::Recipe> &recipe) {
  LOG(INFO) << "Setting recipe, success = " << recipe.ok();
  if (!recipe.ok()) {
    LOG(WARNING) << "Failed loading recipe...";
    recipe_view_->SetError(std::string(recipe.status().message()));
    return;
  }
  recipe_view_->SetRecipe(*recipe);
  setInternalPath(absl::StrCat(kRecipe, recipe->id));
}

void StaCuIstApplication::ProcessPath(absl::string_view path) {
  if (!absl::StartsWith(path, kRecipe)) {
    return;
  }

  LOG(WARNING) << "\t--- Processing URL: " << path;
  absl::string_view p(path);
  absl::ConsumePrefix(&p, kRecipe);
  int32_t recipe_id;
  if (!absl::SimpleAtoi(p, &recipe_id)) {
    LOG(WARNING) << "Failed to parse URL to recipe id: " << path;
    SetRecipe(GetRecipe({}, session_.get()));
    return;
  }

  Wt::Dbo::Transaction transaction{*session_};
  Wt::Dbo::ptr<engine::Recipe> recipe =
      session_->find<engine::Recipe>().where("id = ?").bind(recipe_id);
  if (!recipe) {
    LOG(WARNING) << "Unknown recipe id " << recipe_id;
    recipe_view_->SetError(absl::StrCat("unknown recipe id: ", recipe_id));
    return;
  }
  SetRecipe(*recipe);
}

StaCuIstApplication::StaCuIstApplication(
    const Wt::WEnvironment &env, std::unique_ptr<Wt::Dbo::Session> session)
    : Wt::WApplication(env), session_(std::move(session)) {
  auto c = std::make_unique<Wt::WContainerWidget>();
  auto fit = c->setLayout(std::make_unique<Wt::WVBoxLayout>());

  root()->setContentAlignment(Wt::AlignmentFlag::Center);
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
      fit->addWidget(std::make_unique<Wt::WPushButton>("Neću to"));
  reload_button->clicked().connect([this] {
    SetRecipe(GetRecipe(tags_view_->selected_tags(), session_.get()));
  });

  recipe_view_ = fit->addWidget(std::make_unique<RecipeView>());

  // Reconstruct the state from URL.
  if (internalPath() != "/") {
    ProcessPath(internalPath());
  } else {
    SetRecipe(GetRecipe({}, session_.get()));
  }

  root()->addWidget(std::move(c));
}

};  // namespace stacuist::web

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  absl::ParseCommandLine(argc, argv);

  return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
    LOG(INFO) << "Loading databse: " << absl::GetFlag(FLAGS_db_path);
    std::unique_ptr<Wt::Dbo::backend::Sqlite3> sqlite3{
        new Wt::Dbo::backend::Sqlite3(absl::GetFlag(FLAGS_db_path))};
    auto session = std::make_unique<Wt::Dbo::Session>();
    session->setConnection(std::move(sqlite3));

    session->mapClass<stacuist::engine::Tag>("tag");
    session->mapClass<stacuist::engine::Recipe>("recipe");

    return std::make_unique<stacuist::web::StaCuIstApplication>(
        env, std::move(session));
  });
}

