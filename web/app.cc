#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <glog/logging.h>

#include "absl/algorithm/container.h"
#include "absl/strings/match.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/strip.h"
#include "absl/types/span.h"
#include "engine/recipe.h"
#include "web/recipe_view.h"
#include "web/tags.h"

namespace stacuist::web {

namespace {
constexpr absl::string_view kRecipe = "/recipe/";
}

class StaCuIstApplication : public Wt::WApplication {
 public:
  StaCuIstApplication(const Wt::WEnvironment &env,
                      std::unique_ptr<Wt::Dbo::Session> session);

  engine::Recipe GetRecipe(absl::Span<const std::string> tags);

 private:
  void ProcessPath(absl::string_view path);
  RecipeView *recipe_view_;
  std::unique_ptr<Wt::Dbo::Session> session_;
};

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
    recipe_view_->SetRecipe(GetRecipe({}));
    return;
  }

  Wt::Dbo::Transaction transaction{*session_};
  Wt::Dbo::ptr<engine::Recipe> recipe =
      session_->find<engine::Recipe>().where("id = ?").bind(recipe_id);
  if (!recipe) {
    LOG(WARNING) << "Unknown recipe id " << recipe_id;
    recipe_view_->SetRecipe(GetRecipe({}));
    return;
  }
  recipe_view_->SetRecipe(*recipe);
}

engine::Recipe StaCuIstApplication::GetRecipe(
    absl::Span<const std::string> tags) {
  LOG(INFO) << "\t--- Finding recipes for " << absl::StrJoin(tags, ", ");
  Wt::Dbo::Transaction transaction{*session_};
  std::string matcher = "1=1";
  if (!tags.empty()) {
    matcher = "";
    for (const auto &tag : tags) {
      if (!matcher.empty()) {
        matcher += " and ";
      }
      matcher += absl::StrCat("t.name like '", tag, "'");
    }
  }
  Wt::Dbo::collection<engine::RecipeInfo> recipe =
      session_
          ->query<engine::RecipeInfo>(absl::StrCat(
              "select r.id, r.name, r.author, r.text, r.ingredients from "
              "recipe r where id in "
              "( "
              "select r.id from "
              "recipe r join recipe_tags rt on "
              "r.id = rt.recipe_id join tag t on t.id = rt.tag_id "
              "where ",
              matcher, ")"))
          .limit(1)
          .orderBy("random()")
          .resultList();
  if (recipe.size() > 0) {
    return engine::Recipe(*(recipe.begin()));
  }
  return engine::Recipe();
}

StaCuIstApplication::StaCuIstApplication(
    const Wt::WEnvironment &env, std::unique_ptr<Wt::Dbo::Session> session)
    : Wt::WApplication(env), session_(std::move(session)) {
  setTitle("Šta ću ist?!");

  {
    Wt::Dbo::Transaction transaction{*session_};
    Wt::Dbo::collection<Wt::Dbo::ptr<engine::Tag>> tags =
        session_->find<engine::Tag>();
    root()->addWidget(std::make_unique<TagsWidget>(
        tags, [this](absl::Span<const std::string> tags) {
          recipe_view_->SetRecipe(GetRecipe(tags));
        }));
  }

  root()->addWidget(std::make_unique<Wt::WBreak>());

  recipe_view_ = root()->addWidget(std::make_unique<RecipeView>(GetRecipe({})));

  // Reconstruct the state from URL.
  if (internalPath() != "/") {
    ProcessPath(internalPath());
  }
}

};  // namespace stacuist::web

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);

  return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
    std::unique_ptr<Wt::Dbo::backend::Sqlite3> sqlite3{
        new Wt::Dbo::backend::Sqlite3("/home/slon/recipes.db")};
    auto session = std::make_unique<Wt::Dbo::Session>();
    session->setConnection(std::move(sqlite3));

    session->mapClass<stacuist::engine::Tag>("tag");
    session->mapClass<stacuist::engine::Recipe>("recipe");

    return std::make_unique<stacuist::web::StaCuIstApplication>(
        env, std::move(session));
  });
}

